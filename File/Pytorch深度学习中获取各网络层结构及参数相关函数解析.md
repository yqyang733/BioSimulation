👏 Pytorch|深度学习中获取各网络层结构及参数相关函数解析

[封面PPT](../PPT/Pytorch深度学习中获取各网络层结构及参数相关函数解析.pptx)

---
[TOC]

---
网络结构示例，以下参数获得均以该网络例子为基准。

```python
import torch
import torch.nn as nn

class Net(nn.Module):

    def __init__(self, num_class=10):
        super().__init__()
        self.conv = nn.Sequential(
            nn.Conv2d(in_channels=3, out_channels=6, kernel_size=3),
            nn.ReLU(inplace=True),
            nn.Conv2d(in_channels=6, out_channels=9, kernel_size=3),
            nn.ReLU(inplace=True),
        )

        self.classifier = nn.Sequential(
            nn.Linear(9*8*8, 128),
            nn.ReLU(inplace=True),
            nn.Dropout(p=0.5),
            nn.Linear(128, 64),
            nn.ReLU(inplace=True),
            nn.Dropout(p=0.2),
            nn.Linear(64, num_class)
        )

    def forward(self, x):
        output = self.features(x)
        output = output.view(output.size()[0], -1)
        output = self.classifier(output)

        return output
```

## 网络结构

>Net:
>
>……conv:
>
>…………Conv2d  
>…………nn.ReLU  
>…………nn.Conv2d  
>…………nn.ReLU  
>
>……classifier:
>
>…………nn.Linear  
>…………nn.ReLU  
>…………nn.Dropout  
>…………nn.Linear  
>…………nn.ReLU  
>…………nn.Dropout  
>…………nn.Linear  

## model.modules()
简介：如下所示，直接使用model.modules()返回值是一个生成器，是一个可迭代变量，可通过列表推导式将返回值取出来进一步进行观察。
model.modules()迭代遍历模型的所有子层，所有子层即指nn.Module子类，在本文的例子中，Net(), conv(), classifier(),以及卷积，池化，ReLU, Linear, BN, Dropout等都是nn.Module子类，也就是model.modules()会迭代的遍历它们所有对象。由下方打印示例所示，会根据上述网络结构的顺序把网络中的所有递归层全部打印出来。

```python
from temodel import Net

model = Net()
print(model.modules())
model_modules = [x for x in model.modules()]
print(model_modules)
print(len(model_modules))

# <generator object Module.modules at 0x7f88785cb990>
# [Net(
#   (conv): Sequential(
#     (0): Conv2d(3, 6, kernel_size=(3, 3), stride=(1, 1))
#     (1): ReLU(inplace=True)
#     (2): Conv2d(6, 9, kernel_size=(3, 3), stride=(1, 1))
#     (3): ReLU(inplace=True)
#   )
#   (classifier): Sequential(
#     (0): Linear(in_features=576, out_features=128, bias=True)
#     (1): ReLU(inplace=True)
#     (2): Dropout(p=0.5, inplace=False)
#     (3): Linear(in_features=128, out_features=64, bias=True)
#     (4): ReLU(inplace=True)
#     (5): Dropout(p=0.2, inplace=False)
#     (6): Linear(in_features=64, out_features=10, bias=True)
#   )
# ), Sequential(
#   (0): Conv2d(3, 6, kernel_size=(3, 3), stride=(1, 1))
#   (1): ReLU(inplace=True)
#   (2): Conv2d(6, 9, kernel_size=(3, 3), stride=(1, 1))
#   (3): ReLU(inplace=True)
# ), Conv2d(3, 6, kernel_size=(3, 3), stride=(1, 1)), ReLU(inplace=True), Conv2d(6, 9, kernel_size=(3, 3), stride=(1, 1)), ReLU(inplace=True), Sequential(
#   (0): Linear(in_features=576, out_features=128, bias=True)
#   (1): ReLU(inplace=True)
#   (2): Dropout(p=0.5, inplace=False)
#   (3): Linear(in_features=128, out_features=64, bias=True)
#   (4): ReLU(inplace=True)
#   (5): Dropout(p=0.2, inplace=False)
#   (6): Linear(in_features=64, out_features=10, bias=True)
# ), Linear(in_features=576, out_features=128, bias=True), ReLU(inplace=True), Dropout(p=0.5, inplace=False), Linear(in_features=128, out_features=64, bias=True), ReLU(inplace=True), Dropout(p=0.2, inplace=False), Linear(in_features=64, out_features=10, bias=True)]
# 14
```

## model.named_modules()
简介：与model.models()非常相似，model.named_modules()不但返回模型的所有子层，还会返回这些层的名字，如下所示。

