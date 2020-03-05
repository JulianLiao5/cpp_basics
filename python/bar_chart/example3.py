import numpy
from matplotlib import pyplot

import inspect


def lineno():
    """Returns the current line number in program."""
    return str(inspect.currentframe().f_back.f_lineno) + "  "


c_labels = ['1', '2', '3', '4', '5', '6', '7', '8', 'Unknown']
# 9 colors
colors = ['royalblue', 'red', 'orange', 'green', 'purple',
          'deepskyblue', 'deeppink', 'limegreen', 'firebrick']
x_labels = ['1)', '2)', '3)', '4)', '5)']
# 5rows, 9columns
sizes = numpy.array([
    [2, 8, 2, 1, 0, 0, 0, 0, 1],
    [2, 4, 6, 0, 0, 0, 1, 0, 1],
    [2, 0, 0, 2, 5, 0, 0, 1, 3],
    [1, 0, 0, 3, 2, 2, 4, 0, 2],
    [1, 0, 1, 0, 1, 1, 4, 3, 2],
])


print(lineno() + str(sizes.shape[0]))

fig, axes = pyplot.subplots(ncols=sizes.shape[0], figsize=(10, 5), sharey=True)

for ax, height, title in zip(axes, sizes, x_labels):
    ax.set_title(title)
    left = numpy.arange(len(height)) + 1
    ax.bar(left, height, color=colors)
    ax.set_xticks(left)
    ax.set_xticklabels(c_labels, rotation=45,
                       rotation_mode='anchor', ha='right')
    ax.yaxis.grid(True)

pyplot.show()
