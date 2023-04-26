👏 Gromacs|gmx angle

---
[TOC]

---
## 简介
计算键角和二面角的分布及相关。

## 格式
```text
gmx angle [-f [<.xtc/.trr/...>]] [-n [<.ndx>]] [-od [<.xvg>]] [-ov [<.xvg>]]
    [-of [<.xvg>]] [-ot [<.xvg>]] [-oh [<.xvg>]] [-oc [<.xvg>]]
    [-or [<.trr>]] [-nice ] [-b ] [-e ] [-dt ]
    [-[no]w] [-xvg ] [-type ] [-[no]all] [-binwidth ]
    [-[no]periodic] [-[no]chandler] [-[no]avercorr] [-acflen ]
    [-[no]normalize] [-P ] [-fitfn ] [-beginfit ]
    [-endfit ]
```

## 参数
**输入/输出文件选项**
|参数|默认值|类型|说明|
|:----|:----|:----|:----|
|-f [<.xtc/.trr/...>]|traj.xtc|输入|轨迹：xtc trr cpt trj gro g96 pdb tng|
|-n [<.ndx>]|angle.ndx|输入|索引文件。|
|-od [<.xvg>]|angdist.xvg|输出|xvgr/xmgr文件。|
|-ov [<.xvg>]|angaver.xvg|输出，可选|xvgr/xmgr文件。|
|-of [<.xvg>]|dihfrac.xvg|输出，可选|xvgr/xmgr文件。|
|-ot [<.xvg>]|dihtrans.xvg|输出，可选|xvgr/xmgr文件。|
|-oh [<.xvg>]|trhisto.xvg|输出，可选|xvgr/xmgr文件。|
|-oc [<.xvg>]|dihcorr.xvg|输出，可选|xvgr/xmgr文件。|
|-or [<.trr>]|traj.trr|输出，可选|兼容xdr格式的轨迹。|

**控制选项**
|参数|默认值|说明|
|:----|:----|:----|
|-nice \<int>|19|设置优先级。|
|-b \<time>|0|从轨迹文件中读取的第一帧(ps)。|
|-e \<time>|0|从轨迹文件中读取的最后一帧(ps)。|
|-dt \<time>|0|只使用t除以dt的余数等于第一帧时间(ps)的帧，即两帧之间的时间间隔。|
|-[no]w|no|查看输出的.xvg，.xpm，.eps和.pdb文件。|
|-xvg \<enum>|xmgrace|xvg绘图格式：xmgrace，xmgr，none。|
|-type \<enum>|angle|要分析的键角类型：angle，dihedral，improper，ryckaert-bellemans|
|-[no]all|no|按索引文件中的出现顺序，在平均值文件中单独输出每个键角的平均值。|
|-binwidth \<real>|1|计算分布的分格值(单位: 度)。|
|-[no]periodic|yes|输出二面角除以360度的余数。|
|-[no]chandler|no|使用Chandler相关函数(N[trans] = 1，N[gauche] = 0)而不是余弦相关函数。转变的定义为phi < -60或phi > 60。|
|-[no]avercorr|no|对单个键角或二面角的相关函数进行平均。|
|-acflen \<int>|-1|ACF的长度，默认为帧数的一半。|
|-[no]normalize|yes|归一化ACF。|
|-P \<enum>|0|ACF Legendre多项式的阶数(0表示不使用)：0，1，2，3|
|-fitfn \<enum>|none|拟合函数：none，exp，aexp，exp_exp，vac，exp5，exp7，exp9，erffit|
|-beginfit \<real>|0|对相关函数进行指数拟合的起始时间。|
|-endfit \<real>|-1|对相关函数进行指数拟合的终止时间，-1表示直到最终。|

## 常规使用方法
（1）**gmx angle用于计算一些键角或二面角的角度分布。**

（2）**利用-ov选项，可以得到一组键角的平均值随时间的变化关系图。** 使用-all选项时第一幅图为平均键角，其他则为单个键角。例如如果我们选择几个不同的原子组合来计算其角度。
```text
75    76   1145
75    76   1147
222  223   1145
```
则默认输出文件angaver.xvg中共包含4列数据，首列为3个角度平均值，之后每列为每个角度值。

（3）**利用-of选项，gmx angle也会计算反式二面角的比例(仅适用于二面角)与时间的函数关系**，但这可能只适用于少量的二面角。

（4）**利用-oc选项，可计算二面角的相关函数。**

（5）需要注意，对键角在索引文件中必须包含原子三元组，对二面角则必须包含原子四元组。否则程序会崩溃。

（6）**利用-or选项可生成包含所选二面角sin和cos函数值的轨迹文件。** 当利用gmx covar进行主成分分析时，此轨迹文件可作为输入。

（7）**利用-ot选项可以记录多重度为3的二面角旋转异构体之间的转变。** 假定输入轨迹各帧之间的时间间隔相等，可利用-oh选项得到转变间隔时间的直方图。

## 源码位置及解析
gromacs-2022.2/src/gromacs/gmxana/gmx_angle.cpp

## 示例
