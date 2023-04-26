👏 Python库|time|日期时间相关函数详解

---
[TOC]

---
## 计算程序运行时间
```python
import time

t_start = time.time()   # 获取当前的时间
…… # 程序主体
t_end = time.time()
print("All need " + str(round((t_end-t_start)/60,2)) + " 分钟!")
```