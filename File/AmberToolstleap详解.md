👏 AmberTools|tleap详解

---
[TOC]

---
## tleap命令
（1）savepdb
简介：保存unit为pdb格式文件。
语法：savePdb unit filename
示例：savepdb ALA ala.pdb
（2）solvatebox
简介：加水。
语法：
示例：
加立方体水盒子：后面加iso。
```shell
solvatebox complex TIP3PBOX 15.0 iso
```
(3) setBox
简介：设置水盒子。仅设置水盒子，添加水的操作需要再进行solvatebox进行。   
示例：
对溶质添加x轴上扩充120埃，在y轴和z轴上扩充20埃的水盒子。  
```shell
setbox mol vdw {120 20 20} 
solvatebox mol TIP3PBOX 0
```

## 参考资料
1. [AmberTools官网](https://ambermd.org/AmberTools.php)