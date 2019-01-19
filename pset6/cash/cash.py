import cs50

cash=[.25,.10,.05,.01]

change=cs50.get_float("Change owed: $")

while change<0:
    change=cs50.get_float("Change owed: $")

coins=0

for i in range(0,len(cash)):
    while change>=cash[i]:
        change-=cash[i]
        coins+=1
        change=round(change,2)
print (coins)