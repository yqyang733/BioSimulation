👏 Pytorch|数据预处理之Dataset类

---
[TOC]

---
## 简介
Dataset是pytorch进行数据加载的一个包装的类。可以用于对数据进行shuffle并分割成mini-batch等。Dataset是一个包装类，用于将数据包装为Dataset类，然后传入DataLoader中快捷的对数据进行操作。

## 源码解析
(1) 源码路径
```python
from torch.utils.data import Dataset
```

## 实践应用
写在前面：在继承一个Dataset类之后，我们需要重写len方法，len方法提供了dataset的大小；getitem方法，该方法是对整个数据集进行的索引。


temp:
https://blog.csdn.net/irving512/article/details/106591207/