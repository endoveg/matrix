#!/usr/bin/python3
# use to test make_minor

import numpy as np
import re
from subprocess import call
from random import randint

def make_minor(arr,i,j):
    arr = np.delete(arr,i,0)
    arr = np.delete(arr,j,1)
    return arr
N = 10
MAXINT = 25
MININT = -25
NUMBER_OF_TESTS = 1000
f = 0
for n in range(NUMBER_OF_TESTS):
    A = np.random.randint(MININT, MAXINT, size=(N,N))
    i = 0
    j = randint(0,N-1)
    minor = make_minor(A,i,j)
    fa = open("A", "w")
    fa.write(re.sub('[\[\]]', '', np.array_str(A)))
    fa.close()
    ret = call(["../test2",str(j)])
    minor_res = np.loadtxt("C", dtype='int')
    if not np.array_equal(minor, minor_res):
        f = f + 1
        print("Test failed.\n Expected:")
        print(minor)
        print("Result:")
        print(minor_res)        
if f > 0:
	print(str(f) + "test(s) failed\n")
else:
	print("Nailed it(" + str(NUMBER_OF_TESTS) + " tests) ")
