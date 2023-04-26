👏 Gromacs|CHARMM/cgenff力场下蛋白配体复合物分子动力学模拟

---
[TOC]

---
## 产生Gromacs所需要的配体相关文件
1. 将配体(mol2格式三维结构)进行加氢(pymol)，检查加氢的正确性。注意保证分子中每个原子名称的唯一性。[I3C-3.mol2](GromacsCHARMMcgenff力场下蛋白配体复合物分子动力学模拟/I3C-3.mol2) 将分子的名称也修改一下，最好以三个字母进行命名。不要多于4个字母。
2. 上传到CGenFF网站并下载lig.str文件。[lig.str](GromacsCHARMMcgenff力场下蛋白配体复合物分子动力学模拟/lig.str)
3. 运行cgenff_charmm2gmx_py3_nx2.py脚本文件进行格式转换。[cgenff_charmm2gmx_py3_nx2.py](GromacsCHARMMcgenff力场下蛋白配体复合物分子动力学模拟/cgenff_charmm2gmx_py3_nx2.py)
```python
python cgenff_charmm2gmx_py3_nx2.py obj01 I3C-3.mol2 lig.str charmm36_ljpme-jul2022.ff   # obj01是lig.str中RESI后面跟的名称。需要下载对应版本的力场文件charmm36_ljpme-jul2022.ff。
```
4. 运行成功即可生成下述四个文件：
[obj01.top](GromacsCHARMMcgenff力场下蛋白配体复合物分子动力学模拟/obj01.top)
[obj01.itp](GromacsCHARMMcgenff力场下蛋白配体复合物分子动力学模拟/obj01.itp)
[obj01.prm](GromacsCHARMMcgenff力场下蛋白配体复合物分子动力学模拟/obj01.prm)
[obj01_ini.pdb](GromacsCHARMMcgenff力场下蛋白配体复合物分子动力学模拟/obj01_ini.pdb)


## Temp
流程：
（1）pdb2gmx生成蛋白的pdb文件和top文件。
（2）将obj01_ini.pdb增加到蛋白pdb文件的末尾，将obj01.prm和obj01.itp  include在top文件中。
（3）进行后续的加水加离子等操作。