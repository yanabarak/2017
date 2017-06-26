#!/usr/bin/env python3
import sys
import cs50

def main():
    if len(sys.argv) != 2:
        print("Usage: ./vigenere k")
        exit(1)
    else:
        key = sys.argv[1]
        for z in range(len(key)):
            if key.isalpha() == 0:
                print("Usage: ./vigenere k")
                exit(1)
    print("plaintext: ", end="")
    p = cs50.get_string()
    print("ciphertext: ", end="")
    funcvigenere(p, key)
	
def funcvigenere(strl, key):
    jt = 0
    li = 0
    htc = 0
    for i in range(len(strl)):
        o = len(key)
        newstr = strl[i]
        if newstr.isalpha():
            jt = jt % o
            htc = key[jt].upper()
            htc = ord(htc)
            li = (htc - 65)%26
            if newstr.isupper():
                numb = ord(newstr)
                l = ((numb - 65) + li) % 26
                l = l + 65
                print("{}".format(chr(l)), end="")
            if newstr.islower():
                numb = ord(newstr)
                l = ((numb - 97) + li) % 26
                l = l + 97
                print("{}".format(chr(l)), end="")
            jt += 1
        else:
            print("{}".format(newstr), end="")
    print("")
        
	
if __name__ == "__main__":
    main()