import cs50
import sys

key=int(sys.argv[1])

ptext=cs50.get_string("plaintext: ")

print("ciphertext: ",end="")
for c in ptext:
    if(c.isupper()):
        a=ord(c)-65
        a=((a+key)%26)+65
        print("{}".format(chr(a)), end="")
    elif(c.islower()):
        a=ord(c)-97
        a=((a+key)%26)+97
        print("{}".format(chr(a)), end="")
    else:
        print("{}".format(c), end="")
print()