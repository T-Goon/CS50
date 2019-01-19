import cs50

rows=-1
while rows>23 or rows<0:
    rows= cs50.get_int("Height: ")

temp=1
while temp<=rows:
    print (" "*(rows-temp)+"#"*temp+"  "+"#"*temp)
    temp+=1