```python
from temodel import Net

model = Net()
print(model.named_modules())
model_named_modules = [x for x in model.named_modules()]
print(model_named_modules)
print(len(model_named_modules))

# <generator object Module.named_modules at 0x7f37055a9990>
# [('', Net(
#   (conv): Sequential(
#     (0): Conv2d(3, 6, kernel_size=(3, 3), stride=(1, 1))
#     (1): ReLU(inplace=True)
#     (2): Conv2d(6, 9, kernel_size=(3, 3), stride=(1, 1))
#     (3): ReLU(inplace=True)
#   )
#   (classifier): Sequential(
#     (0): Linear(in_features=576, out_features=128, bias=True)
#     (1): ReLU(inplace=True)
#     (2): Dropout(p=0.5, inplace=False)
#     (3): Linear(in_features=128, out_features=64, bias=True)
#     (4): ReLU(inplace=True)
#     (5): Dropout(p=0.2, inplace=False)
#     (6): Linear(in_features=64, out_features=10, bias=True)
#   )
# )), ('conv', Sequential(
#   (0): Conv2d(3, 6, kernel_size=(3, 3), stride=(1, 1))
#   (1): ReLU(inplace=True)
#   (2): Conv2d(6, 9, kernel_size=(3, 3), stride=(1, 1))
#   (3): ReLU(inplace=True)
# )), ('conv.0', Conv2d(3, 6, kernel_size=(3, 3), stride=(1, 1))), ('conv.1', ReLU(inplace=True)), ('conv.2', Conv2d(6, 9, kernel_size=(3, 3), stride=(1, 1))), ('conv.3', ReLU(inplace=True)), ('classifier', Sequential(
#   (0): Linear(in_features=576, out_features=128, bias=True)
#   (1): ReLU(inplace=True)
#   (2): Dropout(p=0.5, inplace=False)
#   (3): Linear(in_features=128, out_features=64, bias=True)
#   (4): ReLU(inplace=True)
#   (5): Dropout(p=0.2, inplace=False)
#   (6): Linear(in_features=64, out_features=10, bias=True)
# )), ('classifier.0', Linear(in_features=576, out_features=128, bias=True)), ('classifier.1', ReLU(inplace=True)), ('classifier.2', Dropout(p=0.5, inplace=False)), ('classifier.3', Linear(in_features=128, out_features=64, bias=True)), ('classifier.4', ReLU(inplace=True)), ('classifier.5', Dropout(p=0.2, inplace=False)), ('classifier.6', Linear(in_features=64, out_features=10, bias=True))]
# 14
```

## model.children()
简介：如果把这个网络模型Net按层次从外到内进行划分的话，conv和classifier是Net的子层，而conv2d, ReLU这些是conv的子层，Linear, Dropout, ReLU等是classifier的子层，上面的model.modules()不但会遍历模型的子层，还会遍历子层的子层，以及所有子层。
而model.children()只会遍历模型的子层，这里即是conv和classifier。

```python
from temodel import Net

model = Net()
print(model.children())
model_children = [x for x in model.children()]
print(model_children)
print(len(model_children))

# <generator object Module.children at 0x7f1902073990>
# [Sequential(
#   (0): Conv2d(3, 6, kernel_size=(3, 3), stride=(1, 1))
#   (1): ReLU(inplace=True)
#   (2): Conv2d(6, 9, kernel_size=(3, 3), stride=(1, 1))
#   (3): ReLU(inplace=True)
# ), Sequential(
#   (0): Linear(in_features=576, out_features=128, bias=True)
#   (1): ReLU(inplace=True)
#   (2): Dropout(p=0.5, inplace=False)
#   (3): Linear(in_features=128, out_features=64, bias=True)
#   (4): ReLU(inplace=True)
#   (5): Dropout(p=0.2, inplace=False)
#   (6): Linear(in_features=64, out_features=10, bias=True)
# )]
# 2
```

## model.named_children()
简介：与model.children()非常相似，但model.named_children()不但返回模型的一级子层，还会返回这些层的名字，如下所示。

```python
from temodel import Net

model = Net()
print(model.named_children())
model_named_children = [x for x in model.named_children()]
print(model_named_children)
print(len(model_named_children))

# [('conv', Sequential(
#   (0): Conv2d(3, 6, kernel_size=(3, 3), stride=(1, 1))
#   (1): ReLU(inplace=True)
#   (2): Conv2d(6, 9, kernel_size=(3, 3), stride=(1, 1))
#   (3): ReLU(inplace=True)
# )), ('classifier', Sequential(
#   (0): Linear(in_features=576, out_features=128, bias=True)
#   (1): ReLU(inplace=True)
#   (2): Dropout(p=0.5, inplace=False)
#   (3): Linear(in_features=128, out_features=64, bias=True)
#   (4): ReLU(inplace=True)
#   (5): Dropout(p=0.2, inplace=False)
#   (6): Linear(in_features=64, out_features=10, bias=True)
# ))]
# 2
```

