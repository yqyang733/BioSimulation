👏 Python脚本|Python计算均值、方差和标准差

---
[TOC]

---
## Numpy计算
### 均值计算
(1) np.mean()计算均值。  
```python
>>> import numpy as np
>>> lst = [1,2,3,4,5,6,7]
>>> np.mean(lst)
4.0
```
(2) np.average()计算均值。  
```python
>>> import numpy as np
>>> lst = [1,2,3,4,5,6,7]
>>> np.average(lst)
4.0
```
(3) np.average()计算加权平均。  
```python
>>> import numpy as np
>>> lst = [1,2,3,4,5,6,7]
>>> np.average(lst, weights = [1,2,1,2,1,2,2])
4.2727272727272725
```

### 方差计算
(1) 计算总体方差  
```python
>>> import numpy as np
>>> lst = [1,2,3,4,5,6,7]
>>> np.var(lst)
4.0
```
(2) 计算样本方差  
```python
>>> import numpy as np
>>> lst = [1,2,3,4,5,6,7]
>>> np.var(lst, ddof = 1)
4.666666666666667
```
(3) 计算矩阵形式方差  
```python
>>> import numpy as np
>>> lst = [[1,2,3,4,5,6,7],[1,2,3,4,5,6,7]]
>>> np.var(lst) # 计算矩阵所有元素的方差
4.0
>>> np.var(lst, axis = 0) # 计算矩阵每一列的方差
array([0., 0., 0., 0., 0., 0., 0.])
>>> np.var(lst, axis = 1) # 计算矩阵每一行的方差
array([4., 4.])
```

### 标准差计算
(1) 计算总体标准差  
```python
>>> import numpy as np
>>> lst = [1,2,3,4,5,6,7]
>>> np.std(lst)
2.0
```
(2) 计算样本标准差  
```python
>>> import numpy as np
>>> lst = [1,2,3,4,5,6,7]
>>> np.std(lst, ddof = 1)
2.160246899469287
```
(3) 计算矩阵形式标准差  
```python
>>> import numpy as np
>>> lst = [[1,2,3,4,5,6,7],[1,2,3,4,5,6,7]]
>>> np.std(lst) # 计算矩阵所有元素的方差
2.0
>>> np.std(lst, axis = 0) # 计算矩阵每一列的方差
array([0., 0., 0., 0., 0., 0., 0.])
>>> np.std(lst, axis = 1) # 计算矩阵每一行的方差
array([2., 2.])
```