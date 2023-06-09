👏 Gromacs|gmx trjcat

---
[TOC]

---
## 简介
连接轨迹文件。

## 格式
```text
gmx trjcat [-f [<.xtc/.trr/...> [...]]] [-o [<.xtc/.trr/...> [...]]]
    [-n [<.ndx>]] [-demux [<.xvg>]] [-nice ] [-tu ]
    [-xvg ] [-b ] [-e ] [-dt ] [-[no]vel]
    [-[no]settime] [-[no]sort] [-[no]keeplast] [-[no]overwrite]
    [-[no]cat]
```

## 参数
**输入/输出文件选项**
|参数|默认值|类型|说明|
|:----|:----|:----|:----|
|-f [<.xtc/.trr/...> [...]]|traj.xtc|输入|轨迹：xtc trr cpt trj gro g96 pdb tng|
|-o [<.xtc/.trr/...> [...]]|trajout.xtc|输出|轨迹：xtc trr trj gro g96 pdb tng|
|-n [<.ndx>]|index.ndx|输入，可选|索引文件。|
|-demux [<.xvg>]|remd.xvg|输入，可选|xvgr/xmgr文件。|
 
**控制选项**
|参数|默认值|说明|
|:----|:----|:----|
|-nice \<int>|19|设置优先级。|
|-tu \<enum>|ps|时间单位：fs，ps，ns，us，ms，s。|
|-xvg \<enum>|xmgrace|xvg绘图格式：xmgrace，xmgr，none|
|-b|0|从轨迹文件中读取的第一帧(ps)。|
|-e|0|从轨迹文件中读取的最后一帧(ps)。|
|-dt|0|只使用t除以dt的余数等于第一帧时间(ps)的帧，即两帧之间的时间间隔。|
|-[no]vel|yes|如果可能读入并输出速度。|
|-[no]settime|no|交互式地设定每一输入文件在新输出文件中的起始时间。|
|-[no]sort|yes|自动排序输入轨迹文件(而不是帧)。|
|-[no]keeplast|no|将重复帧输出值轨迹末尾。|
|-[no]overwrite|no|追加时覆盖重复帧。|
|-[no]cat|no|不丢弃重复帧。|

## 常规使用方法
（1）gmx trjcat可按顺序将几个输入轨迹文件合并在一起。**当发现同一时刻存在两帧时，会使用后一文件中的帧。** 通过使用-settime，你可以指定每一轨迹文件的起始时间。输入文件由命令行得到，你可能要使用像gmx trjcat -f *.trr -o fixed.trr这样的技巧。**使用-cat选项你可以简单地将几个文件粘帖在一起而不会移除具有系统时间戳的帧。**

（2）当输出文件为输入文件之一时，需要特别注意一点。在这种情况下那个特定的输入文件会被追加，这样就不需要存储双倍的数据。显然要追加的文件必须具有最小的起始时间，因为只能在文件末尾追加。

（3）当给定-demux选项时，会读入N个轨迹，并将它们按.xvg文件中指定的顺序写入另一个文件中。.xvg文件的内容类似于:
```text
0 0 1 2 3 4 5
2 1 0 2 3 5 4
```
其中第一个数字为时间，接下来的数字为轨迹编号，对应于第一行数字的帧会写入输出轨迹中。如果轨迹中帧的数目与.xvg文件中的不匹配，程序会自行决定如何处理，请小心。

## 源码位置及解析
gromacs-2022.2/src/gromacs/tools/trjcat.cpp

## 示例
（1）合并两个文件中时间上不连续的文件。该方法合并出来的文件轨迹并在了一起，但是时间戳是不连续的。
```shell
gmx trjcat -f prod.xtc prod1.xtc -o cluster8_20ns.xtc -cat
```

（2）轨迹连续，时间戳也能连续的合并方法。
```python
# c -> continue
echo -ne "c\nc\nc\n"|gmx trjcat -f prod.xtc prod1.xtc prod2.xtc -settime -o test.xtc
```