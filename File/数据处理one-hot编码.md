👏 数据处理|one-hot编码

---
[TOC]

---
## 简介

## 实践操作

```python
import numpy as np
from sklearn.preprocessing import OneHotEncoder

label = [1,2,3,4,5]
enc_label = OneHotEncoder().fit(np.array(label).reshape(-1, 1))
enc_label.transform(np.array([1,2,2]).reshape(-1,1)).toarray().T

# array([[1., 0., 0.],
#        [0., 1., 1.],
#        [0., 0., 0.],
#        [0., 0., 0.],
#        [0., 0., 0.]])

enc_label.transform(np.array([1,2,2]).reshape(-1,1)).toarray()

# array([[1., 0., 0., 0., 0.],
#        [0., 1., 0., 0., 0.],
#        [0., 1., 0., 0., 0.]])
```