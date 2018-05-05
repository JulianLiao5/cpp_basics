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
WEST = (270 * math.pi/ 180)
MAG_OFFSET = (0 * math.pi/ 180)

LINE_FIT = True
GPS_START = -0.1
MSCKF_START = -1.5
END_DISTANCE = -80.0    #unit: meter

MAG_LEN = 10

# X_VIO_T_NED = -0.02
# Y_VIO_T_NED = 0.165
OFFSET_X = 0.02
OFFSET_Y = 0.165

def load_trajectory_with_file(filename, need_transfer, init_th):
    m = np.loadtxt(filename, delimiter=" ")
    if need_transfer:
        x = [((vec[0] * math.cos(init_th)) + (vec[1] * math.sin(init_th + MAG_OFFSET)) - OFFSET_X) for vec in m]
        y = [((vec[1] * math.cos(init_th)) - (vec[0] * math.sin(init_th + MAG_OFFSET)) + OFFSET_Y) for vec in m]
    else:
        x = [vec[0] for vec in m]
        y = [vec[1] for vec in m]
    return x, y


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
            gps_heading_data = [vec[2] for vec in m]
            for k in range(0, MAG_LEN):
                gps_init_th = gps_init_th + gps_heading_data[k]
            gps_init_th = gps_init_th / MAG_LEN

        try:
            if i == 0:
                x, y = load_trajectory_with_file(name, False, gps_init_th)
                if LINE_FIT:
                    CNT = 0
                    for k in range(0, len(x)):
                        for m in range(k, k+10):
                            if x[m] < GPS_START:
                            	CNT = CNT + 1
                            else:
                            	CNT = 0
                        if CNT >= 10:        #10 mean 10 continue element, all of x must < -0.1
                        	gps_start = k
                        	break

                    for k in range(0, len(x)):
                    	if x[k] < END_DISTANCE:
                    		gps_end = k
                    		break
                    print "gps_start: " + str(gps_start) + ", gps_end: " + str(gps_end)

                    gps_X = x[gps_start:gps_end]
                    gps_Y = y[gps_start:gps_end]
                    gps0, gps1 = np.polyfit(gps_X, gps_Y, 1)
                    print "gps_y = {} gps_x + {}".format(gps0, gps1)

                gps_x_max = float("{0:.4f}".format(max(x)))
                gps_x_min = float("{0:.4f}".format(min(x)))
                gps_x_range = float("{0:.4f}".format(gps_x_max - gps_x_min))
                gps_y_max = float("{0:.4f}".format(max(y)))
                gps_y_min = float("{0:.4f}".format(min(y)))
                gps_y_range = float("{0:.4f}".format(gps_y_max - gps_y_min))
                print "gps x_max: " + str(gps_x_max) + ", x_min: " + str(gps_x_min) + "\n" + "gps y_max: " + str(gps_y_max) + ", y_min: " + str(gps_y_min)
            else:
                x, y = load_trajectory_with_file(name, True, gps_init_th)
                if LINE_FIT:
                    CNT = 0
                    for k in range(0, len(x)):
                        for m in range(k, k+10):
                            if x[m] < MSCKF_START:
                            	CNT = CNT + 1
                            else:
                            	CNT = 0
                        if CNT >= 10:        #10 mean 10 continue element, all of x must < -0.1
                        	msckf_start = k
                        	break

                    for k in range(0, len(x)):
                    	if x[k] < END_DISTANCE:
                    		msckf_end = k
                    		break
                    print "msckf_start: " + str(msckf_start) + ", msckf_end: " + str(msckf_end)

                    msckf_X = x[msckf_start:msckf_end]
                    msckf_Y = y[msckf_start:msckf_end]
                    msckf0, msckf1 = np.polyfit(msckf_X, msckf_Y, 1)
                    print "msckf_y = {} msckf_x + {}".format(msckf0, msckf1)
                    diff_angle = np.arctan((msckf0 - gps0) / (1 + gps0 * msckf0))
                    print "diff_angle: " + str(diff_angle)
                    gps_init_th = gps_init_th + diff_angle
                    x, y = load_trajectory_with_file(name, True, gps_init_th)

                vio_x_max = float("{0:.4f}".format(max(x)))
                vio_x_min = float("{0:.4f}".format(min(x)))
                vio_x_range = float("{0:.4f}".format(vio_x_max - vio_x_min))
                vio_y_max = float("{0:.4f}".format(max(y)))
                vio_y_min = float("{0:.4f}".format(min(y)))
                vio_y_range = float("{0:.4f}".format(vio_y_max - vio_y_min))
                print "vio x_max: " + str(vio_x_max) + ", x_min: " + str(vio_x_min) + "\n" + "vio y_max: " + str(vio_y_max) + ", y_min: " + str(vio_y_min)

        except IOError:
            print "Failed to load file [" + name + "]. :("
            sys.exit(-1)

        plt.figure(1)
        plt.plot(x, y, COLORS[i])
        print "plotting " + name
    plt.figure(1)
    plt.legend(filenames)
    plt.xlabel('x[m]')
    plt.ylabel('y[m]')
    font = {'family': 'serif',
        'color':  'darkred',
        'weight': 'normal',
        'size': 16,
        }
    plt.text(-75, 120, "Calibration Overall:" + "\n" + "    min: 0.0005 pixel" + "\n" + "    max: 5.5112 pixel" + "\n" + "    rms: 0.4759 pixel", fontdict=font)
    # plt.text(-1.8, 1.2, "Calibration Overall:" + "\n" + "    min: 0.000503 pixel" + "\n" + "    max: 5.511186 pixel" + "\n" + "    rms: 0.475902 pixel", fontdict=font)
    if LINE_FIT:
        font = {'family': 'serif',
            'color':  'darkorange',
            'weight': 'normal',
            'size': 14,
            }
        # "init heading_gps_kf:"
        # "init heading_mag:"
        plt.text(-75, 80, "init heading_gps_kf:" + "\n    " + "{0:.4f}".format(gps_init_th * RAD2DEG)
           + "deg\n" + "LineFit:\n    gps: [" + "{0:.4f}".format(GPS_START) + ", " + "{0:.4f}".format(END_DISTANCE)
           + "]\n    msckf: [" + "{0:.4f}".format(MSCKF_START) + ", " + "{0:.4f}".format(END_DISTANCE) + "]\ndiff_angle: "
           + "{0:.4f}".format(diff_angle * RAD2DEG), fontdict=font)
        # plt.text(0.9, -0.2, "init heading_mag:" + "\n    " + str(gps_init_th * RAD2DEG) + "deg\ncos: " + "\n    " + str(math.cos(WEST - gps_init_th - diff_angle)), fontdict=font)
    font = {'family': 'serif',
        'color':  'darkolivegreen',
        'weight': 'normal',
        'size': 12,
        }
    plt.text(-100, 40, "gps | x:[" + str(gps_x_min) + ", " + str(gps_x_max) + "  --  " + str(gps_x_range) + "], y:["
        + str(gps_y_min) + ", " + str(gps_y_max) + "  --  " + str(gps_y_range) + "]"
        + "\nvio | x:[" + str(vio_x_min) + ", " + str(vio_x_max) + "  --  " + str(vio_x_range) + "], y:["
        + str(vio_y_min) + ", " + str(vio_y_max) + "  --  " + str(vio_y_range) + "]", fontdict=font)
    plt.title('2D x-y trajectory')
    plt.gca().set_aspect('equal', adjustable='box')
    plt.show()


if __name__ == "__main__":
    if len(sys.argv) == 3:
        file_names = [sys.argv[1], sys.argv[2]]
        main(file_names)
    else:
        print('Usage[1]:' + sys.argv[0] + ' | for auto download file from adb')
        print(
            'Usage[2]:' + sys.argv[0] + ' [pose1.csv] [pose2.csv] |for given file')
