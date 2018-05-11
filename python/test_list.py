#!/usr/bin/env python
# coding=utf-8


import numpy as np
import sys

def main(filename):
    m = np.loadtxt(filename, delimiter=",")
    flag = [vec[6] for vec in m]
    flagset = set(flag)
    return flagset

'''
mylist = [u'nowplaying', u'PBS', u'PBS', u'nowplaying', u'job', u'debate', u'thenandnow']
myset = set(mylist)
print myset
'''



if __name__ == "__main__":
    if len(sys.argv) == 2:
        filename = sys.argv[1]
        flagset = main(filename)
        print flagset
