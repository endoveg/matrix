#!/usr/bin/python3
# use to test make_minor

import numpy as np
import re
from subprocess import call
from random import randint

N = 12
MAXINT = 6 # det becomes too big
MININT = 0
NUMBER_OF_TESTS = 20
EPSILON = 10**(-3) # less than a 0.1%
f = 0
for n in range(NUMBER_OF_TESTS):
    A = np.random.randint(MININT, MAXINT, size=(N,N)) 
    fa = open("A", "w")
    fr = open("res", "r")
    fa.write(re.sub('[\[\]]', '', np.array_str(A)))
    fa.close()
    ret = call("../test3")
    det_res = float(fr.readline())
    det = float(np.linalg.det(A))
    if abs(det_res - det) / det_res  >= EPSILON:
        f = f + 1
        print("Test failed.\n Expected:")
        print(det)
        print("Result:")
        print(det_res)
        print(A)
    else:
        print("Test #"+str(n)+" :ok")
if f > 0:
	print(str(f) + "test(s) failed")
else:
	print("Nailed it(" + str(NUMBER_OF_TESTS) + " tests) ")
