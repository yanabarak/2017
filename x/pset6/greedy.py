#!/usr/bin/env python3

import cs50

def main():
    counter = 0
    n = get_positive()
    ns = round(n*100)
    while ns>=25:
        ns = ns - 25
        counter += 1
    while ns>=10:
        ns = ns - 10
        counter += 1
    while ns>=5:
        ns = ns - 5
        counter += 1
    while ns>=1:
        ns = ns - 1
        counter += 1
    print("{}".format(counter))
    
    
def get_positive():
    while True:
        print("O hai! How much change is owed?")
        n = cs50.get_float()
        if n > 0:
            break
    return n
    
    
if __name__ == "__main__":
    main()