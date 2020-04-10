# cpp_basics

## 数据类型

平台
a. x86_64

类型 | 平台 | sizeof | 最小值 | 最大值 | 备注
-----|-----|------|-----|-----|-------
char  |  x86_64  |  1  |  -128  |  127  |  char 同 signed char
signed char  |  x86_64  |  1  |  -128  |  127  |  无
unsigned char  |  x86_64  |  1  |  0  |  255  |  无
short  |  x86_64  |  2  |  -32768  |  32767  |  同short int
short int  |  x86_64  |  2  |  -32768  |  32767  |  无
unsigned short  |  x86_64  |  2  |  0  |  65535  |  无
int  |  x86_64  |  4  |  -2^31  |  2^31 - 1  |  无
unsigned int  |  x86_64  |  4  |  0  |  2^32 - 1  |  无
long  |  x86_64  |  8  |  -2^63  |  2^63 - 1  |  无
unsigned long  |  x86_64  |  8  |  0  |  2^64 - 1  |  无
long long |  x86_64  |  8  |  -2^63  |  2^63 - 1  |  同long
unsigned long long |  x86_64  |  8  |  0  |  2^64 - 1  |  同unsigned long
uint8_t  |  x86_64  |  1  |  0  |  255  | 同unsigned char
int8_t  |  x86_64  |  1  |  -128  |  127  | 同signed char
uint16_t  |  x86_64  |  2  |  0  |  65535  | 同unsigned short
int16_t  |  x86_64  |  2  |  -32768  |  32767  |  同short
uint32_t  |  x86_64  |  4  |  0  |  2^32 - 1  |  同unsigned int
int32_t  |  未知  |  4  |  -2^31  |  2^31 - 1  |  同int
uint64_t  |  未知  |  8  |  0  |  2^64 - 1  |  同unsigned long
int64_t  |  未知  |  8  |  -2^63  |  2^63 - 1  |  同long
float  |  x86_64  |  4  |  未知  |  未知  |  无
double  |  x86_64  |  8  |  未知  |  未知  |  无
void *  |  x86_64  |  8  |  未知  |  未知  |  无

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
