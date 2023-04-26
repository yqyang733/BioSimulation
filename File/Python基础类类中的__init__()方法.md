# 👏 Python基础|类|类中的__init__()方法

---
[封面PPT](../PPT/Python基础类类中的__init__()方法.pptx)

---
## Contents
- [类中__init__()作用](#类中__init__()作用)
- [计算圆的周长和面积实例，定义__init__()方法](#计算圆的周长和面积实例，定义__init__()方法)
- [计算圆的周长和面积实例，不定义__init__()方法](#计算圆的周长和面积实例，不定义__init__()方法)

---
写在前面：后面会根据理解程度的深入不断更新~

---
<a name="类中__init__()作用" />

## 1. 类中__init__()作用
(1) 定义类的一些通用属性，这些属性可能会在类中的多个函数中反复用到。
(2) 方便实例化类时参数传递。
(3) 定义类的通用属性之后，使该类的其他函数中传入更少的参数。

---
<a name="计算圆的周长和面积实例，定义__init__()方法" />

## 2. 计算圆的周长和面积实例，定义__init__()方法
```python
import math

class Circle():
    def __init__(self, r):
        self.r = r
    def get_area(self):
        return (math.pi * self.r ** 2)
    def get_peri(self):
        return (2 * math.pi * self.r)

Cir = Circle(3)
print("圆的属性:", Cir.__dict__)
print("圆的面积:", Cir.get_area())
print("圆的周长:", Cir.get_peri())
```
> 输出：   
> 圆的属性: {'r': 3}   
> 圆的面积: 28.274333882308138   
> 圆的周长: 18.84955592153876   

在该例子中，无论是计算圆的面积还是周长，都需要用到圆的半径。所以圆的半径就是圆的属性。即在__init__()方法中就定义了圆的半径。后面计算周长和面积的时候传参也更少。

---
<a name="计算圆的周长和面积实例，不定义__init__()方法">

## 3. 计算圆的周长和面积实例，不定义__init__()方法
```python
import math

class Circle():
    def get_area(self, r):
        return (math.pi * r ** 2)
    def get_peri(self, r):
        return (2 * math.pi * r)

Cir = Circle()
print("圆的属性:", Cir.__dict__)
print("圆的面积:", Cir.get_area(3))
print("圆的周长:", Cir.get_peri(3))
```
不定义__init__()方法也能达到相同的目的，但是圆的属性一栏是空的，并且该类的其他函数中需要传入更多的参数。