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


## use a dist
## {'angle_cmd': float, 'speed_cmd': float, 'angle_status': float, 'speed_status': float}
def load_LOG(filename):
    # vehicle_data = {
    #   'angle_cmd': 1.1,
    #   'speed_cmd': 1.2,
    #   'angle_status': 2.2,
    #   'speed_status': 2.3
    # }
    vehicle_data = dict()
    lineno_cur = 0
    Lineno_planning_cmd = -1
    with open(filename) as myfile:
        matches = [re.search(LOG_PATTERN, line)
                   for line in myfile if re.search(LOG_PATTERN, line)]
        for one_match in matches:
            lineno_cur = lineno_cur + 1
            message = one_match.group(9)
            if re.search("command: angle", message, re.IGNORECASE):
                Lineno_planning_cmd = lineno_cur
                # angle[-3.64962] speed[1.5]
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
            elif lineno_cur == Lineno_planning_cmd and re.search("vehicle status: angle", message, re.IGNORECASE):
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

    return vs_angle, vs_speed


def load_time_Segment(seg_matches, whole_errs):
    global T_vehicle_radars
    global T_vehicle_sonars
    init()
    # total_Errs is a dictionary, key is Enum_Area, value is all vision/radar/sonar errors found under the area.
    # total_Errs = {
    #   Area.slowdown: Errs_slowdown,
    #   Area.fusion: Errs_fusion,
    #   Area.secure: Errs_secure,
    #   Area.out_lane_secure: Errs_out_lane_secure
    # }
    #
    if len(seg_matches) >= 2 and seg_matches[0]:
        start_time = seg_matches[0].group(2)
        message = seg_matches[0].group(6)
    total_Errs = dict()

    Lineno_find_obs = -1

    # last_Line and cur_Line are close to each other
    last_Line_type = Line_Type.unknown
    cur_Line_type = Line_Type.unknown
    cur_area = Area.unknown
    cur_type = Sensor_Type.UNKNOWN
    cur_id = -1

    # cur_vision_obs is a list, its element is {'vel_x': xx, 'vel_y': yy}
    cur_vision_obs = []

    for one_match in seg_matches:
        file_name = one_match.group(4)
        message = one_match.group(6)
        if re.search("find obs in ", message, re.IGNORECASE):
            Lineno_find_obs = lineno_cur
            cur_Line_type = Line_Type.find_obs
            # cur_Line(find_obs), last_Line(vehicle_coordinate)
            if Sensor_Type.VISION == cur_type and Line_Type.vehicle_coordinate == last_Line_type:
                if cur_area in total_Errs:
                    area_Errs = total_Errs[cur_area]
                    if cur_type in area_Errs:
                        type_Errs = area_Errs[cur_type]
                        if cur_id in type_Errs:
                            vision_list_list = type_Errs[cur_id]
                        else:
                            vision_list_list = []
                        vision_list_list.append(cur_vision_obs)
                        type_Errs.update({cur_id: vision_list_list})
                        area_Errs.update({cur_type: type_Errs})
                        total_Errs.update({cur_area: area_Errs})
                    else:
                        vision_list_list = []
                        vision_list_list.append(cur_vision_obs)
                        type_Errs = dict({cur_id: vision_list_list})
                        area_Errs.update({cur_type: type_Errs})
                        total_Errs.update({cur_area: area_Errs})
                else:
                    vision_list_list = []
                    vision_list_list.append(cur_vision_obs)
                    type_Errs = dict({cur_id: vision_list_list})
                    area_Errs = dict({cur_type: type_Errs})
                    total_Errs.update({cur_area: area_Errs})
            cur_vision_obs = []
            if re.search("find obs in slowdown_area", one_match.group(6), re.IGNORECASE):
                cur_area = Area.slowdown
            elif re.search("find obs in fusion_area", message, re.IGNORECASE):
                cur_area = Area.fusion
            elif re.search("find obs in secure_area", message, re.IGNORECASE):
                cur_area = Area.secure
            elif re.search("find obs in out_lane_secure_area", message, re.IGNORECASE):
                cur_area = Area.out_lane_secure
        e
            cur_Line_type = Line_Type.sensor_type
            last_Line_type = Line_Type.find_obs
            name, var = message.partition("sensor type: ")[::2]
            type_item = var.split(" ")[0]
            if Sensor_Type.VISION.name == type_item:
                cur_type = Sensor_Type.VISION
            elif Sensor_Type.RADAR.name == type_item:
                cur_type = Sensor_Type.RADAR
            elif Sensor_Type.ULTRASONIC.name == type_item:
                cur_type = Sensor_Type.ULTRASONIC
            name, var = message.partition("sensor id: ")[::2]
            id_item = var.split(" ")[0]
            try:
                cur_id = int(id_item)
            except:
                print(lineno() + "BAD id: " + str(id_item) +
                      ", cur_Line_no: " + str(lineno_cur))
            name, var = message.partition("obstacle type: ")[::2]
            obs_type_item = var.split(" ")[0]
            if Sensor_Type.VISION == cur_type:
                if "UNKNOWN" == obs_type_item:
                    cur_id = 0
                else:
                    cur_id = 1
            sensor_time = one_match.group(2)
        elif re.search("vehicle coordinate x: ", message, re.IGNORECASE):
            if Line_Type.sensor_type == cur_Line_type:
                last_Line_type = Line_Type.sensor_type
            elif Line_Type.vehicle_coordinate == cur_Line_type:
                last_Line_type = Line_Type.vehicle_coordinate
            cur_Line_type = Line_Type.vehicle_coordinate
            if (Sensor_Type.RADAR == cur_type or Sensor_Type.ULTRASONIC == cur_type) and cur_id >= 0 and cur_id <= 7:
                name, var = message.partition(" x: ")[::2]
                x_item = var.split(" ")[0]
                try:
                    x_val = float(x_item)
                except:
                    print(lineno() + "BAD x: " + x_item +
                          ", cur_Line_no: " + str(lineno_cur))
                name, var = message.partition(" y:")[::2]
                y_item = var.split(" ")[0]
                try:
                    y_val = float(y_item)
                except:
                    print(lineno() + "BAD y: " + y_item +
                          ", cur_Line_no: " + str(lineno_cur))
                pose_in_vel = np.array([x_val, y_val, 1])
                if Sensor_Type.RADAR == cur_type:
                    pose_in_radar = np.linalg.solve(
                        T_vehicle_radars[cur_id], pose_in_vel)
                    angle_in_rad = math.atan2(
                        -pose_in_radar[1], pose_in_radar[0])
                    angle_item = round(angle_in_rad * 180 / math.pi, 1)
                    radar_range_item = round(
                        (pose_in_radar[0] / math.cos(angle_in_rad)), 2)
                elif Sensor_Type.ULTRASONIC == cur_type:
                    pose_in_sonar = np.linalg.solve(
                        T_vehicle_sonars[cur_id], pose_in_vel)
                    sonar_range_item = round(pose_in_sonar[0], 2)
            elif Sensor_Type.VISION == cur_type and cur_id >= 0 and cur_id <= 1:
                name, var = message.partition(" x: ")[::2]
                x_item = var.split(" ")[0]
                try:
                    x_val = float(x_item)
                except:
                    print(lineno() + "BAD x: " + x_item +
                          ", cur_Line_no: " + str(lineno_cur))
                name, var = message.partition(" y:")[::2]
                y_item = var.split(" ")[0]
                try:
                    y_val = float(y_item)
                except:
                    print(lineno() + "BAD y: " + y_item +
                          ", cur_Line_no: " + str(lineno_cur))

            if cur_area in total_Errs:
                tmp_area_Errs = total_Errs[cur_area]
                if cur_type in tmp_area_Errs:
                    tmp_type_Errs = tmp_area_Errs[cur_type]
                    if cur_id in tmp_type_Errs:
                        tmp_id_Errs = tmp_type_Errs[cur_id]
                        if Sensor_Type.RADAR == cur_type:
                            tmp_id_Errs.append(
                                {'Timestamp': sensor_time, 'Range': radar_range_item, 'Angle': angle_item})
                        elif Sensor_Type.ULTRASONIC == cur_type:
                            tmp_id_Errs.append(
                                {'Timestamp': sensor_time, 'Range': sonar_range_item, 'Angle': 0.0})
                        elif Sensor_Type.VISION == cur_type:
                            cur_vision_obs.append(
                                {'Timestamp': sensor_time, 'vel_x': x_val, 'vel_y': y_val})
                            if len(seg_matches) == lineno_cur:
                                tmp_id_Errs.append(cur_vision_obs)
                                tmp_type_Errs.update({cur_id: tmp_id_Errs})
                                tmp_area_Errs.update({cur_type: tmp_type_Errs})
                                total_Errs.update({cur_area: tmp_area_Errs})
                                break
                            continue
                        tmp_type_Errs.update({cur_id: tmp_id_Errs})
                        tmp_area_Errs.update({cur_type: tmp_type_Errs})
                        total_Errs.update({cur_area: tmp_area_Errs})
                    else:
                        # print(lineno(
                        # ) + "newly Found [" + cur_area.name + "][" + cur_type.name + "]  --  id: " + str(cur_id))
                        tmp_id_Errs = []
                        if Sensor_Type.RADAR == cur_type:
                            tmp_id_Errs.append(
                                {'Timestamp': sensor_time, 'Range': radar_range_item, 'Angle': angle_item})
                        elif Sensor_Type.ULTRASONIC == cur_type:
                            tmp_id_Errs.append(
                                {'Timestamp': sensor_time, 'Range': sonar_range_item, 'Angle': 0.0})
                        elif Sensor_Type.VISION == cur_type:
                            cur_vision_obs.append(
                                {'Timestamp': sensor_time, 'vel_x': x_val, 'vel_y': y_val})
                            if len(seg_matches) == lineno_cur:
                                tmp_id_Errs.append(cur_vision_obs)
                                tmp_type_Errs.update({cur_id: tmp_id_Errs})
                                tmp_area_Errs.update({cur_type: tmp_type_Errs})
                                total_Errs.update({cur_area: tmp_area_Errs})
                                break
                            continue
                        tmp_type_Errs.update({cur_id: tmp_id_Errs})
                        tmp_area_Errs.update({cur_type: tmp_type_Errs})
                        total_Errs.update({cur_area: tmp_area_Errs})
                else:
                    # print(
                    #    lineno() + "newly Found [" + cur_area.name + "]  --  [" + cur_type.name + "]")
                    tmp_id_Errs = []
                    if Sensor_Type.RADAR == cur_type:
                        tmp_id_Errs.append(
                            {'Timestamp': sensor_time, 'Range': radar_range_item, 'Angle': angle_item})
                    elif Sensor_Type.ULTRASONIC == cur_type:
                        tmp_id_Errs.append(
                            {'Timestamp': sensor_time, 'Range': sonar_range_item, 'Angle': 0.0})
                    elif Sensor_Type.VISION == cur_type:
                        cur_vision_obs.append(
                            {'Timestamp': sensor_time, 'vel_x': x_val, 'vel_y': y_val})
                        if len(seg_matches) == lineno_cur:
                            tmp_id_Errs.append(cur_vision_obs)
                            tmp_type_Errs = dict({cur_id: tmp_id_Errs})
                            tmp_area_Errs.update({cur_type: tmp_type_Errs})
                            total_Errs.update({cur_area: tmp_area_Errs})
                            break
                        continue
                    tmp_type_Errs = dict({cur_id: tmp_id_Errs})
                    tmp_area_Errs.update({cur_type: tmp_type_Errs})
                    total_Errs.update({cur_area: tmp_area_Errs})
            else:
                # print(lineno() + "newly Found  --  " + cur_area.name +
                #      "  --  " + cur_type.name + "  --  " + str(cur_id))
                tmp_id_Errs = []
                if Sensor_Type.RADAR == cur_type:
                    tmp_id_Errs.append(
                        {'Timestamp': sensor_time, 'Range': radar_range_item, 'Angle': angle_item})
                elif Sensor_Type.ULTRASONIC == cur_type:
                    tmp_id_Errs.append(
                        {'Timestamp': sensor_time, 'Range': sonar_range_item, 'Angle': 0.0})
                elif Sensor_Type.VISION == cur_type:
                    cur_vision_obs.append(
                        {'Timestamp': sensor_time, 'vel_x': x_val, 'vel_y': y_val})
                    if len(seg_matches) == lineno_cur:
                        tmp_id_Errs.append(cur_vision_obs)
                        tmp_type_Errs = dict({cur_id: tmp_id_Errs})
                        tmp_area_Errs = dict({cur_type: tmp_type_Errs})
                        total_Errs.update({cur_area: tmp_area_Errs})
                        break
                    continue
                tmp_type_Errs = dict({cur_id: tmp_id_Errs})
                tmp_area_Errs = dict({cur_type: tmp_type_Errs})
                total_Errs.update({cur_area: tmp_area_Errs})
        elif Line_Type.vehicle_coordinate == cur_Line_type and 'planner.cpp' == file_name:
            if Sensor_Type.VISION == cur_type and Line_Type.vehicle_coordinate == last_Line_type:
                if cur_area in total_Errs:
                    area_Errs = total_Errs[cur_area]
                    if cur_type in area_Errs:
                        type_Errs = area_Errs[cur_type]
                        if cur_id in type_Errs:
                            vision_list_list = type_Errs[cur_id]
                        else:
                            vision_list_list = []
                        vision_list_list.append(cur_vision_obs)
                        type_Errs.update({cur_id: vision_list_list})
                    else:
                        vision_list_list = []
                        vision_list_list.append(cur_vision_obs)
                        type_Errs = dict({cur_id: vision_list_list})
                    area_Errs.update({cur_type: type_Errs})
                    total_Errs.update({cur_area: area_Errs})
                    cur_vision_obs = []
                else:
                    # print(lineno() + "newly Found  --  " + cur_area.name +
                    #      "  --  " + cur_type.name + "  --  " + str(cur_id))
                    vision_list_list = []
                    vision_list_list.append(cur_vision_obs)
                    tmp_type_Errs = dict({cur_id: vision_list_list})
                    tmp_area_Errs = dict({cur_type: tmp_type_Errs})
                    total_Errs.update({cur_area: tmp_area_Errs})
                    cur_vision_obs = []
            cur_Line_type = Line_Type.planner
            last_Line_type = Line_Type.vehicle_coordinate
            Lineno_find_obs = -1
            cur_area = Area.unknown
            cur_type = Sensor_Type.UNKNOWN
            cur_id = -1
            cur_vision_obs = []

    whole_errs.update({start_time: total_Errs})


