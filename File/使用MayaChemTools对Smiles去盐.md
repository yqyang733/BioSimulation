👏 使用MayaChemTools对Smiles去盐

---
[TOC]

---

## 1. 简介

有机化合物成盐之后往往能增加化合物的水溶性，但是盐类化合物在体内产生作用的部分一般是水解产物。所以数据前处理过程需要对Smiles进行去盐。

不同来源的化合物结构，其Smiles编码千差万别，需要统一表示。以钠盐为例,一个常见的表达方式为钠与羧酸氧之间通过键连接，比如：

> [Na]OC(=O)c1ccc(C[S+2]([O-])([O-]))cc1

这不利于后续的脱盐处理（会被当成金属有机化合物而不是盐），需要标准化为非键连接：

> [Na+].O=C([O-])c1ccc(CS(=O)=O)cc1

所以一般建议首先采用 molvs 包对所有的 Smiles 进行标准化处理，再使用本章内容对 Smiles 进行去盐。

## 2. 输入数据格式

输入数据格式第一列是小分子Smiles，第二列是小分子名字，两列以一个空格分隔。

## 3. 脚本

一共需要4个py文件，RDKitRemoveSalts.py、docopt.py、MiscUtil.py和RDKitUtil.py。其中主文件是RDKitRemoveSalts.py。

（1）[rdkit_desault.py](./使用MayaChemTools对Smiles去盐/rdkit_desault.py)  
（2）[docopt.py](./使用MayaChemTools对Smiles去盐/docopt.py)  
（3）[MiscUtil.py](./使用MayaChemTools对Smiles去盐/MiscUtil.py)  
（4）[RDKitUtil.py](./使用MayaChemTools对Smiles去盐/RDKitUtil.py)  

---

## 4. 使用方法

（1）批量将Smiles的.smi文件进行去盐操作。

```shell
python rdkit_desault.py -i in.smi -o out.smi
```
