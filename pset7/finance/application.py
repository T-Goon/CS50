from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions
from werkzeug.security import check_password_hash, generate_password_hash
import datetime
import decimal

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure responses aren't cached


@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # declarations
    tot = 0
    stocks2 = []
    flag = False
    blank = []

    # get name of stock and amount of shares from table
    stockData = db.execute("SELECT name,amount FROM portfolio WHERE userId=:id", id=session["user_id"])

    # get rid of duplicates
    for i in stockData:

        for j in range(len(stocks2)):
            if stocks2[j]['name'] == i['name']:
                stocks2[j]['amount'] = stocks2[j]['amount'] + i['amount']
                flag = True
        if not flag:
            stocks2.append(i)
        flag = False

    # calcuate total value of each position and get new price via lookup
    for i in range(len(stocks2)):
        newData = lookup(stocks2[i]["name"])
        stocks2[i]["price"] = '{0:.2f}'.format(newData["price"])
        stocks2[i]["total"] = '{0:.2f}'.format(round((float(stocks2[i]["price"]) * stocks2[i]["amount"]), 2))

    # calcualte total value of stocks
    for i in range(len(stocks2)):
        tot = round(tot + float(stocks2[i]["total"]), 2)

    # get cash from data base
    balance = db.execute("SELECT cash FROM users WHERE id=:id", id=session["user_id"])
    balance[0]["cash"] = round(balance[0]["cash"], 2)

    # show index.html
    return render_template("index.html", stocks=stocks2, totalValue='{0:.2f}'.format(tot + balance[0]["cash"]), cash=balance[0]["cash"])


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # show buy.html
    if request.method == "GET":

        return render_template("buy.html")

    # when button pressed do this
    elif (request.method == "POST"):

        # get symbol from form
        symbol = request.form.get("symbol")

        # if symbol is upper case make it lower case
        if symbol.isupper():
            symbol = symbol.lower()

        # lookup symbol
        quoteDict = lookup(symbol)

        # symbol is invalid if empty
        if quoteDict == None:
            return apology("Invalid Symbol")

        # set price to seperate variable
        price = quoteDict["price"]

        # set shares to a variable
        amount = int(request.form.get("shares"))

        # if shares is not and interger give apology
        if type(amount) != int:
            return apology("Enter a number")

        # case to int
        amount = int(amount)

        # make sure it a positive number
        if amount <= 0:
            return apology("Enter a positive number")

        # get the user's cash
        cash = db.execute("SELECT cash FROM users WHERE id=:id", id=session["user_id"])

        # if the user has enough cash buy the stock
        if cash[0]["cash"] >= (price * int(amount)):
            db.execute("INSERT INTO portfolio (userId, name, amount, price) VALUES (:id, :name, :amount, :price)",
                       id=session['user_id'], name=quoteDict['name'], amount=amount, price=price)
            db.execute("UPDATE users SET cash=cash-(:price *:amount) WHERE id=:id",
                       id=session["user_id"], price=price, amount=amount)
            db.execute("INSERT INTO history (symbol, amount, dateTime, price, type, userId) VALUES (:symbol, :amount, :dateTime, :price, :type, :id)",
                       symbol=quoteDict["name"], amount=amount, dateTime=datetime.datetime.now(), price=price, type="buy", id=session["user_id"])
        # if there is not enough cash return apology
        else:
            return apology("Not Enough Cash")

        # go to index.html
        return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    stockHistoryData = db.execute(
        "SELECT symbol, amount, dateTime, price, type FROM history WHERE userId=:id", id=session['user_id'])

    return render_template('history.html', stocks=stockHistoryData)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")

    if request.method == "POST":

        quoteDict = lookup(request.form.get("symbol"))

        if quoteDict == None:
            return apology("Invalid Symbol")

        return render_template("quoted.html", name=quoteDict["name"], price=quoteDict["price"], symbol=quoteDict["symbol"])


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User Sumbitted form
    if request.method == "POST":

        # Check if there is a username
        if not request.form.get("username"):
            return apology("Please Enter a Username")

        # Check if there is a password
        elif not request.form.get("password"):
            return apology("Please Enter a Password")

        # Check if there is a password confirmation
        elif not request.form.get("confirmation"):
            return apology("Please Confirm Your Password")

        # Check if password and confirmation match
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("Passwords do not Match")

        else:

            # store username and password hash
            username = request.form.get("username")
            hash = generate_password_hash(request.form.get("password"))
            exists = db.execute("SELECT username FROM users")

            # check if usename is already taken
            if {'username': username} in exists:
                return apology("Username is Already Taken")

            # Add data to database and login the newly registered user
            else:
                db.execute("INSERT INTO users (username,hash) VALUES(:username,:hash)",
                           username=username, hash=hash)

                temp = db.execute("SELECT id FROM users WHERE username=:username", username=username)

                session["user_id"] = temp[0]['id']

                return redirect("/")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "GET":
        if(db.execute("SELECT userId FROM portfolio WHERE userId=:id", id=session["user_id"]) != None):
            name = db.execute("SELECT name FROM portfolio WHERE userId=:id", id=session["user_id"])

            flag = False
            stock = []

            for i in name:

                for j in range(len(stock)):
                    if stock[j]['name'] == i['name']:
                        flag = True
                if not flag:
                    stock.append(i)
                flag = False

        return render_template("sell.html", stocks=stock)
    elif request.method == "POST":

        toSell = int(request.form.get("shares"))

        if toSell <= 0:
            return apology("Enter a Positive Number")

        symbol = request.form.get("symbol")

        if symbol == "symbol":
            return apology("Choose a stock")

        shares = db.execute("SELECT amount FROM portfolio WHERE name=:name AND userId=:id", name=symbol, id=session["user_id"])

        while len(shares) > 1:

            shares[0]["amount"] = shares[0]["amount"] + shares[1]["amount"]
            del shares[1]

        if shares[0]["amount"] < toSell:

            return apology("You Don't Own Enough Shares")

        shares = db.execute("SELECT amount,transactionId FROM portfolio WHERE name=:name AND userId=:id",
                            name=symbol, id=session["user_id"])

        for i in shares:

            if toSell >= i["amount"]:

                toSell = toSell - i["amount"]
                db.execute("DELETE FROM portfolio WHERE transactionId=:id", id=i["transactionId"])

            elif toSell < i["amount"] and toSell != 0:

                i["amount"] = i["amount"] - toSell
                db.execute("UPDATE portfolio SET amount=:amount WHERE transactionId=:id", amount=i["amount"], id=i["transactionId"])
                toSell = 0

            if toSell == 0:

                break

        stockData = lookup(symbol)

        balance = db.execute("SELECT cash FROM users WHERE id=:id", id=session["user_id"])
        balance[0]["cash"] = round(balance[0]["cash"], 2)

        money = (balance[0]["cash"] + (stockData["price"] * int(request.form.get("shares"))))

        db.execute("UPDATE users SET cash=:cash WHERE id=:id", id=session["user_id"], cash=money)

        db.execute("INSERT INTO history (symbol, amount, dateTime, price,type, userId) VALUES (:symbol, :amount, :dateTime, :price, :type, :id)", symbol=symbol,
                   amount=int(request.form.get("shares")), dateTime=datetime.datetime.now(), price=stockData['price'], type='sell', id=session['user_id'])

        return redirect("/")


@app.route("/addCash", methods=["GET", "POST"])
@login_required
def addCash():
    if request.method == "GET":
        money = db.execute("SELECT cash FROM users WHERE id=:id", id=session["user_id"])
        return render_template("addCash.html", cash=round(money[0]['cash'], 2))
    elif request.method == "POST":
        toAdd = request.form.get("toAdd")
        toAdd = float(toAdd)
        if toAdd <= 0:
            return apology("Enter a Postive Number")
        money = db.execute("SELECT cash FROM users WHERE id=:id", id=session["user_id"])
        db.execute("UPDATE users SET cash=:cash WHERE id=:id", cash=toAdd + money[0]['cash'], id=session["user_id"])
        money = db.execute("SELECT cash FROM users WHERE id=:id", id=session["user_id"])
        return render_template("addCash.html", cash=round(money[0]['cash'], 2))


def errorhandler(e):
    """Handle error"""
    return apology(e.name, e.code)


# listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
