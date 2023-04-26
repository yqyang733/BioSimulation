👏 Pytorch|单GPU设置

---
[TOC]

---
## 查看机器上GPU是否可用
```python
import torch
torch.cuda.is_available()
```

## 设置训练时的GPU或者CPU
```python
import torch

use_gpu = torch.cuda.is_available()

def get_device():
    device = "cpu"
    if use_gpu:
        print("GPU is available on this device!")
        device = "cuda"
    else:
        print("CPU is available on this device!")
    return device

device = get_device()
```

**指定固定号码的GPU**
```python
device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
model.to(device)
```

**指定多GPU**

**.to(device)和.cuda()的异同**
（1）.to(device)，可以指定使用CPU或GPU
（2）.cuda()，只能指定GPU

## 将模型放到GPU上

## 将数据放到GPU上