def main(filename):
    init()
    mpl.rcParams['legend.fontsize'] = 10

    try:
        all_Errs, time_segs = load_LOG(filename)
        if len(time_segs) >= 1:
            first_time = list(time_segs.keys())[0]
            time_match = re.search(TIME_PATTERN, first_time)
            if time_match:
                first_hh = time_match.group(1)
                first_mm = time_match.group(2)
                first_ss = time_match.group(3)
                sensors_file = 'pi_sensors_' + first_hh + \
                    '_' + first_mm + '_' + first_ss + '.csv'
        else:
            sensors_file = 'pi_sensors.csv'
        with open(sensors_file, 'w') as sensors_f:
            sensors_f.write(
                "Start_time,Area,Timestamp,Sensor type,Sensor id,Range,Angle\n")
            for key_start in all_Errs.keys():
                if key_start in time_segs:
                    print("******************[" + key_start + "  --  " +
                          time_segs[key_start] + "]******************")
                for key_area in Area:
                    exist = False
                    for key_type in Sensor_Type:
                        for key_id in [0, 1, 2, 3, 4, 5, 6, 7]:
                            if (key_area in all_Errs[key_start]) and (key_type in all_Errs[key_start][key_area]) and (key_id in all_Errs[key_start][key_area][key_type]):
                                exist = True
                                print("[" + key_area.name + "][" + key_type.name + "][" + str(
                                    key_id) + "]: " + str(len(all_Errs[key_start][key_area][key_type][key_id])))
                                if Sensor_Type.RADAR == key_type or Sensor_Type.ULTRASONIC == key_type:
                                    for ra_so_item in all_Errs[key_start][key_area][key_type][key_id]:
                                        sensors_f.write('%s,%s,%s,%s,%d,%f,%f\n' % (
                                            key_start, key_area.name, ra_so_item['Timestamp'], key_type.name, key_id, ra_so_item['Range'], ra_so_item['Angle']))
                    if exist:
                        exist = False
                        print("++++++++")

    except IOError:
        print("Failed to load file [" + filename + "]. :(")
        sys.exit(-1)

    '''
    j = 0
    rows = 2
    if 2 == len(all_Errs):
        rows = 1
    columns = int(len(all_Errs) / rows)
    print(lineno() + ", rows: " + str(rows) + ", columns: " + str(columns))
    fig_radar = plt.figure("radar")
    font_blue = {'family': 'serif',
                 'color':  'blue',
                 'weight': 'normal',
                 'size': 16,
                 }
    font_green = {'family': 'serif',
                  'color':  'green',
                  'weight': 'normal',
                  'size': 16,
                  }
    outter = gridspec.GridSpec(rows, columns, wspace=0.3, hspace=0.3)
    j = 0
    for key in all_Errs.keys():
        inner = gridspec.GridSpecFromSubplotSpec(
            2, 1, subplot_spec=outter[j], wspace=0.2, hspace=0.2)
        j = j + 1
        Ranges = dict({key: [item['Range']
                             for item in all_Errs[key]]})
        Angles = dict({key: [item['Angle']
                             for item in all_Errs[key]]})

        for k in range(2):
            ax = plt.Subplot(fig_radar, inner[k])
            if 0 == k:
                ax.plot(Ranges[key], '-m')
                curve = ["Range"]
                ax.legend(curve, prop={'size': 10})
                ax.set_xlabel('x[data num]')
                ax.set_ylabel('y[m]')
                ax.text(0.5, 0.5, "Data_num: " + str(len(Ranges[key])) + "\nmin: " + "{0:.2f}".format(np.min(np.mean(Ranges[key]))) + ", mean: " + "{0:.2f}".format(np.mean(
                    Ranges[key])) + ", max: " + "{0:.2f}".format(np.max(Ranges[key])), horizontalalignment='center', verticalalignment='center', transform=ax.transAxes, fontdict=font_blue)
                ax.grid(True)
            elif 1 == k:
                ax.plot(Angles[key], '-k')
                curve = ["Angle"]
                ax.legend(curve, prop={'size': 10})
                ax.set_xlabel('x[data num]')
                ax.set_ylabel('y[deg]')
                ax.text(0.5, 0.8, "\nmin: " + "{0:.1f}".format(np.min(np.min(Angles[key]))) + ", mean: " + "{0:.1f}".format(np.mean(Angles[key])) + ", max: " + "{0:.1f}".format(
                    np.max(Angles[key])), horizontalalignment='center', verticalalignment='center', transform=ax.transAxes, fontdict=font_blue)
                ax.grid(True)
            fig_radar.add_subplot(ax)

        plt.title('Radar' + str(key) + '  Err Info', y=2.2)

    plt.show()
    '''


if __name__ == "__main__":
    if len(sys.argv) == 2:
        main(sys.argv[1])
    else:
        print(
            'Usage[2]:' + sys.argv[0] + ' gps_radar_timestamp.txt')
