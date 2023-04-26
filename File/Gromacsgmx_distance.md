👏 Gromacs|gmx distance

---
[TOC]

---
## 简介
**计算两个位置之间的距离。** 计算一对位置间的距离随时间变化的函数。每个选择指定要计算的一组独立距离。每个选择应包括位置对，要计算的，如位置1-2、3-4等之间的距离。

## 格式
```text
gmx distance [-f [<.xtc/.trr/...>]] [-s [<.tpr/.tpb/...>]] [-n [<.ndx>]]
    [-oav [<.xvg>]] [-oall [<.xvg>]] [-oxyz [<.xvg>]] [-oh [<.xvg>]]
    [-oallstat [<.xvg>]] [-b ] [-e ] [-dt ]
    [-tu ] [-xvg ] [-[no]rmpbc] [-[no]pbc] [-sf ]
    [-selrpos ] [-select ] [-len ]
    [-tol ] [-binw ]
```

## 参数
**输入/输出文件选项**
|参数|默认值|类型|说明|
|:----|:----|:----|:----|
|-f [<.xtc/.trr/...>]|traj.xtc|输入, 可选|输入轨迹或单个构型：xtc trr cpt trj gro g96 pdb tng|
|-s [<.tpr/.tpb/...>]|topol.tpr|输入，可选|输入结构：tpr tpb tpa gro g96 pdb brk ent|
|-n [<.ndx>]|index.ndx|输入，可选|额外的索引组。|
|-oav [<.xvg>]|distave.xvg|输出，可选|平均距离随时间变化的函数。|
|-oall [<.xvg>]|dist.xvg|输出，可选|间距随时间变化的函数。|
|-oxyz [<.xvg>]|distxyz.xvg|输出，可选|间距(在x y z 方向上的分量)随时间变化的函数。|
|-oh [<.xvg>]|disthist.xvg|输出，可选|距离的直方图。|
|-oallstat [<.xvg>]|diststat.xvg|输出，可选|独立距离的统计。|

**控制选项**
|参数|默认值|说明|
|:----|:----|:----|
|-b \<time>|0|从轨迹文件中读取的第一帧(ps)。|
|-e \<time>|0|从轨迹文件中读取的最后一帧(ps)。|
|-dt \<time>|0|只使用t除以dt的余数等于第一帧时间(ps)的帧，即两帧之间的时间间隔|
|-tu \<enum>|ps|时间的单位：fs，ps，ns，us，ms，s|
|-xvg \<enum>|xmgrace|绘制的格式：none，xmgrace，xmgr。|
|-[no]rmpbc|yes|每一帧使用所有分子。|
|-[no]pbc|yes|使用周期性边界条件计算距离。|
|-sf \<file>|-|从文件中提供选择|
|-selrpos \<enum>|atom|选择参考的位置：atom，res_com，res_cog，mol_com，mol_cog，whole_res_com，whole_res_cog，whole_mol_com，whole_mol_cog，part_res_com，part_res_cog，part_mol_com，part_mol_cog，dyn_res_com，dyn_res_cog，dyn_mol_com，dyn_mol_cog|
|-select \<selection>|-|计算距离对的位置。|
|-len \<real>|0.1|直方图的平均距离。|
|-tol \<real>|1|直方图分布的宽度关于-len的函数。|
|-binw \<real>|0.001|直方图单元格的宽度。|

## 常规使用方法
（1）-oav记录下每个选择的平均距离随时间变化的函数。-oall记录下所有独立的间距随时间变化的函数。-oxyz也是记录所有独立的间距，但会记录距离的x，y，z分量而不是距离向量的大小。  
（2）-oh记录每个选择的距离的直方图，直方图的位置由-len和–tol来进行设置，分格的宽度由-binw设置。
（3）-oallstat记录下所有帧的每个单独距离的平均值和标准偏差。

## 计算距离的方法及注意点
**（1）计算原子对间距离。**
* gmx distance可以对索引组中列出的原子对间距离进行计算。因此要计算距离的组中总原子个数必须是偶数，否则会给出原子数目非偶数的错误。例如我们要计算体系中3号原子和5,9,13,19之间的距离，那么可以在索引文件中定义如下组：
```text
# index.ndx
[ dist ]
3 5 3 9 3 13 3 19
```
* 编号对也可以放于多行中，每行一个原子对编号。  
* 注意在有些gromacs版本中在处理这些编号对时存在错误，对1 2 2 3这样的编号对，会先将中间重复的编号合并为一个，变成1 2 3，从而导致无法计算。解决办法也很简单，就是更换顺序，保证相邻编号之间不存在重复。如上面的例子改成1 2 3 2就可以正常计算了。   
* 组中的原子对数目最好不要太多, 否则GROMACS运行时所需的内存太多, 可能导致失败.

**（2）计算质心间距离。**
要想计算两个组的质心之间的距离就有点麻烦，需要利用选区语法来完成。例如我们要计算残基1-4与残基7-9质心之间的距离，可以使用下面的方式：
```shell
gmx distance -s -f -select "com of resnr 1 to 4 plus com of resnr 7 to 9" -oav -oall
```

## 源码位置及解析
gromacs-2022.2/src/gromacs/trajectoryanalysis/modules/distance.cpp

## 示例
（1）测量gro文件两组残基质心间距离。
```shell
gmx_mpi distance -s prod.tpr -f prod.gro -select "com of resnr 1 to 52 plus com of resnr 333 to 527" -oall
gmx_mpi distance -s prod.tpr -f prod.gro -select "com of resnr 3 to 7 45 to 49 plus com of resnr 333 to 527" -oall
```
（2）测量xtc文件两个group质心间距离。
```shell
gmx distance -s npt.tpr -f prod.xtc -n index.ndx -select 'com of group "Chain_A" plus com of group "Chain_B"' -oall dist.xvg
```