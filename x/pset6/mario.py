#!/usr/bin/env python3

import cs50

def main():
    height = get_positive_int() + 1
    for i in range(height):
        j = i
        new = height - i
        for new in range(new,0,-1):
            print(" ", end="")
        for i in range(i, 0, -1):
            print("#", end="")
        print("  ", end="")
        for j in range(j, 0, -1):
            print("#", end="")
        print("")
        
def get_positive_int():
    while True:
        print("Give me an integer number between 0 and 23:", end="")
        n = cs50.get_int()
        if n > 0 and n < 23:
            break
    return n

if __name__ == "__main__":
    main()