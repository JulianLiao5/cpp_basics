

import numpy as np
import matplotlib.pyplot as plt

# step = 3
#     [0 3]
# step = 2
#     [0 2 4]
# step = 1
#     [0 1 2 3 4]
# step = 0.5
#     [0.0 0.5 1.0 1.5 2.0 2.5 3.0 3.5 4.0 4.5]
#
X = np.arange(0, 5, 0.1)
Z = [3 + 5 * x for x in X]
Y = [np.random.normal(z, 0.5) for z in Z]

plt.figure(1)
plt.plot(X, Y, 'ro')


def linear_regression(x, y):
	N = len(x)
	sumx = sum(x)
	sumy = sum(y)
	sumx2 = sum(x**2)
	sumxy = sum(x*y)
	print "sumx: " + str(sumx) + ", sumy: " + str(sumy) + ", sumx2: " + str(sumx2) + ", sumxy: " + str(sumxy) + "\n"

    # N = 50
	A = np.mat([[N, sumx], [sumx, sumx2]])
	print A
	b = np.array([sumy, sumxy])
	print b

	return np.linalg.solve(A, b)

a0, a1 = linear_regression(X, Y)
b0, b1 = np.polyfit(X, Y, 1)
print "a0: " + str(a0) + ", a1: " + str(a1)
print "b0: " + str(b0) + ", b1: " + str(b1)

_X = [0, 5]
_Y = [a0 + a1 * x for x in _X]

__X = [0, 5]
__Y = [b1 + b0 * x for x in __X]

plt.figure(2)
plt.plot(X, Y, 'ro', _X, _Y, 'b', __X, __Y, 'g', linewidth=2)
plt.title("y = {} + {}x".format(a0, a1))
plt.show()