## model.parameters()
简介：返回网络结构中最底层的所有层的参数。

```python
from temodel import Net

model = Net()
print(model.parameters())
model_parameters = [x for x in model.parameters()]
print(model_parameters)
print(len(model_parameters))

# <generator object Module.parameters at 0x7f095e4b6990>
# [Parameter containing:
# tensor([[[[ 1.5672e-01,  8.5022e-02, -1.9003e-01],
#           [-1.0299e-01, -6.4035e-02,  1.5543e-01],
#           [-1.7732e-01,  5.9933e-02, -1.5432e-01]],

#          [[ 8.0891e-02, -1.9001e-01, -8.9000e-02],
#           [-1.5993e-01, -1.7304e-01,  1.3154e-01],
#           [ 4.6030e-02,  2.3695e-02,  1.4548e-01]],

#          [[ 3.6459e-02, -7.1986e-02,  1.4795e-01],
#           [ 1.2099e-01,  2.9547e-02,  2.0741e-02],
#           [-4.0435e-02, -1.9884e-02, -1.0583e-01]]],
#           ……
#           [-0.0310,  0.0544,  0.0998,  0.0333,  0.0226,  0.0956, -0.0950, -0.0097,
#          -0.1074, -0.0559, -0.0026, -0.0790,  0.0501,  0.0104, -0.0191,  0.0398,
#           0.1229,  0.0531, -0.0481, -0.0470,  0.0287,  0.1163, -0.0574,  0.0928,
#           0.0903,  0.0462,  0.0811, -0.1162,  0.0640,  0.0674, -0.0848,  0.0770,
#          -0.0019, -0.0629,  0.0477,  0.0885, -0.0412, -0.0302,  0.0687,  0.0389,
#          -0.0023, -0.0824, -0.0072,  0.0531, -0.1086, -0.0063, -0.1052,  0.0506,
#           0.0624,  0.0584,  0.1065,  0.0696, -0.0217, -0.0636, -0.0536,  0.0366,
#          -0.0289, -0.1242, -0.0045, -0.0371,  0.0963,  0.0221, -0.0411, -0.0537]],
#        requires_grad=True), Parameter containing:
# tensor([ 0.0218, -0.0065,  0.0448, -0.0209,  0.0541,  0.0377,  0.0680, -0.0571,
#         -0.0273,  0.0927], requires_grad=True)]
# 10
```

## model.named_parameters()
简介：返回网络结构中最底层的所有层的名字以及参数。

```python
from temodel import Net

model = Net()
print(model.named_parameters())
model_named_parameters = [x for x in model.named_parameters()]
print(model_named_parameters)
print(len(model_named_parameters))

# <generator object Module.named_parameters at 0x7f07692df990>
# [('conv.0.weight', Parameter containing:
# tensor([[[[ 0.0609,  0.0010, -0.0673],
#           [ 0.1317,  0.0431, -0.1739],
#           [-0.0770, -0.0562, -0.0636]],

#          [[ 0.1521,  0.1039, -0.0044],
#           [-0.1146,  0.0620,  0.1771],
#           [ 0.0854,  0.0459, -0.1763]],

#          [[ 0.0049, -0.0264,  0.0557],
#           [-0.1627, -0.1535,  0.0671],
#           [ 0.1506, -0.1787,  0.1066]]],


#         [[[-0.0280, -0.0207, -0.0461],
#           [-0.1175,  0.0028,  0.1538],
#           [ 0.0773,  0.1027, -0.1398]],
#           ……
#          -1.2218e-01,  1.0992e-02,  7.8785e-02,  5.9861e-02, -8.4948e-02,
#          -5.0510e-02, -1.1441e-01, -8.6045e-02,  5.2428e-02, -1.1190e-01,
#          -1.1121e-01,  1.1976e-01, -6.4868e-02,  7.4042e-02, -3.1632e-02,
#           1.3492e-02,  4.1203e-04,  1.0810e-01, -6.0778e-02,  1.0632e-02,
#          -2.7233e-02, -1.0846e-01,  6.5591e-02, -4.2251e-02,  1.9229e-02,
#          -1.1945e-01, -9.5764e-02,  3.0486e-02, -1.1423e-01, -3.7346e-02,
#          -6.2044e-02,  6.3128e-02,  1.1994e-01,  1.2201e-01, -9.7853e-02,
#          -6.6571e-02,  3.4964e-02, -2.3597e-02,  1.9873e-02]],
#        requires_grad=True)), ('classifier.6.bias', Parameter containing:
# tensor([-0.0320,  0.0305, -0.1125,  0.0509, -0.0654, -0.0097,  0.0493, -0.0013,
#          0.0725, -0.0042], requires_grad=True))]
# 10
```