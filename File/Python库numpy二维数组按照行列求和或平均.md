👏 Python库|numpy|二维数组按照行列求和或平均

---
[TOC]

---
## 按行
(1) 二维数组按行求和
```python
>>> import numpy as np

>>> tmp = np.array([[1, 2, 3, 4], [4, 5, 6, 7], [7, 8, 9, 10]])
>>> print(tmp.sum(axis=1))
[10 22 34]
```
(2) 二维数组按行求平均
```python
>>> import numpy as np

>>> tmp = np.array([[1, 2, 3, 4], [4, 5, 6, 7], [7, 8, 9, 10]])
>>> print(tmp.mean(axis=1))
[2.5 5.5 8.5]
```

## 按列
(1) 二维数组按列求和
```python
>>> import numpy as np

>>> tmp = np.array([[1, 2, 3, 4], [4, 5, 6, 7], [7, 8, 9, 10]])
>>> print(tmp.sum(axis=0))
[12 15 18 21]
```
(2) 二维数组按列求平均
```python
>>> import numpy as np

>>> tmp = np.array([[1, 2, 3, 4], [4, 5, 6, 7], [7, 8, 9, 10]])
>>> print(tmp.mean(axis=0))
[4. 5. 6. 7.]
```