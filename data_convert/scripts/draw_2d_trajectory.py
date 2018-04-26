#!/usr/bin/env python
import matplotlib.pyplot as plt
import matplotlib as mpl
import numpy as np
import sys
import os
import subprocess
import math

COLORS = ['-b', '-r', '-g', '-m', '-c']
RAD2DEG = 180 / math.pi
WEST = (270 / 180) * math.pi
X_VIO_T_NED = -0.02
Y_VIO_T_NED = 0.165

def load_trajectory_with_file(filename, need_transfer, init_th):
    m = np.loadtxt(filename, delimiter=" ")
    if need_transfer:
        x = [((vec[1] * math.cos(WEST - init_th)) - X_VIO_T_NED) for vec in m]
        y = [((vec[2] * math.cos(WEST - init_th)) - Y_VIO_T_NED) for vec in m]
        theta = [(init_th - vec[3]) for vec in m]
        t = [vec[0] for vec in m]
    else:
        x = [vec[1] for vec in m]
        y = [vec[2] for vec in m]
        # reverse haier theta to positive in order to compare
        theta = [vec[3] for vec in m]
        t = [vec[0] for vec in m]
    return x, y, t, theta


def main(filenames):
    assert isinstance(filenames, list)
    assert len(filenames) > 0
    assert len(filenames) <= len(COLORS)

    mpl.rcParams['legend.fontsize'] = 10
    for i in range(0, len(filenames)):
        name = filenames[i]
        if i == 0:
            m = np.loadtxt(name, delimiter=" ")
            init_th = m[3][0];

            try:
                if i == 0:
                    x, y, t, theta = load_trajectory_with_file(name, false, init_th)
                else:
                    x, y, t, theta = load_trajectory_with_file(name, true, init_th)
            except IOError:
                print "Failed to load file [" + name + "]. :("
                sys.exit(-1)

        if i == 0:
            j = 0
            while j < len(theta):
                # Haier's output pose is scaled by 10 so divide by 10 to scale it back
                theta[j] = theta[j] * RAD2DEG
                j = j + 1
        if i == 1:
            j = 0
            while j < len(theta):
                while theta[j] > math.pi:
                    theta[j] = theta[j] - 2 * math.pi
                while theta[j] < -math.pi:
                    theta[j] = theta[j] + 2 * math.pi
                theta[j] = theta[j] * RAD2DEG
                j = j + 1
        plt.figure(1)
        plt.plot(x, y, COLORS[i])
        plt.figure(2)
        plt.plot(t, theta, COLORS[i])
        print "plotting " + name
    plt.figure(1)
    plt.legend(filenames)
    plt.xlabel('x[m]')
    plt.ylabel('y[m]')
    plt.title('2D x-y trajectory')
    plt.figure(2)
    plt.xlabel('timestamp[ms]')
    plt.ylabel('heading [deg]')
    plt.legend(filenames)
    plt.show()


if __name__ == "__main__":
    if len(sys.argv) == 3:
        file_names = [sys.argv[1], sys.argv[2]]
        main(file_names)
    elif len(sys.argv) == 4:
        file_names = [sys.argv[1], sys.argv[2], sys.argv[3]]
        main(file_names)

    else:
        print('Usage[1]:' + sys.argv[0] + ' | for auto download file from adb')
        print(
            'Usage[2]:' + sys.argv[0] + ' [pose1.csv] [pose2.csv] |for given file')
        print(
            'Usage[3]:' + sys.argv[0] + ' [pose1.csv] [pose2.csv] [pose3.csv] |for given file')