import matplotlib.pyplot as plt
import inspect
from enum import Enum


def lineno():
    """Returns the current line number in program."""
    return str(inspect.currentframe().f_back.f_lineno) + "  "


Sensors = Enum('Sensors', 'Stixel DL Radar0 Radar1 Radar2 Radar3 Radar4 Radar5 Radar6 Radar7 Sonar0 Sonar1 Sonar2 Sonar3 Sonar4 Sonar5 Sonar6 Sonar7')
Colors = Enum(
    'Colors', 'k silver brown r slamon tan orange gold y g c skyblue b mediumpurple indigo m orchid pink')

area_list = ['Slowdown', 'Fusion', 'Secure', 'Out_lane_secure']
slowdown_obs = [{Sensors.Stixel: 100}, {Sensors.Radar2: 900},
                {Sensors.Sonar1: 50}, {Sensors.Sonar7: 33}]
fusion_obs = [{Sensors.Stixel: 109}, {Sensors.DL: 788}, {
    Sensors.Radar1: 5}, {Sensors.Radar3: 433}, {Sensors.Sonar2: 133}]
secure_obs = [{Sensors.DL: 1788}, {Sensors.Radar3: 51},
              {Sensors.Sonar1: 33}, {Sensors.Sonar3: 311}]
out_lane_secure_obs = [{Sensors.DL: 1788}, {Sensors.Radar3: 51}, {
    Sensors.Sonar1: 33}, {Sensors.Sonar3: 311}]
stixel = [162533, 65033, 0, 0]
x = list(range(len(area_list)))
print(lineno() + str(x))
total_width, n = 0.8, 4
width = total_width / n

# fc代表颜色，'y'表示yellow
plt.bar(x, slowdown_obs, width=width, label='Stixel', fc='y')

for i in range(len(x)):
    print(lineno() + "x[" + str(i) + "]: " + str(x[i]))
    x[i] = x[i] + width
    print(lineno() + "x[" + str(i) + "]: " + str(x[i]))

plt.bar(x, fusion_obs, width=width, label='DL', tick_label=area_list, fc='r')

for i in range(len(x)):
    print(lineno() + "x[" + str(i) + "]: " + str(x[i]))
    x[i] = x[i] + width
    print(lineno() + "x[" + str(i) + "]: " + str(x[i]))

plt.bar(x, secure_obs, width=width, label='Radar2',
        tick_label=area_list, fc='c')

for i in range(len(x)):
    print(lineno() + "x[" + str(i) + "]: " + str(x[i]))
    x[i] = x[i] + width
    print(lineno() + "x[" + str(i) + "]: " + str(x[i]))

plt.bar(x, out_lane_secure_obs, width=width, label='Sonar1',
        tick_label=area_list, fc='m')

plt.legend()
plt.gca().yaxis.grid(True)
plt.savefig('sensors.png')
plt.show()
