#!/usr/bin/env python3
import sys
import numpy as np

def sdbm(L):
    h = np.uint32(0)
    six = np.uint32(6)
    sixteen = np.uint32(16)
    for c in L:
        h = np.uint32(ord(c) + (h << six) + (h << sixteen) - h)
    return h

if __name__ == "__main__":
    print(sdbm(sys.argv[1]))
