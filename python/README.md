


## 画图

### 柱状图(bar chart)

好的介绍文章链接

https://stackoverflow.com/questions/42725409/skip-zero-values-in-barchart-matplotlib

## 常用的function

1. 打印lineno

import inspect

def lineno():
    """Returns the current line number in program."""
    return str(inspect.currentframe().f_back.f_lineno) + "  "


test_dict.py:

主要测试dictionary赋值方法，update函数等的使用。