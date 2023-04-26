👏 Gromacs|使用Gromacs没有自带的新力场

---
[TOC]

---
## 使用Amber14sb力场
（1）Gromacs自带的所有力场文件位于：\$GMX/share/gromacs/top/     
（2）若要添加上述目录中没有的力场14sb，则可下载14sb的压缩包，解压后放至$GMX/share/gromacs/top/即可。
（3）也可不放在公共文件夹中，则需要将14sb力场文件位置变量添加到GMXLIB中。具体做法则是在~/.bashrc中加入如下所示命令：
```shell
export GMXLIB=/public/home/yqyang/software/forcefield/
```
（4）除了上述两种方法外，也可以将力场文件放在工作目录下，运行下述命令即可：
```shell
gmx pdb2gmx -f cmp.pdb -ff amber19sb -water tip3p
```