👏 分子对接|Smina分子对接之以复合物中配体定位的分子对接

---
[TOC]

---
## 对接前准备
(1) 将复合物的的配体单独取出来保存成mol2文件用于提供对接位置的参考坐标。  
(2) 配体准备核受体准备参考“分子对接|Smina分子对接之自定义盒子分子对接”中准备步骤。  

---
## Smina对接
(1) 以复合物中的配体作为对接盒子中心，autobox_add指往各维度扩展多少距离（单位为埃，默认为4），ligand-old.pdb指晶体结合口袋中已有的配体文件，用于定位坐标。  
```shell
smina --seed 0 --autobox_ligand ${ligand-old}.pdb --autobox_add 8 -r ${receptor}.pdbqt -l ${ligand}.pdbqt -o ${receptor}-${ligand}-result.sdf  --exhaustiveness 32 --num_modes 1
```