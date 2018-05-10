#!/usr/bin/env python
import matplotlib.pyplot as plt
import matplotlib as mpl
import numpy as np
import sys
import os
import subprocess
import math
from matplotlib import collections as mc
from matplotlib.colors import ListedColormap


GPS_FIX = 4
GPS_FLOAT = 3

COLOR_FIX = '-b'
COLOR_FLOAT = '-r'

GPS_COLORS = [COLOR_FIX, COLOR_FLOAT]
COLORS = ['-y', '-g', '-k', '-m', '-c']
RAD2DEG = 180 / math.pi
MAG_OFFSET = (0 * math.pi/ 180)

LINE_FIT = True
GPS_START = -0.1
MSCKF_START = -3.5
END_DISTANCE = -80.0    #unit: meter

MAG_LEN = 10

OFFSET_X = 0.02
OFFSET_Y = 0.165

FUSION = True

'''
def load_gps_trajectory_with_file(filename):
    m = np.loadtxt(filename, delimiter=" ")
    x_gps_fix = [vec[1] for vec in m if vec[4] == GPS_FIX]
    y_gps_fix = [vec[2] for vec in m if vec[4] == GPS_FIX]
    x_gps_float = [vec[1] for vec in m if vec[4] == GPS_FLOAT]
    y_gps_float = [vec[2] for vec in m if vec[4] == GPS_FLOAT]
    return x_gps_fix, y_gps_fix, x_gps_float, y_gps_float
'''

def load_gps_trajectory_with_file(filename):
    m = np.loadtxt(filename, delimiter=" ")
    x = [vec[1] for vec in m]
    y = [vec[2] for vec in m]
    flag = [vec[4] for vec in m]
    return x, y, flag

def load_vio_trajectory_with_file(filename, need_transfer, init_th):
    m = np.loadtxt(filename, delimiter=" ")
    if need_transfer:
        x = [((vec[1] * math.cos(init_th)) + (vec[2] * math.sin(init_th + MAG_OFFSET)) - OFFSET_X) for vec in m]
        y = [((vec[2] * math.cos(init_th)) - (vec[1] * math.sin(init_th + MAG_OFFSET)) + OFFSET_Y) for vec in m]
    else:
        x = [vec[1] for vec in m]
        y = [vec[2] for vec in m]
    return x, y


