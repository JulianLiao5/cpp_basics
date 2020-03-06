
import inspect


def lineno():
    """Returns the current line number in program."""
    return str(inspect.currentframe().f_back.f_lineno) + "  "


with open('test_data/test1.INFO', 'r') as f:
    for line in f:
        d = line.split(" ", 1)[0]
        print(lineno() + d)
        if '2012/09/30-00:00:10' <= d <= '2012/09/30-00:00:13':
            print(lineno() + line)

with open(test_data/PI_AUTO_Line_0_39079) as f:
    for line in f:
        if 

