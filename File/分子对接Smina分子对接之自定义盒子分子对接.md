👏 分子对接|Smina分子对接之自定义盒子分子对接

---
[TOC]

---
## 确定对接盒子
(1) 采用口袋计算软件预测蛋白口袋确定对接盒子，根据口袋坐标生成盒子的中心以及长宽高。参考“分子对接|根据D3Pockets预测的蛋白口袋生成Smina对接的盒子参数文件”。   
(2) 根据配体坐标生成盒子的中心以及长宽高。  
(3) 根据蛋白坐标生成盒子的中心以及长宽高。 
(4) 将盒子文件保存成如下的conf.txt的格式。   
> center_x = 19  
> center_y = -13  
> center_z = 36  
> size_x = 63  
> size_y = 115  
> size_z = 143  
> num_modes = 10  
> exhaustiveness = 32  

---
## 对接盒子可视化
参考“Pymol脚本|分子对接盒子可视化”   

---
## 配体准备
(1) 首先生成小分子的3维结构mol2文件。以下方式选择其一即可。  
- ChemBio3D，MMFF94力场优化。  
- OpenBabel生成，
- RDKit生成，MMFF94力场优化。  

(2) 采用mgltools/AudockTools，给配体加氢加电荷，合并非极性氢的电荷到键连的非极性碳原子上，删去非极性氢，保留极性氢，生成配体的pdbqt格式文件（输入文件为mol2）。  
- Linux上Conda安装mgltools软件包
```shell
prepare_ligand4.py -l ${ligand}.mol2 -o ${ligand}.pdbqt
```
- Windows上安装mgltools软件，在图形界面中输入mol2文件，输出保存pdbqt文件

## 受体准备
(1) 用pdb2pqr给蛋白质（pdb文件）加氢加电荷，生成pqr文件，再用mgltools合并非极性氢，生成受体的pdbqt文件。  
(2) Conda安装pdb2pqr软件包，将pdb文件生成pqr文件。   
```shell
pdb2pqr30 --ff=amber --ffout=amber --chain A --with-ph=7 ${receptor}.pdb ${receptor}.pqr
```
(3) mgltools合并非极性氢，生成受体的pdbqt文件。   
- Linux上Conda安装mgltools软件包
```shell
prepare_receptor4.py -r ${receptor}.pqr -o ${receptor}.pdbqt -C -U nphs_lps_waters_nonstdres
```
- Windows上安装mgltools软件，在图形界面中输入pdb文件，输出保存pdbqt文件

## Smina对接
```shell
smina --seed 0 --config conf.txt -r ${receptor}.pdbqt -l ${ligand}.pdbqt -o  ${receptor}-${ligand}-result.sdf
```