# cpp_basics

## 数据类型

平台
a. x86_64

类型 | 平台 | sizeof | 最小值 | 最大值 | 离职时间 | 在职时间 | 离职原因 | 备注
-----|-----|------|-----|-----|------|------|-------|-------
int  |  x86_64  |  4  |  技术  |  未知  |  2017-10  |  未知  |  未知  |  无
unsigned int  |  x86_64  |  4  |  技术  |  未知  |  2018-11  |  未知  |  开会被老板评判狠了，心里委屈没憋住  |  无
short  |  x86_64  |  2  |  技术  |  未知  |  2018-11  |  未知  |  开会被老板评判狠了，心里委屈没憋住  |  无
short int  |  x86_64  |  2  |  技术  |  未知  |  未知  |  未知  |  找到更好工作  |  无
uint8_t  |  x86_64  |  1  |  技术  |  未知  |  未知  |  未知  |  部门整体裁掉  |  无
int8_t  |  x86_64  |  1  |  技术  |  未知  |  未知  |  未知  |  人身攻击文博  |  无
uint16_t  |  x86_64  |  2  |  技术  |  未知  |  未知  |  未知  |  被开  |  无
int16_t  |  x86_64  |  2  |  技术  |  未知  |  2019-10  |  未知  |  未知  |  无
uint32_t  |  x86_64  |  4  |  商务  |  未知  |  2019-09  |  未知  |  未知  |  无
int32_t  |  未知  |  4  |  商务  |  未知  |  2019-09  |  未知  |  未知  |  无
uint64_t  |  未知  |  8  |  美工  |  未知  |  2019-09  |  未知  |  未知  |  
int64_t  |  未知  |  8  |  美工  |  未知  |  2019-09  |  未知  |  未知  |  

## Linux常用命令

### awk

1. awk 'BEGIN {FS=","} {print $1 "," $2 "," $3 "," $4 "," $6 "," $7}' gps_zte_0425_1845.txt > gps_zte_0425_1845.csv

    gps_zte_0425_1845.csv
        1_tow(ms),2_lat(degrees),3_lon(degrees),4_flags,5_heading_data(degrees),6_heading_mag(degrees)
                heading_data: [0, 2pi)
                heading_mag: [0, 2pi)

    final_gps_pose_2d_0426_1815.csv
        1_tow(ms),2_x(meters),3_y(meters),4_heading_data(radian),5_heading_mag(radian),6_flags
                heading_data: [0, 2pi)
                heading_mag: [0, 2pi)
