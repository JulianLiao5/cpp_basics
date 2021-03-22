#!/usr/bin/env python
import csv
import inspect
import math
import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec
import matplotlib as mpl
import numpy as np
import sys
import os
import re
import subprocess
from enum import Enum
from mpl_toolkits.mplot3d import Axes3D

COLORS = ['-b', '-r', '-g', '-m', '-c']
RAD2DEG = 180 / math.pi

# I1119 17:22:59.953446 20912 planner.cpp:1069] [planning] vehicle status: angle:2.41 speed:1.6127
LOG_PATTERN = '^(\S+)\s(\d{2}):(\d{2}):(\d{2}).(\d{6})\s(\d[0-9]+)\s(\S+):([0-9]+)\]\s*(.+)'


DURATION = 70  # in minutes

'''
'b' : (0.0, 0.0, 1.0)
'c' : (0.0, 0.75, 0.75)
'm' : (0.75, 0, 0.75)
'y' : (0.75, 0.75, 0)
'k' : (0.0, 0.0, 0.0)
'''

T_vehicle_radars = []
T_vehicle_sonars = []

Area = Enum('Area', 'unknown slowdown fusion secure out_lane_secure')
Sensor_Type = Enum('Sensor type', 'UNKNOWN VISION RADAR ULTRASONIC')
# for vision, 0 represents stixel, 1 represents deep_learning
Line_Type = Enum(
    'Line type', 'unknown find_obs sensor_type vehicle_coordinate planner')


def init():
    global T_vehicle_radars
    global T_vehicle_sonars
    T_vehicle_radars = dict({0: np.array([[1.000000, 0.000000, 2.10000], [
                            0.000000, 1.000000, 0.000000], [0, 0, 1]])})
    T_vehicle_radars.update({1: np.array(
        [[0.707107, 0.707107, 1.83], [-0.707107, 0.707107, -0.66], [0, 0, 1]])})
    T_vehicle_radars.update({2: np.array(
        [[0.000000, 1.000000, 0.935000], [-1.000000, 0.000000, -0.755000], [0, 0, 1]])})
    T_vehicle_radars.update({3: np.array(
        [[-0.670427, 0.741976, -0.695000], [-0.741976, -0.670427, -0.728000], [0, 0, 1]])})
    T_vehicle_radars.update({4: np.array(
        [[-1.000000, 0.000000, -0.730000], [0.000000, -1.000000, 0.000000], [0, 0, 1]])})
    T_vehicle_radars.update({5: np.array(
        [[0.707107, -0.707107, 1.83], [0.707107, 0.707107, 0.660000], [0, 0, 1]])})
    T_vehicle_radars.update({6: np.array(
        [[0.000000, -1.000000, 0.935000], [1.000000, 0.000000, 0.770000], [0, 0, 1]])})
    T_vehicle_radars.update({7: np.array(
        [[0.685818, -0.727773, 2.805000], [0.727773, 0.685818, 0.740000], [0, 0, 1]])})
    T_vehicle_sonars = dict({0: np.array([[1.000000, 0.000000, 2.960000], [
                            0.000000, 1.000000, 0.385000], [0, 0, 1]])})
    T_vehicle_sonars.update({1: np.array(
        [[1.000000, 0.000000, 2.960000], [0.000000, 1.000000, -0.385000], [0, 0, 1]])})
    T_vehicle_sonars.update({2: np.array(
        [[0.000000, 1.000000, 2.100000], [-1.000000, 0.000000, -0.740000], [0, 0, 1]])})
    T_vehicle_sonars.update({3: np.array(
        [[0.000000, 1.000000, -0.440000], [-1.000000, 0.000000, -0.735000], [0, 0, 1]])})
    T_vehicle_sonars.update({4: np.array(
        [[-1.000000, 0.000000, -0.790000], [0.000000, 1.000000, -0.283000], [0, 0, 1]])})
    T_vehicle_sonars.update({5: np.array(
        [[-1.000000, 0.000000, -0.790000], [0.000000, 1.000000, 0.283000], [0, 0, 1]])})
    T_vehicle_sonars.update({6: np.array(
        [[0.000000, 1.000000, -0.440000], [1.000000, 0.000000, 0.735000], [0, 0, 1]])})
    T_vehicle_sonars.update({7: np.array(
        [[0.000000, 1.000000, 2.100000], [1.000000, 0.000000, 0.760000], [0, 0, 1]])})


def lineno():
    return str(inspect.currentframe().f_back.f_lineno) + "  "


def load_LOG(filename):
    radar_velx = []
    lineno_cur = 0
    Lineno_find_obs_fusion = -1
    with open(filename) as myfile:
        matches = [re.search(LOG_PATTERN, line)
                   for line in myfile if re.search(LOG_PATTERN, line)]
        for one_match in matches:
            lineno_cur = lineno_cur + 1
            message = one_match.group(9)
            if re.search(" find obs in fusion_area", message, re.IGNORECASE):
                Lineno_find_obs_fusion = lineno_cur
                name, var = message.partition("angle[")[::2]
                angle_cmd_item = var.split("] ")[0]
                try:
                    angle_val = float(angle_item)
                except:
                    print("BAD angle: " + angle_item)
                name, var = message.partition("speed:")[::2]
                speed_item = var.split(" ")[0]
                try:
                    speed_val = float(speed_item)
                except:
                    print("BAD speed: " + speed_item)
            elif lineno_cur == (Lineno_find_obs_fusion + 1) and re.search("sensor type: RADAR", message, re.IGNORECASE):
                name, var = message.partition("angle:")[::2]
                angle_item = var.split(" ")[0]
                try:
                    angle_val = float(angle_item)
                except:
                    print("BAD angle: " + angle_item)
                name, var = message.partition("speed:")[::2]
                speed_item = var.split(" ")[0]
                try:
                    speed_val = float(speed_item)
                except:
                    print("BAD speed: " + speed_item)
            elif lineno_cur == (Lineno_find_obs_fusion + 3) and re.search("velocity_x:", message, re.IGNORECASE):
                Lineno_find_obs_fusion = -1
                name, var = message.partition("velocity_x:")[::2]
                velx_item = var.split(" ")[0]
                try:
                    velx_val = float(velx_item)
                    radar_velx.append(velx_val)
                except:
                    print("BAD velx: " + velx_item)

    print(radar_velx)

    return radar_velx

def main(filename):
    init()
    mpl.rcParams['legend.fontsize'] = 10

    try:
        radar_velx = load_LOG(filename)
    except IOError:
        print("Failed to load file [" + filename + "]. :(")
        sys.exit(-1)

    plt.title('Radar velocity Info', y=2.2)
    plt.show()

if __name__ == "__main__":
    if len(sys.argv) == 2:
        main(sys.argv[1])
    else:
        print(
            'Usage[2]:' + sys.argv[0] + ' gps_radar_timestamp.txt')
