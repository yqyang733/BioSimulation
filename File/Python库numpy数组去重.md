👏 Python库|numpy|数组去重

---
[TOC]

---
## np.unique()进行去重
(1) 参数说明：   
- array：输入数组，如果不是一维数组，则会被展开。   
- return_index：如果为True，返回新列表元素在旧列表中的位置（下标），并以列表形式储存。  
- return_inverse：如果为True，返回旧列表元素在新列表中的位置（下标），并以列表形式储存。 
- return_counts：如果为True，返回去重数组中元素在原数组中的出现次数。
(2) 示例：  
```python
>>> import numpy as np
>>> a = np.array([1,2,3,3,5,2,6,3,8,3,5])
>>> r = np.unique(a)
>>> r
array([1, 2, 3, 5, 6, 8])
>>> r, index = np.unique(a, return_index=True)
>>> r
array([1, 2, 3, 5, 6, 8])
>>> index
array([0, 1, 2, 4, 6, 8])   # 现列表元素在原列表中第一次出现时的索引
>>> r, index = np.unique(a, return_inverse=True)
>>> r
array([1, 2, 3, 5, 6, 8])
>>> index
array([0, 1, 2, 2, 3, 1, 4, 2, 5, 2, 3])  # 原列表元素在现列表中的索引  
>>> r, index = np.unique(a, return_counts=True)
>>> r
array([1, 2, 3, 5, 6, 8])
>>> index
array([1, 2, 4, 2, 1, 1])   # 现列表元素在原列表中出现的次数  
```