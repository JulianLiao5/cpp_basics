


## 画图

### 柱状图(bar chart)

好的介绍文章链接

https://stackoverflow.com/questions/42725409/skip-zero-values-in-barchart-matplotlib

## Python regex

模式(pattern) | 含义_英文 | 含义_中文 | 备注
-----|-----|------|-----
^  |  start of string  |  字符串的开头  |  无
$  |  end of string  |  字符串的结尾  |  无
.  |  dot matches any character except newline  |  匹配除newline外任意单个字符  |  无
\+  |  match one or more of the preceding character  |  匹配一个或多个前面的字符  |  无
\*  |  match zero or more of the preceding character  |  匹配零个或多个前面的字符  |  无
.+  |  match one or more characters until newline  |  匹配直到碰到newline的字符串  |  无
\S  |  match single non-whitespace character  |  匹配单个非空白字符  |  无
\S+  |  match one or more non-whitespace charcaters |  匹配一个或多个非空白字符串  |  无
\S*  |  match any non-whitespace character  |  匹配零个或多个非空白字符串  |  无
\s  |  match single whitespace character  |  匹配任意单个空白字符  |  空白字符包括1. \n 2. \t 3. space
\s+  |  match one or more non-whitespace charcaters |  匹配一个或多个空白字符串  |  无
\s*  |  match any non-whitespace character  |  匹配零个或多个空白字符串  |  无
\d  |  match a single digit  |  匹配单个数字  |  无
\d{4}  |  match a four-number digit  |  匹配4个数字  |  无
\D  |  match a single character that is not a digit  |  匹配非数字单个字符  |  无
[abc]  |  match single character in the set, i.e either match a, b or c  |  匹配a或者b或者c  |  无
[^abc]  |  match a single character other than a, b and c  |  匹配不是a/b/c的单个字符  |  无
[a-z]  |  match a single character in the range a to z  |  匹配a到z之间单个小写字母  |  无
[a-zA-Z]  |  match a single character in the range a-z/A-Z  |  匹配a到z或者A到Z之间单个字母  |  无
[0-9]  |  match a single character in the range 0-9  |  匹配0到9之间单个字符  |  [0-9]同\d
[-+]?  |  ? means an option  |  ?意味着可选，即加号或者减号  |  无

极好的文章列表：

https://thepythonguru.com/python-regular-expression/

https://stackoverflow.com/questions/12660164/the-best-way-to-filter-a-log-by-a-dates-range-in-python

https://stackoverflow.com/questions/55867124/parse-log-between-datetime-range-using-python

https://stackoverflow.com/questions/30956820/log-parsing-with-regex

https://regex101.com/


## 常用的function

1. 打印lineno

```
import inspect

def lineno():
    """Returns the current line number in program."""
    return str(inspect.currentframe().f_back.f_lineno) + "  "
```

2. 写csv文件

2.1 方案1

```
sensors_file = 'pi_sensors.csv'
with open(sensors_file, 'w') as sensors_f:
    sensors_f.write("Start_time,Area,Timestamp,Sensor type,Sensor id,Range,Angle\n")
    sensors_f.write('%s,%s,%s,%s,%d,%f,%f\n' % ('10:40:36.390728', 'slowdown', '10:44:58.054139', 'RADAR', 0, 19.86, 5.8))
```

## 常用的第三方库

1. glog

https://pypi.org/project/glog/

glog 0.3.1
```
pip install glog
```


test_dict.py:

主要测试dictionary赋值方法，update函数等的使用。