import inspect
import numpy as np
import matplotlib.pyplot as plt

'''
# sample data
x = np.arange(10)
y = 5*x + 10

# fit with np.polyfit
m, b = np.polyfit(x, y, 1)
print "m: " + str(m) + ", b: " + str(b)

plt.plot(x, y, '-')
plt.plot(x, m*x + b, '.')
plt.show()
'''


def lineno():
    return str(inspect.currentframe().f_back.f_lineno) + "  "


radar_ids = [0, 1, 2, 3, 4, 5, 6, 7]

# ObjInfo包含了所有雷达误报
# ObjInfo is an Hashmap
# key: radar_id, int
# value: list
ObjInfo = []

k = 0
cur_radarid = -1
cur_Range = 100.0
cur_Angle = 100.0

while k < 16:
    for i in radar_ids:
        if (k % 2) == 0:
            cur_radarid = 2
            cur_Range = 0.74 + (k / 2)
            cur_Angle = -45.3 + (k / 2)
        elif (k % 3) == 0:
            cur_radarid = 3
            cur_Range = 0.74 + (k/3)
            cur_Angle = -45.3 + (k/3)
        elif (k % 5) == 0:
            cur_radarid = 5
            cur_Range = 0.74 + (k/5)
            cur_Angle = -45.3 + (k/5)
        else:
            break
        if i == cur_radarid:
            if i in ObjInfo:
                tmp_objs = ObjInfo[i]
                tmp_objs.append({'Range': cur_Range, 'Angle': cur_Angle})
                ObjInfo.update({i: tmp_objs})
            else:
                tmp_objs = []
                tmp_objs.append({'Range': cur_Range, 'Angle': cur_Angle})
                if isinstance(ObjInfo, dict):
                    print(lineno() + "ObjInfo has already been a dist")
                    ObjInfo.update({i: tmp_objs})
                else:
                    print(lineno() + "ObjInfo init as a list")
                    ObjInfo = dict({i: tmp_objs})
            break

    k = k + 1

print(ObjInfo)

for key in ObjInfo.keys():
    Ranges = dict({key: [item['Range'] for item in ObjInfo[key]]})
    Angles = dict({key: [item['Angle'] for item in ObjInfo[key]]})
    print(lineno() + "key: " + str(key) + ", Ranges: " +
          str(Ranges[key]) + ", Angles: " + str(Angles[key]))
