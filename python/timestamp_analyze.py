#!/usr/bin/env python
# coding=utf-8

import numpy as np
import matplotlib.pyplot as plt
import sys


def main(filename):
    if "CameraTrajectory" in filename:
        m = np.loadtxt(filename, delimiter=" ")
        timestamp = [(vec[0] * 1000.0) for vec in m]
    elif "gps" in filename:
        m = np.loadtxt(filename, delimiter=",")
        timestamp = [vec[0] for vec in m]
        tow = [vec[1] for vec in m]

    first_t = timestamp[0]
    last_t = timestamp[len(timestamp) - 1]
    duration_t = last_t - first_t
    duration_min = int(duration_t) / (60 * 1000)
    duration_min_left = int(duration_t) % (60 * 1000)
    duration_sec = duration_min_left / 1000
    duration_sec_left = duration_min_left % 1000
    print(filename + ":\n    duration: " + str(duration_min) + " min, " + str(duration_sec) + " sec, " + str(duration_sec_left) + " ms")

    if "gps" in filename:
        first_tow = tow[0]
        last_tow = tow[len(tow) -1]
        duration_tow = last_tow - first_tow
        duration_tow_min = int(duration_tow) / (60 * 1000)
        duration_tow_min_left = int(duration_tow) % (60 * 1000)
        duration_tow_sec = duration_tow_min_left / 1000
        duration_tow_sec_left = duration_tow_min_left % 1000
        print(filename + ":\n    duration_tow: " + str(duration_tow_min) + " min, " + str(duration_tow_sec) + " sec, " + str(duration_tow_sec_left) + " ms")

    dt = np.diff(timestamp)
    avg_dt = sum(dt)/len(dt)
    print("    avg_delta_time: " + "{0:.4f}".format(avg_dt) + "ms")
    
    plt.figure(1)
    plt.plot(dt, '-r')
    plt.grid(True)
    '''
    plt.figure(2)
    plt.plot(dtow, '-b')
    plt.grid(True)
    '''
    # plt.show()

if __name__ == "__main__":
    if len(sys.argv) == 2:
        filename = sys.argv[1]
        main(filename)
    else:
        print("Usage: python " + sys.argv[0] + " CameraTrajectory_0515_1957.txt")
