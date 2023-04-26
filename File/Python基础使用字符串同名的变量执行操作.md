👏 Python基础|使用字符串同名的变量执行操作

---
[TOC]

---
## 实用locals()[]将字符串指向同名的变量
(1) 有多个规律的变量，利用迭代的方式对这些规律变量执行操作。  
```python
import numpy as np

num_6 = np.array(num_6, dtype=np.str_)
num_5 = np.array(num_5, dtype=np.str_)
num_4 = np.array(num_4, dtype=np.str_)
num_3 = np.array(num_3, dtype=np.str_)
num_2 = np.array(num_2, dtype=np.str_)
num_1 = np.array(num_1, dtype=np.str_)
rt = open("cutoff_num.csv","w")
for i in range(1, 7):
    com_num, TCM_num, Target_num = num_info_cutoff(locals()["num_%d"%(i)])
```