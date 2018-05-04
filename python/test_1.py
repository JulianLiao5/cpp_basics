#!/usr/bin/env python
# coding=utf-8

import numpy as np
print "A:" + np.__version__
print "B:" + np.version.version

a = np.array([[3,1], [1,2]])
b = np.array([9,8])
x = np.linalg.solve(a, b)
print x

print np.allclose(np.dot(a, x), b)
