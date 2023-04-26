👏 Pytorch|Variable()实现普通tensor和存储梯度tensor间的切换

---
[TOC]

---
## 实现脚本
```python
>>> import torch
>>> import torch.nn.functional as F
>>> from torch.autograd import Variable

>>> def create_var(tensor, requires_grad=None):
...     if requires_grad is None:
...         return Variable(tensor)
...     else:
...         return Variable(tensor, requires_grad=requires_grad)

>>> input = torch.randn(3,4)
>>> input
tensor([[-0.0334, -1.3225, -0.5638, -0.2359],
        [-0.3935, -0.8167, -1.4941,  2.0484],
        [-1.1414, -0.4522,  0.1570,  0.7436]])

>>> d = create_var(input, True)
>>> d
tensor([[-0.0334, -1.3225, -0.5638, -0.2359],
        [-0.3935, -0.8167, -1.4941,  2.0484],
        [-1.1414, -0.4522,  0.1570,  0.7436]], requires_grad=True)
```