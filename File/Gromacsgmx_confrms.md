👏 Gromacs|gmx confrms

---
[TOC]

---
## 简介
叠合两个结构并计算RMSD

## 格式
```text
gmx confrms [-f1 [<.tpr/.tpb/...>]] [-f2 [<.gro/.g96/...>]]
    [-o [<.gro/.g96/...>]] [-n1 [<.ndx>]] [-n2 [<.ndx>]]
    [-no [<.ndx>]] [-nice ] [-[no]w] [-[no]one] [-[no]mw]
    [-[no]pbc] [-[no]fit] [-[no]name] [-[no]label] [-[no]bfac]
```

## 参数
**输入/输出文件选项**
|参数|默认值|类型|说明|
|:----|:----|:----|:----|
|-f1 [<.tpr/.tpb/...>]|conf1.gro|输入|结构+质量(db)：tpr tpb tpa gro g96 pdb brk ent|
|-f2 [<.gro/.g96/...>]|conf2.gro|输入|结构文件：gro g96 pdb brk ent esp tpr tpb tpa|
|-o [<.gro/.g96/...>]|fit.pdb|输出|结构文件：gro g96 pdb brk ent esp|
|-n1 [<.ndx>]|fit1.ndx|输入，可选|索引文件。|
|-n2 [<.ndx>]|fit2.ndx|输入，可选|索引文件。|
|-no [<.ndx>]|match.ndx|输出，可选|索引文件。|

**控制选项**
|参数|默认值|说明|
|:----|:----|:----|
|-nice \<int>|19|设置优先级。|
|-[no]w|no|查看输出的.xvg，.xpm，.eps和.pdb文件。|
|-[no]one|no|只输出叠合后的结构。|
|-[no]mw|yes|叠合与计算RMSD时使用质量加权。|
|-[no]pbc|no|尝试将分子恢复完整。|
|-[no]fit|yes|将目标结构与参考结构进行最小二乘叠合。|
|-[no]name|no|只考虑名称匹配的原子。|
|-[no]label|no|增加链标识，第一个结构为A，第二个结构为B。|
|-[no]bfac|no|根据原子的MSD值输出B因子。|

## 常规使用方法
（1）gmx confrms首先将第二个结构最小二乘叠合到第一个结构，然后再计算两个结构的均方根偏差(RMSD，root mean square deviation)。两个结构的原子数不必相同，只要用于叠合的两个索引组一样即可。使用-name选项时只对所选组中名称匹配的原子进行叠合和RMSD计算。**当比较蛋白质的突变体时这个功能很有用。**

（2）叠合的结构会写入一个文件中。在这个.pdb文件中两个结构会当作独立的模型(使用rasmol –nmrpdb)。使用-bfac选项时根据原子的MSD值计算的B因子也会写入这个.pdb文件中。

## 源码位置及解析
gromacs-2022.2/src/gromacs/gmxana/gmx_confrms.cpp

## 示例
