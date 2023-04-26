👏 acpype将Amber文件转换为Gromacs文件

---
[TOC]

---
## acpype安装
```python
pip install acpype
```

## 将prmtop和inpcrd转换为gro,top和itp
```shell
acpype -p ligand.prmtop -x ligand.inpcrd
```