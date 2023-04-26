👏 Gromacs|gmx insert-molecules

---
[TOC]

---
## 简介
将分子插入已有空位

## 格式
```text
gmx insert-molecules [-f [<.gro/.g96/...>]] [-ci [<.gro/.g96/...>]]
    [-ip [<.dat>]] [-o [<.gro/.g96/...>]] [-nice ]
    [-box ] [-nmol ] [-try ] [-seed ]
    [-radius ] [-scale ] [-dr ] [-rot ]
    [-[no]allpair]
```

 ## 参数
**输入/输出文件选项**
|参数|默认值|类型|说明|
|:----|:----|:----|:----|
|-f [<.gro/.g96/...>]|protein.gro|输入，可选|结构文件：gro g96 pdb brk ent esp tpr tpb tpa|
|-ci [<.gro/.g96/...>]|spc216.gro|输入|结构文件：gro g96 pdb brk ent esp tpr tpb tpa|
|-ip [<.dat>]|positions.dat|输入，可选|通用数据文件。|
|-o [<.gro/.g96/...>]|out.gro|输出|结构文件：gro g96 pdb brk ent esp|

**控制选项**
|参数|默认值|说明|
|:----|:----|:----|
|-nice \<int>|0|设置优先级。|
|-box \<vector>|0 0 0|盒子尺寸(nm)。|
|-nmol \<int>|0|要插入的分子的数目。|
|-try \<int>|10|尝试插入-nmol乘以-try次。|
|-seed \<int>|1997|随机数发生器的种子。|
|-radius \<real>|0.105|默认的范德华距离。|
|-scale \<real>|0.57|用于数据文件share/gromacs/top/vdwradii.dat中范德华半径的缩放因子。对水中的蛋白质，使用默认值0.57可以得到接近1000 g/l的密度值。|
|-dr \<vector>|0 0 0|相对-ip文件中的位置，在x/y/z方向允许的最大偏离位移。|
|-rot \<enum>|xyz|随机旋转插入分子，可用选项：xyz，z或none。|
|-[no]allpair|no|与-ci选项同用时避免近邻搜索过程中的内存泄露. 对大的体系可能会比较慢。|

## 常规使用方法
（1）**gmx insert-molecules命令可以插入-nmol个体系的副本到盒子中，体系由-ci输入文件定义。** 插入的分子可以填充由-f指定的溶质分子构型中的空位，或者填充由-box指定的空盒子。同时指定-f和-box选项等同于-f，但插入前会在溶质周围放置一个新盒子。该命令运行过程中坐标文件中的速度不予考虑。

（2）**默认情况下，插入的位置是随机的**(初始随机数种子由-seed设置)。程序将会迭代直至将-nmol个分子插入盒子中。对某一位置若已存在的任意原子和插入分子任意原子之间的距离小于两个原子范德华半径之和，则不会插入分子。程序会读取数据文件(vdwradii.dat)中的范德华半径，并根据-scale选项的设置进行缩放。若不能在数据文件中找到所需的半径值，相应的原子将通过-radius来设定(未缩放)距离。

（3）**停止前共进行-nmol*-try次尝试插入。** 若存在一些小的空隙需要填充，可以增加-try的值。-rot选项用于指定在尝试插入前是否对插入分子进行随机旋转。

（4）作为替代，也可以仅将分子插入到positions.dat(-ip)文件中指定的特定位置。此文件应包含三列信息(x,y,z)，它们给出了相对于输入分子位置(-ci)的偏离位移。因此如果该文件应包含绝对位置，使用gmx insert-molecules命令前必须把分子的中心置于(0, 0, 0)(例如使用gmx editconf -center)。该文件中以#开始的内容为注释会被忽略。-dr选项定义了插入尝试中允许的最大位移。-try和-rot以默认模式运行。

