👏 Gromacs|动力学模拟轨迹可视化

---
[TOC]

---
## 将蛋白RNA模拟轨迹可视化
```shell
# 先使用npt.gro去水去离子建立new.pdb，然后使用new.pdb建立index_new.ndx。
gmx editconf -f npt.gro -c -o new.pdb -n index.ndx
gmx make_ndx -f new.pdb -o index_new.ndx
gmx trjconv -f t1.part0001.xtc -s new.pdb -o md_pbcwhole_new.xtc -pbc whole -tu ns -dt 1 -n index_new.ndx 
gmx trjconv -f md_pbcwhole_new.xtc -s new.pdb -o md_pbcnojump_new.xtc -pbc nojump -tu ns -dt 1 -n index_new.ndx
gmx trjconv -f md_pbcnojump_new.xtc -s new.pdb -o md_pbcmol_new.xtc -pbc mol -ur compact -center -tu ns -dt 1 -n index_new.ndx 
gmx trjconv -f md_pbcmol_new.xtc -s new.pdb -o md_pbcfit_all_new.xtc -fit rot+trans  -tu ns -dt 1 -n index_new.ndx
```