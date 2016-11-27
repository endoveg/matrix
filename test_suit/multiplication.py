#!/usr/bin/python3
import numpy as np
import re
from subprocess import call

N = 20
MAXINT = 25
MININT = -25
NUMBER_OF_TESTS = 1000
f = 0
for i in range(NUMBER_OF_TESTS):
	A = np.random.randint(MININT, MAXINT, size=(N,N))
	B = np.random.randint(MININT, MAXINT, size=(N,N))
	C = np.dot(A,B)

	fa = open("A","w")
	fb = open("B", "w")
	fa.write(re.sub('[\[\]]', '', np.array_str(A)))
	fb.write(re.sub('[\[\]]', '', np.array_str(B)))
	fa.close()
	fb.close()
	ret = call("../test")
	C_res = np.loadtxt("C",dtype='int')
	if not np.array_equal(C,C_res):
		f = f + 1
		print("Test failed.\n Expected:")
		print(C)
		print("Result:")
		print(C_res)
		print(C-C_res)
if f > 0:
	print(str(f) + "test(s) failed\n")
else:
	print("Nailed it(" + str(NUMBER_OF_TESTS) + " tests) ")
