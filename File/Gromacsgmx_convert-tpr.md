👏 Gromacs|gmx convert-tpr

---
[TOC]

---
## 简介
生成修改的运行输出文件。

## 格式
```text
gmx convert-tpr [-s [<.tpr/.tpb/...>]] [-f [<.trr/.cpt/...>]] [-e [<.edr>]]
    [-n [<.ndx>]] [-o [<.tpr/.tpb/...>]] [-nice ]
    [-extend ] [-until ] [-nsteps ] [-time ]
    [-[no]zeroq] [-[no]vel] [-[no]cont] [-init_fep_state ]
```

## 参数
**输入/输出文件选项**
|参数|默认值|类型|说明|
|:----|:----|:----|:----|
|-s [<.tpr/.tpb/...>]|topol.tpr|输入|运行输入文件：tpr tpb tpa|
|-f [<.trr/.cpt/...>]|traj.trr|输入，可选|全精度轨迹: trr cpt trj tng|
|-e [<.edr>]|ener.edr|输入，可选|能量文件。|
|-n [<.ndx>]|index.ndx|输入，可选|索引文件。|
|-o [<.tpr/.tpb/...>]|tpxout.tpr|输出|运行输入文件：tpr tpb tpa。|

**控制选项**
|参数|默认值|说明|
|:----|:----|:----|
|-nice \<int>|0|设置优先级。|
|-extend \<real>|0|通过此值来延长运行时间(单位: ps)。|
|-until \<real>|0|延长运行时间直到此时间结束(单位: ps)。|
|-nsteps \<int>|0|修改模拟运行步数。|
|-time \<real>|-1|由此时间(单位: ps)帧继续运行，而不是从最后一帧继续运行。|
|-[no]zeroq|no|将一个组(来自索引)的电荷设置为零。|
|-[no]vel|yes|需要来自轨迹的速度。|
|-[no]cont|yes|为了精确的延续，在第一步之前不应施加约束。|
|-init_fep_state \<int>|0|由此状态开始初始化fep状态。|

## 常规使用方法
（1）gmx convert-tpr可以四种方式来编辑运行输入文件：
* **修改运行输入文件中的模拟步数，** 可使用选项-extend，-until或-nsteps(nsteps=-1表示步数不受限制)。

* （已废弃）因磁盘已满等原因而导致模拟崩溃时，产生一个运行输入文件以继续模拟或创建一个继续的运行输入文件。此选项已废弃，因为mdrun现在可以读写检查点文件。**注意运行需要带有坐标和速率的帧。** 当使用压力和/或Nose-Hoover温度耦合时，还需要提供能量文件以得到原始运行的精确继续。

* **对原始tpx文件一部分创建一个.tpx文件。** 当你想从.tpx文件中移除溶剂，或想产生一个例如只包含C~α~的.tpx文件时，此功能很有用。需要注意的是你可能需要使用-nsteps -1(或类似的选项)。警告：此.tpx文件功能不全。

* **将指定组的电荷设置为零。** 当使用LIE(Linear Interaction Energy, 线性相互作用能)方法估算自由能时，此功能很有用。

## 源码位置及解析
gromacs-2022.2/src/gromacs/tools/convert_tpr.cpp

## 示例
