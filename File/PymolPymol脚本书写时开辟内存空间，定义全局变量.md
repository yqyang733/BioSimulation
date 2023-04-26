# 👏 Pymol|Pymol脚本书写时开辟内存空间，定义全局变量

---
[封面PPT](../PPT/)

---
## Contents
- [简介](#简介)
- [全局字典方式定义全局变量](#全局字典方式定义全局变量)
- [pymol.store定义全局变量](#pymol.store定义全局变量)

---
<a name="简介" />

## 简介

在写pymol脚本的时候。和python不同，pymol中新建一个变量总是无法真正的在内存中开辟空间，从而会报错。解决方式有两种：

---
<a name="全局字典方式定义全局变量" />

## 全局字典方式定义全局变量

1. 在iterate命令中会使用全局字典的形式定义变量：myspace = {"lst":[]}

---
<a name="pymol.store定义全局变量" />

## pymol.store定义全局变量

2. 使用pymol.stored
from pymol import stored
然后使用stored.name定义变量。