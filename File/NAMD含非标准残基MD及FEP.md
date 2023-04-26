👏 NAMD|含非标准残基MD及FEP

---
[TOC]

---
[Parameterizing a Novel Residue (官方教程)](NAMD含非标准残基MD及FEP/forcefield.pdf)

## D-AA MD
（1）使用常规的蛋白准备流程构建体系。
（2）将system.pdb中的残基进行pymol编辑使其变成D构型。
（2）加载相应的力场文件toppar_all36_prot_modify_res.str，并使用mutate命令进行突变即可。
（3）将pymol中改变的原子的坐标对应的写入system.pdb中。
（4）pymol打开检查。
（5）MD，看是否会报错。看了，没有报错并且结构合理。