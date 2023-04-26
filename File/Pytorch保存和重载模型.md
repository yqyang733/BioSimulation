👏 Pytorch|保存和重载模型

---
[TOC]

---
## 保存模型
**保存方式一：** 只存储模型中的参数，该方法速度快，占用空间少 **（官方推荐使用）**。
**代码示例：**
```python
import torch

model = VGGNet()
torch.save(model.state_dict(), PATH)
```
**对应该保存方式的重载模型方式：**
**代码示例：**
```python
import torch

new_model = VGGNet()                          
new_model.load_state_dict(torch.load(PATH)) 
```

**保存方式二：** 存储整个模型，不仅存储模型的架构，也存储模型的参数。
**代码示例：**
```python
import torch

torch.save(model, PATH)
```
**对应该保存方式的重载模型方式：**
**代码示例：**
```python
# 模型类必须在别的地方定义，模型的类必须在运行文件中进行定义，不然会报错。
import torch

new_model = torch.load(PATH) 
```