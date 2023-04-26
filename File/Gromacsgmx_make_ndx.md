👏 Gromacs|gmx make_ndx

---
[TOC]

---
## 简介
制作索引文件。

## 格式
```text
gmx make_ndx [-f [<.gro/.g96/...>]] [-n [<.ndx> [...]]] [-o [<.ndx>]]
    [-nice ] [-natoms ] [-[no]twin]
```

## 参数
**输入/输出文件选项**
|参数|默认值|类型|说明|
|:----|:----|:----|:----|
|-f [<.gro/.g96/...>]|conf.gro|输入，可选|结构文件：gro g96 pdb brk ent esp tpr tpb tpa|
|-n [<.ndx> [...]]|index.ndx|输入，可选|索引文件。|
|-o [<.ndx>]|index.ndx|输出|索引文件。|

**控制选项**
|参数|默认值|说明|
|:----|:----|:----|
|-nice \<int>|0|设置优先级。|
|-natoms \<int>|0|设置原子数(默认从坐标或索引文件中读取)。|
|-[no]twin|no|复写所有索引组并进行-natoms的偏移。|

## 常规使用方法
（1）几乎每个GROMACS程序都需要使用索引组。所有程序都可以生成默认的索引组，**只有需要特殊索引组的时候，你才不得不使用gmx make_ndx。** 一般情况下，整个体系会有一个默认组，蛋白质会有九个默认组，每个其他的残基会有一个默认组。

（2）当没有提供索引文件时，gmx make_ndx也会生成这些默认组。借助命令中的索引编辑器可以选择原子，残基或链的名称和数目。如果提供了运行输入文件也可以选择原子类型。可以使用NOT，AND或OR等逻辑判断词，你可以将索引组分成链，残基和原子。你也可以随意删除或重命名索引组。

（3）在索引编辑器和索引文件中，原子编号都是从1开始的。

（4）**选项-twin可以复写所有索引组，并对其施加-natoms的偏移。** 在设置计算电生理双层膜时，这个选项很有用。

（5）

## 示例
（1）制作Protein_RNA组索引。
```shell
gmx_mpi make_ndx -f npt.gro -o index.ndx
# "1|12"
```
（2）选择原子编号范围制作索引组。
```shell
gmx make_ndx -f npt.gro -o index.ndx
# a 1-9
# name 31 Chain_A # 蒋31组重命名为Chain_A
```
（3）选择链制作索引组。
```shell
gmx make_ndx -f npt.gro -o index.ndx
# chain A
# chain B
```