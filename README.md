# cpp_basics
cpp examples for basic programming

1. awk 'BEGIN {FS=","} {print $1 "," $2 "," $3 "," $4 "," $6 "," $7}' gps_zte_0425_1845.txt > gps_zte_0425_1845.csv

    gps_zte_0425_1845.csv
        1_tow(ms),2_lat(degrees),3_lon(degrees),4_flags,5_heading_data(degrees),6_heading_mag(degrees)
                heading_data: [0, 2pi)
                heading_mag: [0, 2pi)

    final_gps_pose_2d_0426_1815.csv
        1_tow(ms),2_x(meters),3_y(meters),4_heading_data(radian),5_heading_mag(radian),6_flags
                heading_data: [0, 2pi)
                heading_mag: [0, 2pi)
