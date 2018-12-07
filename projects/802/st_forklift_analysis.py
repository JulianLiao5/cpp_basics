#!/usr/bin/env python
# coding=utf-8

import matplotlib.pyplot as plt
import numpy as np
import xlrd

#case_6_-40deg_-0.5_2
# workbook = xlrd.open_workbook('case_6_-40deg_-0.5.xls')
# workbook = xlrd.open_workbook('case_1_forward_10m.xls')
# workbook = xlrd.open_workbook('case_2_backward_10m.xls')
# workbook = xlrd.open_workbook('case_3_40deg_0.5.xls')
# workbook = xlrd.open_workbook('case_4_-40deg_0.5.xls')
workbook = xlrd.open_workbook('case_5_40deg_-0.5.xls')
#workbook = xlrd.open_workbook('case_6_-40deg_-0.5_3.xls')
worksheet = workbook.sheet_by_index(0)

dist_use = []
speed_use = []
angle_use = []

dist = worksheet.col_values(2, 1);
speed = worksheet.col_values(3, 1);
angle = worksheet.col_values(4, 1);
num_rows = worksheet.nrows - 1
print("num_rows: " + str(num_rows))
curr_row = 0
while curr_row < num_rows:
        curr_row += 1
        dist_use.append(worksheet.cell_value(curr_row, 2))
        speed_use.append(worksheet.cell_value(curr_row, 3))
        angle_use.append(worksheet.cell_value(curr_row, 4))

speed_2 = speed_use[1000:4000]

print("len_dist_use: " + str(len(dist_use)))
total_dist = dist_use[len(dist_use) - 1] - dist_use[0]
print("dist_use_0: " + str(dist_use[0]) + ", dist_use(len(dist_use) - 1): " + str(dist[len(dist_use) - 1]) + ", total_dist: " + str(total_dist))
dist_figure = plt.figure("dist")
plt.plot(dist_use)
font = {'family': 'serif',
    'color':  'darkred',
    'weight': 'normal',
    'size': 12,
    }
#plt.text(250, 510, "Ground truth: suppose 10.00m\n    measurement: " + "{:.4f}".format(total_dist / 1000.0) + "m error: " + "{:.4%}".format((total_dist / 1000.0 - (-10.00)) / -10.00), fontdict=font)
plt.text(1000, 0, "measurement: " + "{:.4f}".format(total_dist / 1000.0) + "m ", fontdict=font)
plt.grid(True)
speed_figure = plt.figure("speed")
plt.plot(speed_use)
plt.text(1000, -2.0, "Ground truth: suppose -0.5m/s\n    Index_range: [1000, 4000]\n    measurement: " + "{:.4f}".format(np.mean(speed_2)) + "m error: " + "{:.4%}".format((np.mean(speed_2) - (-0.5)) / (-0.5)), fontdict=font)
plt.grid(True)
angle_figure = plt.figure("angle")
plt.title("turn_Encoder")
plt.plot(angle_use)
plt.grid(True)
plt.show()
