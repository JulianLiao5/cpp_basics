import datetime as dt
import matplotlib.pyplot as plt
import numpy as np


# 手动创建我们想要画的时间。列表里面的时间与前一个柱子的结尾是相对应的
# 列表里面的第一项将会是x轴上出现的第一个时间
times = [
    dt.time(10, 40, 36, 390728),
    dt.time(10, 50, 36, 396122),
    dt.time(11, 00, 36, 390144),
    dt.time(11, 10, 36, 392283),
    dt.time(11, 20, 36, 399591),
    dt.time(11, 30, 36, 397396),
    dt.time(11, 40, 36, 391353)
]

# convert times into the number of seconds elapsed.
seconds = [(x.hour * 3600 + x.minute * 60 + x.second) for x in times]

# manually add the y-axis values
values = [13.5, 25.5, 50.5, 55.5, 20.5, 38.5, 5.5]

start_time = (times[0].hour * 3600 + times[0].minute * 60 + times[0].second)
# xticks一共有48个值
# 1800seconds = 30min
# 48个30min就是24小时
# 60 × 60 × 24 = 86400,一天是86400秒
xticks = [x + start_time for x in range(60 * 60 * 24) if x % 600 == 0]

# 为了显示时间，而不是从00:00过去的秒数，我们需要调用dt.timedelta()来把一个int型数转换成一个dt.time数据类型
# 如果start_time是一个大于0的数，那么xticks当中就至少有一个数其值超过86400，对于这些超过86400的数，我们必须减去86400，
# 否则的话，xticklabels就会出现类似于"one day, 0:30"


def label_format(seconds):
    if seconds >= 86400:
        seconds -= 86400

    return str(dt.timedelta(seconds=seconds))


xticklabels = [label_format(x) for x in xticks]

# create a histogram style chart with unequal bar widths, we need a way to specify the bar width.
# bar_traits()能够给出一个柱子(bar)的起点和宽度基于该柱子(bar)在x轴上的起点和下一个柱子的起点。


def bar_traits(ix, second, seconds, start_time):
    if ix < len(seconds) - 1:
        if seconds[ix+1] < second:
            width = 86400 + seconds[ix+1] - second
        else:
            width = seconds[ix+1] - second
    else:
        if start_time < second:
            width = 86400 + start_time - second
        else:
            width = start_time - second
    if start_time > second:
        second += 86400

    return second, width


fig, ax = plt.subplots(figsize=(16, 8))

for ix, (second, value) in enumerate(zip(seconds, values)):
    second, width = bar_traits(ix, second, seconds, start_time)
    ax.bar(second, value, width=width, align='edge', color='C0')

ax.set_xticks(xticks)
ax.set_xticklabels(xticklabels, rotation=90)

plt.show()
