👏 parmed将Amber文件转换为Gromacs或CHARMM文件

---
[TOC]

---
## prmtop&inpcrd to gro&top
```python
import parmed as pmd
parm = pmd.load_file('name-of-your.prmtop', 'name-of-your.inpcrd')
parm.save('gromacs.top', format='gromacs')
parm.save('gromacs.gro')
```

## prmtop&inpcrd to charmm
```python
import parmed as pmd
parm = pmd.load_file('name-of-your.prmtop', 'name-of-your.inpcrd')
parm.save('charmm.psf')
parm.save('charmm.crd')
```