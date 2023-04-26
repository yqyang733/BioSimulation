👏 Python库|numpy|数据类型及类型转换

---
[TOC]

---
## list与array之间转换
(1) 字符串list转换为浮点数数组。  
```python
>>> import numpy as np
>>> cha = "126936488471270"
>>> lst = list(cha)
>>> lst
['1', '2', '6', '9', '3', '6', '4', '8', '8', '4', '7', '1', '2', '7', '0']
>>> np.array(lst, dtype=np.float64)
array([1., 2., 6., 9., 3., 6., 4., 8., 8., 4., 7., 1., 2., 7., 0.])
```
(2) list转换为字符串类型数组。  
```python
>>> import numpy as np
>>> a = [["a", "b", "c", "d"],[1, 2, 3, 4]]
>>> a
[['a', 'b', 'c', 'd'], [1, 2, 3, 4]]
>>> np.array(a, dtype=np.str_)
array([['a', 'b', 'c', 'd'],
       ['1', '2', '3', '4']], dtype='<U1')
```
(3) float -> int
```python
cluster_frames = np.array(df["frame1"].dropna(), dtype=np.int32)-1
```
(4) 将列表转换为numpy数组，其中列表中有数字也有字符串，此时适合用的类型是dtype=object。
```python
>>> import numpy as np
>>> test = ["1", "2", "3", "4", 5]
>>> tttt = np.array(test, dtype=object)
>>> tttt
array(['1', '2', '3', '4', 5], dtype=object)
```