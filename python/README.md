


## 画图

### 柱状图(bar chart)

好的介绍文章链接

https://stackoverflow.com/questions/42725409/skip-zero-values-in-barchart-matplotlib

## Regex

模式(pattern) | 含义_英文 | 含义_中文 | 备注
-----|-----|------|-----
  |    |    |    |  2019-04-08（周一）  |  未知  |  无
  |    |    |    |  2018-11-05（周一）  |  未知  |  无

## 常用的function

1. 打印lineno

import inspect

def lineno():
    """Returns the current line number in program."""
    return str(inspect.currentframe().f_back.f_lineno) + "  "

## 常用的第三方库

1. glog

https://pypi.org/project/glog/

glog 0.3.1
```
pip install glog
```


test_dict.py:

主要测试dictionary赋值方法，update函数等的使用。