
gps_zte_0428_1730.csv
    第一步：删掉gps_zte_0428_1730.txt前面50个数据
    第二步：awk 'BEGIN {FS=","} {print $1 " " $2 " " $3 " " $4 " " $6 " " $7}' gps_zte_0428_1730.txt > gps_zte_0428_1730.csv
CameraTrajectory_0428_1730.csv
    第一步：./msckf_vio ../config/4008.yaml /home/julian/PerceptIn/VIO_GPS/dataset/zte_0428_1730
    第二步：删掉前面10个数据
