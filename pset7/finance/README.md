# Finance

A python web app that allows users to create accounts and has the basic function of paper trading stocks.

### Login

A standard login and register page by hashing and storing passwords in an SQL database.

![image](https://user-images.githubusercontent.com/32044950/120088946-d767ec80-c0c3-11eb-8d88-4c5af2630d2d.png)

### Portfolio Page

The account "home page" after logging in. All of the current stock holdings of the account are displayed in a table. It also displays the account's currently free cash
and total account value.

The menu bar at the top of the screen allows to do a variety of different operations such as looking up a quote for a stock symbol, buying a stock, selling a stock, etc..

![image](https://user-images.githubusercontent.com/32044950/120089014-70970300-c0c4-11eb-9cfa-c91a6d315096.png)


### Quotes

Here users can look up the current prices of stock symbols. Prices are downloaded as a .csv file and parsed for the symbol info.

![image](https://user-images.githubusercontent.com/32044950/120089624-29ac0c00-c0ca-11eb-89d9-63d6bf3cbd78.png)
![image](https://user-images.githubusercontent.com/32044950/120089637-392b5500-c0ca-11eb-9cfb-e1f61950d9bb.png)


### Buying and Selling

Users are able to "buy" stocks by typing in the stock symbol and quantity they want to buy.

![image](https://user-images.githubusercontent.com/32044950/120089675-92938400-c0ca-11eb-92ae-48e889e8c057.png)

To sell a stock users can navigate to the sell page and select the stock symbol from the dropdown.

![image](https://user-images.githubusercontent.com/32044950/120090403-3e8b9e00-c0d0-11eb-9759-422927e1e229.png)


### Adding Cash

Cash can be added to the account in this menu.

![image](https://user-images.githubusercontent.com/32044950/120090411-5105d780-c0d0-11eb-8973-1e4379f0779e.png)

### History Page

The transaction history for the account is stored in the SQL database and is displayed in a table in the history menu.

![image](https://user-images.githubusercontent.com/32044950/120090417-6844c500-c0d0-11eb-8633-42e774e7313a.png)

## Running

This app was developed to run on the CS50 IDE.

It can be launched with `flask run`.
