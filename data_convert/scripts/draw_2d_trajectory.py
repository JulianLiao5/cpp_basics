#!/usr/bin/env python
import matplotlib.pyplot as plt
import matplotlib as mpl
import numpy as np
import sys
import os
import subprocess
import math

GPS_FIX = 4
GPS_FLOAT = 3
COLOR_FIX = '-b'
COLOR_FLOAT = '-r'
COLORS = ['-y', '-g', '-k', '-m', '-c']
RAD2DEG = 180 / math.pi
WEST = (270 * math.pi/ 180)
MAG_OFFSET = (0 * math.pi/ 180)

MAG_LEN = 30

# X_VIO_T_NED = -0.02
# Y_VIO_T_NED = 0.165
X_VIO_T_NED = -0.02
Y_VIO_T_NED = 0.165

def load_gps_trajectory_with_file(filename):
    m = np.loadtxt(filename, delimiter=" ")
    flag = [vec[5] for vec in m]
    x_gps_fix = [vec[1] for vec in m if vec[5] == GPS_FIX]
    y_gps_fix = [vec[2] for vec in m if vec[5] == GPS_FIX]
    x_gps_float = [vec[1] for vec in m if vec[5] == GPS_FLOAT]
    y_gps_float = [vec[2] for vec in m if vec[5] == GPS_FLOAT]
    # reverse haier theta to positive in order to compare
    theta = [vec[3] for vec in m]
    t = [vec[0] for vec in m]
    return x_gps_fix, y_gps_fix, x_gps_float, y_gps_float, t, theta

def load_vio_trajectory_with_file(filename, init_th):
    m = np.loadtxt(filename, delimiter=" ")
    # x = [((-1.0) * ((vec[1] * math.cos(WEST - init_th - MAG_OFFSET)) - X_VIO_T_NED)) for vec in m]
    # y = [((-1.0) * ((vec[2] * math.cos(WEST - init_th - MAG_OFFSET)) - Y_VIO_T_NED)) for vec in m]
    x = [((-1.0) * (vec[1] - X_VIO_T_NED)) for vec in m]
    y = [((-1.0) * (vec[2] - Y_VIO_T_NED)) for vec in m]
    theta = [(init_th - vec[3]) for vec in m]
    t = [vec[0] for vec in m]
    return x, y, t, theta

def main(filenames):
    assert isinstance(filenames, list)
    assert len(filenames) > 0
    assert len(filenames) <= len(COLORS)

    mpl.rcParams['legend.fontsize'] = 10
    gps_init_th = 0
    for i in range(0, len(filenames)):
        name = filenames[i]
        if i == 0:
            m = np.loadtxt(name, delimiter=" ")
            gps_heading_mag = [vec[4] for vec in m]
            for k in range(0, MAG_LEN):
                gps_init_th = gps_init_th + gps_heading_mag[k]
            gps_init_th = gps_init_th / MAG_LEN
            print gps_init_th

        try:
            if i == 0:
                x_gps_fix, y_gps_fix, x_gps_float, y_gps_float, t, theta = load_gps_trajectory_with_file(name)
            else:
                x, y, t, theta = load_vio_trajectory_with_file(name, gps_init_th)
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
        if i == 0:
            plt.plot(x_gps_fix, y_gps_fix, COLOR_FIX)
            plt.plot(x_gps_float, y_gps_float, COLOR_FLOAT)
        else:
            plt.plot(x, y, COLORS[i])
        plt.figure(2)
        plt.plot(t, theta, COLORS[i])
        print "plotting " + name
    plt.figure(1)
    if i == 0:
        plt.legend(filenames[0])
    else:
        plt.legend(filenames)
    plt.xlabel('x[m]')
    plt.ylabel('y[m]')
    font = {'family': 'serif',
        'color':  'darkred',
        'weight': 'normal',
        'size': 16,
        }
    plt.text(-60, 80, "Overall:" + "\n" + "    min: 0.000503" + "\n" + "    max: 5.511186" + "\n" + "    rms: 0.475902", fontdict=font)
    plt.title('2D x-y trajectory')
    plt.gca().set_aspect('equal', adjustable='box')
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