👏 Gromacs|gmx genion

---
[TOC]

---
## 简介
在能量有利位置加入单原子离子。

## 格式
```text
gmx genion [-s [<.tpr/.tpb/...>]] [-n [<.ndx>]] [-o [<.gro/.g96/...>]]
    [-p [<.top>]] [-nice ] [-np ] [-pname ]
    [-pq ] [-nn ] [-nname ] [-nq ]
    [-rmin ] [-seed ] [-conc ] [-[no]neutral]
```

## 参数
**输入/输出文件选项**
|参数|默认值|类型|说明|
|:----|:----|:----|:----|
|-s [<.tpr/.tpb/...>]|topol.tpr|输入|运行输入文件：tpr tpb tpa|
|-n [<.ndx>]|index.ndx|输入，可选|索引文件。|
|-o [<.gro/.g96/...>]|out.gro|输出|结构文件：gro g96 pdb brk ent esp   得到这个文件之后，可以再用它产生tpr文件。|
|-p [<.top>]|topol.top|输入/输出，可选|拓扑文件；在往体系中添加金属离子时，genion会往拓扑文件最后的分子类型中写入添加的离子数，并修改拓扑文件中体系的原子数。|

**控制选项**
|参数|默认值|说明|
|:----|:----|:----|
|-nice \<int>|0|设置优先级。|
|-np \<int>|0|阳离子的数目。|
|-pname \<string>|NA|阳离子的名称。|
|-pq \<int>|1|阳离子的电荷。|
|-nn \<int>|0|阴离子的数目。|
|-nname \<string>|CL|阴离子的名称。|
|-nq \<int>|-1|阴离子的电荷。|
|-rmin \<real>|0.6|离子间的最小距离。|
|-seed \<int>|1993|随机数发生器的种子。|
|-conc \<real>|0|指定盐的浓度(mol/L)。程序会添加足够多的离子以达到指定的浓度，浓度根据输入.tpr文件中的盒子体积计算。覆盖-np和-nn选项。|
|-[no]neutral|no|此选项会添加足够多的离子以使体系的净电荷为零。会优先添加这些离子，然后在添加那些由-np/-nn或-conc指定的离子。|

## 常规使用方法
（1）**gmx genion用单原子离子随机地取代溶剂分子。** 溶剂分子组应该连续，且所有分子的原子数应该相同。用户应该将离子添加到拓扑文件中，或使用-p选项自动修改拓扑文件。

（2）**在所有力场中，离子的分子类型，残基名称和原子名称都是大写的元素名称且不含符号。** 分子名称应使用-pname或-nname给出，并且拓扑文件的[ molecules ]段也要相应地更新，可以手动更新或使用-p选项。不要使用原子名称！

（3）具有多个电荷态的离子会添加多重度，不含符号，只用于非常见态。

（4）**对更大的离子，例如硫酸根，我们建议使用gmx insert-molecules。**

## 源码位置及解析
gromacs-2022.2/src/gromacs/gmxpreprocess/genion.cpp

## 示例
（1）加固定数目的离子。
```shell
gmx genion -s ions.tpr -o solv_ions_mg.gro -p topol.top -pname MG -np 5
```