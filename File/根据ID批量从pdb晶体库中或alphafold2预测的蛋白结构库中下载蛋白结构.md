👏 根据ID批量从pdb晶体库中或alphafold2预测的蛋白结构库中下载蛋白结构

---

[封面PPT](../PPT/根据ID批量从pdb晶体库中或alphafold2预测的蛋白结构库中下载蛋白结构.pptx)

---

[TOC]

---

## 1. 根据 PDB ID 批量从pdb蛋白晶体结构库 [RCSB](https://link.zhihu.com/?target=https%3A//www.pdbus.org/) 中下载蛋白pdb结构

首先涉及到批量的问题，就需要查看下载pdb的网址的规律，可以看出下载特定 PDB ID 的蛋白结构的网址格式是 [http://www.rcsb.org/pdb/files/](https://link.zhihu.com/?target=http%3A//www.rcsb.org/pdb/files/)${PDB ID}.pdb，例如PPT中图片所示：http://www.rcsb.org/pdb/files/1A2B.pdb

找到规律就很好办了，可以看出特定 PDB ID 的蛋白结构的下载网址也基本是固定的，唯一的变量就是网址中的 PDB ID。所以我们可以根据 PDB ID 使用 wget 批量下载 RCSB 晶体结构库中的蛋白结构。 具体操作如下：

(1) 准备一个你需要下载的含有 PDB ID 的 list 文件，一行一个 PDB ID。如PPT中图所示：

(2) 执行下述命令进行批量下载。

```bash
for i in `cat list`;do mkdir ${i};wget -q -N -O ./${i}/${i}.pdb http://www.rcsb.org/pdb/files/${i}.pdb;done
```

## 2. 根据 UniProt ID 批量从 alphafold2 预测的蛋白结构库 [AlphaFold](https://link.zhihu.com/?target=https%3A//alphafold.ebi.ac.uk/) [Protein Structure Database](https://link.zhihu.com/?target=https%3A//alphafold.ebi.ac.uk/) 中下载蛋白pdb结构

同样首先分析下载 alphafold2 预测的蛋白结构的网址的规律，可以看出下载特定 UniProt ID 的预测结构的网址格式是 [https://alphafold.ebi.ac.uk/files/AF-](https://link.zhihu.com/?target=https%3A//alphafold.ebi.ac.uk/files/AF-)${UniProt ID}-F1-model_v1.pdb，例如：https://alphafold.ebi.ac.uk/files/AF-P61586-F1-model_v1.pdb。

批量下载具体操作如下：

(1) 准备一个你需要下载的含有 UniProt ID 的 list 文件，一行一个 UniProt ID。如PPT中图所示：

(2) 执行下述命令进行批量下载。

```bash
for i in `cat list`;do mkdir ${i};wget -q -N -O ./${i}/${i}.pdb https://alphafold.ebi.ac.uk
```