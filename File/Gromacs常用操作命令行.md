👏 Gromacs|常用操作命令行

---
[TOC]

---
## 制作索引
（1）制作Protein_RNA组索引。
```shell
gmx_mpi make_ndx -f npt.gro -o index.ndx
# "1|12"
```

## 格式转换
（1）将gro文件去水去离子居中保存为pdb文件。
```shell
echo 22 22|gmx_mpi editconf -f prod.gro -c -o prod.pdb -n index.ndx
```

## 距离测量
（1）测量gro文件两组残基质心间距离。
```shell
gmx_mpi distance -s prod.tpr -f prod.gro -select "com of resnr 1 to 52 plus com of resnr 333 to 527" -oall
gmx_mpi distance -s prod.tpr -f prod.gro -select "com of resnr 3 to 7 45 to 49 plus com of resnr 333 to 527" -oall
```

## 周期性边界处理
（1）以原子为中心对结构居中。
```shell
gmx trjconv -s prod1.tpr -f cluster8_20ns.xtc -n index.ndx -o cluster8_20ns_nowat_8.xtc -pbc atom -center
```

## 合并轨迹
（1）合并两个文件中时间上不连续的文件。
```shell
gmx trjcat -f prod.xtc prod1.xtc -o cluster8_20ns.xtc -cat
```