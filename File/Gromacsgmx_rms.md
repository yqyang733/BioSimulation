👏 Gromacs|gmx rms

---
[TOC]

---
## 简介
计算与参考结构之间的RMSD及其矩阵。

## 格式
```text
gmx rms [-s [<.tpr/.tpb/...>]] [-f [<.xtc/.trr/...>]]
    [-f2 [<.xtc/.trr/...>]] [-n [<.ndx>]] [-o [<.xvg>]] [-mir [<.xvg>]]
    [-a [<.xvg>]] [-dist [<.xvg>]] [-m [<.xpm>]] [-bin [<.dat>]]
    [-bm [<.xpm>]] [-nice ] [-b ] [-e ] [-dt ]
    [-tu ] [-[no]w] [-xvg ] [-what ] [-[no]pbc]
    [-fit ] [-prev ] [-[no]split] [-skip ] [-skip2 ]
    [-max ] [-min ] [-bmax ] [-bmin ] [-[no]mw]
    [-nlevels ] [-ng ]
```

## 参数
**输入/输出文件选项**
|参数|默认值|类型|说明|
|:----|:----|:----|:----|
|-s [<.tpr/.tpb/...>]|topol.tpr|输入|结构+质量(db): tpr tpb tpa gro g96 pdb brk ent|
|-f [<.xtc/.trr/...>]|traj.xtc|输入|轨迹：xtc trr cpt trj gro g96 pdb tng|
|-f2 [<.xtc/.trr/...>]|traj.xtc|输入，可选|轨迹：xtc trr cpt trj gro g96 pdb tng|
|-n [<.ndx>]|index.ndx|输入，可选|索引文件。|
|-o [<.xvg>]|rmsd.xvg|输出|xvgr/xmgr文件。|
|-mir [<.xvg>]|rmsdmir.xvg|输出，可选|xvgr/xmgr文件。|
|-a [<.xvg>]|avgrp.xvg|输出，可选|xvgr/xmgr文件。|
|-dist [<.xvg>]|rmsd-dist.xvg|输出，可选|xvgr/xmgr文件。|
|-m [<.xpm>]|rmsd.xpm|输出，可选|X PixMap兼容矩阵文件。|
|-bin [<.dat>]|rmsd.dat|输出，可选|通用数据文件。|
|-bm [<.xpm>]|bond.xpm|输出，可选|X PixMap兼容矩阵文件。|

**控制选项**
|参数|默认值|说明|
|:----|:----|:----|
|-nice \<int>|19|设置优先级。|
|-b|0|从轨迹文件中读取的第一帧(ps)。|
|-e|0|从轨迹文件中读取的最后一帧(ps)。|
|-dt|0|只使用t除以dt的余数等于第一帧时间(ps)的帧, 即两帧之间的时间间隔。|
|-tu \<enum>|ps|时间单位：fs，ps，ns，us，ms，s|
|-[no]w|no|程序结束后查看输出的.xvg，.xpm，.eps和.pdb文件。|
|-xvg \<enum>|xmgrace|xvg绘图格式：xmgrace，xmgr，none。|
|-what \<enum>|rmsd|结构差异类型：rmsd，rho，rhosc。|
|-[no]pbc|yes|PBC检查。|
|-fit \<enum>|rot+trans|叠合到参考结构：rot+trans，translation，none。|
|-prev \<int>|0|和前面的帧进行比较。|
|-[no]split|no|在时间为0的地方分割图。|
|-skip \<int>|1|每nr帧写入矩阵一次。|
|-skip2 \<int>|1|每nr帧写入矩阵一次。|
|-max \<real>|-1|比较矩阵的最大水平。|
|-min \<real>|-1|比较矩阵的最小水平。|
|-bmax \<real>|-1|键角矩阵的最大水平。|
|-bmin \<real>|-1|键角矩阵的最小水平。|
|-[no]mw|yes|重叠部分使用质量权重。|
|-nlevels \<int>|80|矩阵的水平数。|
|-ng \<int>|1|计算RMS的组数。|

## 常规使用方法
（1）**gmx rms通过计算均方根偏差(RMSD，root mean square deviation)，尺寸无关的ρ相似性参数(rho)或标度ρ参数(rhosc)来比较两个结构。** 可利用-what选项来选择计算那个参数。

（2）**程序会将轨迹(-f)中的每个结构与参考结构进行比较。** 参考结构取自结构文件(-s)。

（3）使用-mir选项，还会与参考结构的镜像进行比较。这可以作为一个很有用的参考'显著'值。

（4）**选项-prev会对当前帧的结构与前面指定帧中的结构进行比较。**

（5）**选项-m将生成一个.xpm格式的矩阵，其值为轨迹中所有结构彼此之间的比较值。** 这个矩阵文件可以使用如xv之类的程序进查看，也可以使用gmx xpm2ps将其转换为postscript格式。

（6）**选项-fit控制结构彼此之间的最小二乘叠合：** 完全叠合(旋转和平移)，仅平移，或不叠合。

（7）**选项-mw控制是否使用质量加权。** 如果你选择了这个选项(默认)，并提供一个有效的.tpr文件，程序会读取.tpr文件中的质量，否则将会从GMXLIB目录下的atommass.dat文件中获取质量。对于蛋白质这还可以，但对于别的分子来说就未必了。对未知的原子会分配默认的质量12.011 amu(碳原子)。你可以通过打开-debug选项并检查log文件来判断是否这样。

（8）使用-f2选项，程序会从第二个轨迹文件中读取'其他结构'，并生成两个轨迹之间的比较矩阵。

（9）选项-bin会对比较矩阵进行二进制转储。

（10）**选项-bm会产生平均键角偏差的矩阵，类似-m选项。** 比较时只会考虑比较组中原子之间的键。

## 源码位置及解析
gromacs-2022.2/src/gromacs/gmxana/gmx_rms.cpp

## 示例
（1）以第一帧为参考计算轨迹的RMSD。
```python
# 一般情况下先对轨迹文件进行处理，变成流畅的动画格式的xtc文件再计算RMSD。
gmx rms -f cluster4_50ns.xtc -s npt.gro -o rms.xvg -n index.ndx
```