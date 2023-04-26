👏 Gromacs|charmm36力场下蛋白RNA复合物分子动力学模拟

---
[TOC]

---
**与14sb的md流程非常近似，细节稍有差别。详细可参考14sb流程。**
## 体系构建前准备
（1）获得蛋白RNA复合物pdb文件。
（2）Charmm36是gmx自带的力场，选择即可。

## 体系构建
**使用pdb2gmx生成拓扑文件时可能会出现碱基摸板不匹配的错误**
具体参考文件：$GMX/share/gromacs/top/charmm36-jul2021.ff/na.rtp。该文件中提供了碱基的摸板，pdb文件中的碱基应与该摸板中的碱基匹配才能正常构建体系。主要有以下两点需要注意：
* 在使用pdb2gmx时注意-ter选项的应用可解决一部分问题。
* 注意摸板文件中原子的写法要一致，需要特别注意O1P和O2P。

（1）生成拓扑文件
（2）加水盒子
（3）加入中和离子。**注意加入中和离子时charmm36力场和14sb力场离子的名称是不一样的，所以需要调整。** 离子名称的模板具体参考文件：$GMX/share/gromacs/top/charmm36-jul2021.ff/ions.itp。charmm36力场中钠离子的名称是SOD，氯离子的名称是CLA，水的名称是SOL。所以加入离子中和的命令修改为如下：
```shell
gmx genion -s ions.tpr -o solv_ions.gro -p topol.top -pname SOD -nname CLA -neutral -conc 0.15
```

## em、nvt、npt、md
**与14sb构建的模型md流程一样，不过该方法自带的index.ndx中没有Water_and_ions，需要构建新的index.ndx创建Water_and_ions项。**
需要修改nvt.mdp、npt.mdp和prod.mdp文件中的以下内容：
```text
tc_grps    =  Protein RNA Water_and_ions
```