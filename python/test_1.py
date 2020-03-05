#!/usr/bin/env python
# coding=utf-8

import numpy as np
print("A:" + np.__version__)
print("B:" + np.version.version)

##---------------BEGIN 1------------------
a = np.array([[3,1], [1,2]])
b = np.array([9,8])
x = np.linalg.solve(a, b)
print("result 11: " + str(x))
##---------------END 1------------------

##---------------BEGIN 2------------------
a = np.array([[64,256,1024], [48,256,1280], [24,192,1280]])
b = np.array([-9,0,0])
x = np.linalg.solve(a, b)
print("result 22: " + str(x))
##---------------END 2------------------

print(np.allclose(np.dot(a, x), b))
