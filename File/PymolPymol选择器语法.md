👏 Pymol|Pymol选择器语法

---
[TOC]

---
## 一般标示符
|操作符|别名|描述|
|:----|:----|:----|
|name CA|n.|原子名称为"CA"的原子|
|resn ALA|r.|残基名称为"ALA"的残基所有原子|
|resi 100-200|i.|残基编号在100到200之间所有残基的所有原子|
|id 123||PDB文件中原子编号为123的原子|

## 生物化学类结构选择
|操作符|别名|描述|
|:----|:----|:----|
|polymer.protein||选择蛋白(New in PyMOL 2.1)|
|polymer.nucleic||选择核酸(New in PyMOL 2.1)|
|hydrogens|h.|Hydrogen atoms|
|backbone|bb.|Polymer backbone atoms|
||||

## 范围运算
|操作符|别名|描述|
|:----|:----|:----|
|S1 within 12.3 of S2|w.|S2 12.3埃范围内S1的所有原子|

## 参考资料
1. https://pymolwiki.org/index.php/Selection_Algebra