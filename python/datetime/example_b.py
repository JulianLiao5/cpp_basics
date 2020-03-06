from datetime import datetime
import time

import inspect


def lineno():
    """Returns the current line number in program."""
    return str(inspect.currentframe().f_back.f_lineno) + "  "


print(datetime.now().strftime("%d_%m_%Y"))

print(lineno() + str(datetime.now()))

time_obj = time.localtime()
print(lineno() + str(time_obj))

t = time.asctime(time_obj)
print(lineno() + str(t))

# test_secs = 950000000.81956
test_secs = 1568802902.264
obj = time.localtime(test_secs)

print(lineno() + str(obj))

line = "I0202 10:40:36.390728  2902 can_transmitter.cpp:94] Set receive thread to SCHED_RR failed."
