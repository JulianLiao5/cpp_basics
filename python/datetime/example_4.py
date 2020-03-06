
import inspect
import re


def lineno():
    """Returns the current line number in program."""
    return str(inspect.currentframe().f_back.f_lineno) + "  "
# ^                 start of string
# \S+               # match one or more non-space characters
# \S*               # match any non-whitespace character
# \s+
# \s*               # matches any whitespace character
# [-+]?             #
##
##
##
##


ACCESS_LOG_PATTERN = '^(\S+) (\S+) (\S+) \[([\w:/]+\s[+\-]\d{4})\] "(\S+) (\S+)\s*(\S+)\s*" (\d{3}) (\S+)'

logLine = '127.0.0.1 - - [01/Jul/1995:00:00:01 -0400] "GET /images/launch-logo.gif HTTP/1.0" 200 1839'

PLANNING_LOG_PATTERN = '^(\S+)\s*(\S+)\s*(\[0-9]+) (\S+):([0-9]+)\] (.+)'
log_example1 = 'Log line format: [IWEF]mmdd hh:mm:ss.uuuuuu threadid file:line] msg'
log_example2 = 'I0202 10:40:36.471122  2910 stereo_barrier_range_finder.cc:107] stereo msg: bounding info received'
log_example3 = 'I0202 10:41:37.726524  2916 collision_checker.cpp:518]  sensor type: VISION sensor id: 0 id: 67obstacle type: UNKNOWN'
planning_match = re.search(PLANNING_LOG_PATTERN, log_example1)

if planning_match:
    level_date = planning_match.group(1)
    time = planning_match.group(2)
    threadid = planning_match.group(3)
    source_file = planning_match.group(4)
    line_num = planning_match.group(5)
    message = planning_match.group(6)
    print(lineno() + "  " + level_date + "  " + time + "  threadid: " + threadid +
          "  source_file: " + source_file + "  line_num: " + line_num + "  message: " + message)
else:
      print(lineno() + "no match")

match = re.search(ACCESS_LOG_PATTERN, logLine)

if match:
    host = match.group(1)
    client_identd = match.group(2)
    user_id = match.group(3)
    date_time = match.group(4)
    method = match.group(5)
    endpoint = match.group(6)
    protocol = match.group(7)
    response_code = int(match.group(8))
    content_size = match.group(9)
    
    print(lineno() + "  " + host + "  " + client_identd + "  " + user_id + "  date_time: " + date_time + "  method: " + method + "  " + endpoint
          + "  " + protocol + "  " + str(response_code) + "  " + content_size)
else:
      print(lineno() + "no match")