def main(filenames):
    assert isinstance(filenames, list)
    assert len(filenames) > 0
    assert len(filenames) <= len(COLORS)

    mpl.rcParams['legend.fontsize'] = 10
    gps_init_th = 0
    gps_data = []
    fusion_x = []
    fusion_y = []
    colors = []
    aa = 0
    for i in range(0, len(filenames)):
        name = filenames[i]
        if i == 0:
            m = np.loadtxt(name, delimiter=" ")
            gps_heading_data = [vec[3] for vec in m]
            for k in range(0, MAG_LEN):
                gps_init_th = gps_init_th + gps_heading_data[k]
            gps_init_th = gps_init_th / MAG_LEN
            print "gps_init_th: " + str(gps_init_th)

        try:
            if i == 0:
                # x_gps_fix, y_gps_fix, x_gps_float, y_gps_float = load_gps_trajectory_with_file(name)
                gps_x, gps_y, flag = load_gps_trajectory_with_file(name)
                if LINE_FIT:
                    CNT = 0
                    for k in range(0, len(gps_x)):
                        for m in range(k, k+10):
                            if gps_x[m] < GPS_START:
                            	CNT = CNT + 1
                            else:
                            	CNT = 0
                        if CNT >= 10:        #10 mean 10 continue element, all of x must < -0.1
                        	gps_start = k
                        	break

                    for k in range(0, len(gps_x)):
                    	if gps_x[k] < END_DISTANCE:
                    		gps_end = k
                    		break
                    print "gps_start: " + str(gps_start) + ", gps_end: " + str(gps_end)

                    gps_X = gps_x[gps_start:gps_end]
                    gps_Y = gps_y[gps_start:gps_end]
                    gps0, gps1 = np.polyfit(gps_X, gps_Y, 1)
                    print "gps_y = {} gps_x + {}".format(gps0, gps1)

                gps_x_max = float("{0:.4f}".format(max(gps_x)))
                gps_x_min = float("{0:.4f}".format(min(gps_x)))
                gps_x_range = float("{0:.4f}".format(gps_x_max - gps_x_min))
                gps_y_max = float("{0:.4f}".format(max(gps_y)))
                gps_y_min = float("{0:.4f}".format(min(gps_y)))
                gps_y_range = float("{0:.4f}".format(gps_y_max - gps_y_min))
                print "gps x_max: " + str(gps_x_max) + ", x_min: " + str(gps_x_min) + "\n" + "gps y_max: " + str(gps_y_max) + ", y_min: " + str(gps_y_min)
            else:
                vio_x, vio_y = load_vio_trajectory_with_file(name, True, gps_init_th)
                if LINE_FIT:
                    CNT = 0
                    for k in range(0, len(vio_x)):
                        for m in range(k, k+10):
                            if vio_x[m] < MSCKF_START:
                            	CNT = CNT + 1
                            else:
                            	CNT = 0
                        if CNT >= 10:        #10 mean 10 continue element, all of x must < -0.1
                        	msckf_start = k
                        	break

                    for k in range(0, len(vio_x)):
                    	if vio_x[k] < END_DISTANCE:
                    		msckf_end = k
                    		break
                    print "msckf_start: " + str(msckf_start) + ", msckf_end: " + str(msckf_end)

                    msckf_X = vio_x[msckf_start:msckf_end]
                    msckf_Y = vio_y[msckf_start:msckf_end]
                    msckf0, msckf1 = np.polyfit(msckf_X, msckf_Y, 1)
                    print "msckf_y = {} msckf_x + {}".format(msckf0, msckf1)
                    diff_angle = np.arctan((msckf0 - gps0) / (1 + gps0 * msckf0))
                    print "diff_angle: " + str(diff_angle)
                    gps_init_th = gps_init_th + diff_angle
                    vio_x, vio_y = load_vio_trajectory_with_file(name, True, gps_init_th)

                if FUSION:
                    with open('ned_msckf.txt', 'w+') as ned_msckf:
                        np.savetxt(ned_msckf, zip(vio_x, vio_y), fmt="%f %f")
                vio_x_max = float("{0:.4f}".format(max(vio_x)))
                vio_x_min = float("{0:.4f}".format(min(vio_x)))
                vio_x_range = float("{0:.4f}".format(vio_x_max - vio_x_min))
                vio_y_max = float("{0:.4f}".format(max(vio_y)))
                vio_y_min = float("{0:.4f}".format(min(vio_y)))
                vio_y_range = float("{0:.4f}".format(vio_y_max - vio_y_min))
                print "vio x_max: " + str(vio_x_max) + ", x_min: " + str(vio_x_min) + "\n" + "vio y_max: " + str(vio_y_max) + ", y_min: " + str(vio_y_min)

        except IOError:
            print "Failed to load file [" + name + "]. :("
            sys.exit(-1)

        plt.figure(1)
        if i == 0:
            for x1, x2, y1, y2, flag0 in zip(gps_x, gps_x[1:], gps_y, gps_y[1:], flag):
                if flag0 == GPS_FIX:
                    colors.append(tuple([0,0,1,1]))
                else:
                    colors.append(tuple([1,0,0,1]))
                gps_data.append([(x1, y1), (x2, y2)])
                aa += 1

            # gps_fix = np.array([x_gps_fix, y_gps_fix])
            # gps_float = np.array([x_gps_float, y_gps_float])
            # gps_fix = np.array([x_gps_fix, y_gps_fix])
            # gps_float = np.array([x_gps_float, y_gps_float])
            # gps_data = [gps_fix, gps_float]
            # gps_data = np.concatenate([gps_fix, gps_float], axis=1)
            # cmap = ListedColormap(['b', 'r'])
            # gps_data = [(x_gps_fix, y_gps_fix), (x_gps_float, y_gps_float)]
            # colors = [mcolors.to_rgba(c) for c in plt.rcParams['axes.prop_cycle'].by_key()['color']]
            lc = mc.LineCollection(gps_data, colors=colors, linewidths=2)
            plt.gca().add_collection(lc)
        else:
            plt.plot(vio_x, vio_y, '-g')

        print "plotting " + name

    if FUSION:
        assert len(vio_x) == len(gps_x)
        aa = 0
        # The fact is whether you change vio_x/vio_y or not, it will affact the final fusion result.
        delta_x = 0
        delta_y = 0
        last_state = GPS_FIX
        for aa in range(0, len(gps_x)):
            x1 = gps_x[aa]
            y1 = gps_y[aa]
            x2 = vio_x[aa]
            y2 = vio_y[aa]
            flag_gps = flag[aa]
            if flag_gps == GPS_FIX:
                fusion_x.append(x1)
                fusion_y.append(y1)
                last_x_gps_fix = x1
                last_y_gps_fix = y1
                last_x_vio = x2
                last_y_vio = y2
                last_state = GPS_FIX
            elif flag_gps == GPS_FLOAT:
                delta_x = last_x_vio - last_x_gps_fix
                delta_y = last_y_vio - last_y_gps_fix
                if last_state == GPS_FIX:
                    for bb in range(aa, len(vio_x)):
                        vio_x[bb] -= delta_x
                        vio_y[bb] -= delta_y
                    with open('ned_msckf_{}.txt'.format(aa), 'w+') as ned_msckf:
                        np.savetxt(ned_msckf, zip(vio_x, vio_y), fmt="%f %f")
                fusion_x.append(vio_x[aa])
                fusion_y.append(vio_y[aa])
                last_state = GPS_FLOAT
            aa += 1
        with open('ned_msckf_final.txt', 'w+') as ned_msckf:
            np.savetxt(ned_msckf, zip(vio_x, vio_y), fmt="%f %f")
        with open('fusion_data.txt', 'w+') as f_fusion:
            np.savetxt(f_fusion, zip(fusion_x, fusion_y), fmt="%f %f")
        '''
        for x1, y1, x2, y2, flag_gps in zip(gps_x, gps_y, vio_x, vio_y, flag):
            if flag_gps == GPS_FIX:
                fusion_x.append(x1)
                fusion_y.append(y1)
                last_x_gps_fix = x1
                last_y_gps_fix = y1
                last_x_vio = x2
                last_y_vio = y2
                last_state = GPS_FIX
            else:
                delta_x = last_x_vio - last_x_gps_fix
                delta_y = last_y_vio - last_y_gps_fix
                # vio_x = [(x_item - delta_x) for x_item in vio_x]
                # vio_y = [(y_item - delta_y) for y_item in vio_y]
                fusion_x.append(x2 - delta_x)
                fusion_y.append(y2 - delta_y)
                if last_state == GPS_FIX :
                    for k in range(aa, len(vio_x)):
                        vio_x[k] -= delta_x
                        vio_y[k] -= delta_y
                last_state = GPS_FLOAT
            # vio_x = [(x_item - delta_x) for x_item in vio_x]
            # vio_y = [(y_item - delta_y) for y_item in vio_y]
            aa += 1

        with open('fusion_data.txt', 'w+') as f_fusion:
            np.savetxt(f_fusion, zip(fusion_x, fusion_y), fmt="%f %f")
'''
        plt.plot(fusion_x, fusion_y, '-k')

    plt.figure(1)
    curves = filenames
    if FUSION:
        curves.extend(["FUSION"])
    plt.legend(curves, prop={'size':10})
    ax = plt.gca()
    leg = ax.get_legend()
    leg.legendHandles[0].set_color('magenta')
    leg.legendHandles[1].set_color('green')
    if FUSION:
        leg.legendHandles[2].set_color('black')
    plt.xlabel('x[m]', size = 10)
    plt.ylabel('y[m]', size = 10)
    plt.xticks(size = 10)
    plt.xticks(size = 10)
    plt.grid(True)
    '''
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
    font = {'family': 'serif',
        'color':  'darkolivegreen',
        'weight': 'normal',
        'size': 12,
        }
    plt.text(-100, 40, "gps | x:[" + str(gps_x_min) + ", " + str(gps_x_max) + "  --  " + str(gps_x_range) + "], y:["
        + str(gps_y_min) + ", " + str(gps_y_max) + "  --  " + str(gps_y_range) + "]"
        + "\nvio | x:[" + str(vio_x_min) + ", " + str(vio_x_max) + "  --  " + str(vio_x_range) + "], y:["
        + str(vio_y_min) + ", " + str(vio_y_max) + "  --  " + str(vio_y_range) + "]", fontdict=font)
    '''
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

