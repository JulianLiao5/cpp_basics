#!/usr/bin/env python
# coding=utf-8

from math import sqrt
import numpy as np

def standard_deviation(lst, population=True):
    """Calculates the standard deviation for a list of numbers."""
    num_items = len(lst)
    mean = sum(lst) / num_items
    differences = [x - mean for x in lst]
    sq_differences = [d ** 2 for d in differences]
    ssd = sum(sq_differences)

    # Note: it would be better to return a value and then print it outside
    # the function, but this is just a quick way to print out the values along
    # the way.
    if population is True:
        print('This is POPULATION standard deviation.')
        variance = ssd / num_items
    else:
        print('This is SAMPLE standard deviation.')
        variance = ssd / (num_items - 1)
    sd = sqrt(variance)
    # You could `return sd` here.

    print('The mean of {} is {}.'.format(lst, mean))
    print('The differences are {}.'.format(differences))
    print('The sum of squared differences is {}.'.format(ssd))
    print('The variance is {}.'.format(variance))
    print('The standard deviation is {}.'.format(sd))
    print('--------------------------')


s = [98, 127, 133, 147, 170, 197, 201, 211, 255]
standard_deviation(s)
standard_deviation(s, population=False)

mean_s = np.mean(s)
std_dev_s = np.std(s)
sample_std_s = np.std(s, ddof = 1)
print("mean_s: " + str(mean_s) + ", population std_dev_s: " + str(std_dev_s) + ", sample std_dev_s: " + str(sample_std_s))
