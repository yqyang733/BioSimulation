👏 vmd|图形化界面操作教程

---
[TOC]

---
## Graphics
### Representations
**（1）选区语法：**
|名称|语法|
|:----|:----|
|蛋白|protein|
|某条链|chain A|
|某条链的某个残基|chain A && resid 52|
|某个残基|resid 52|
|某一段残基|resid 1 to 52|
|水|water|
|同时显示蛋白和核酸|protein or nucleic|
|α碳原子|name CA|
|ALA上的α碳原子|name CA and resname ALA|
|蛋白骨架原子|backbone|
|非蛋白原子|not protein|
|所有碳原子|name "C.*"|
|质量小于5的原子|mass < 5|
|键连其他两个原子的原子|numbonds = 2|
||abs(charge) > 1|
||z < 6 and z > 3|
|配体的非氢原子|(segname LIG) && noh|
|配体周围一定范围内的所有蛋白原子|protein within 5 of resname LIG; within 5 of name FE; protein within 5 of nucleic; water within 3 of protein|
|配体周围一定范围内的所有蛋白残基|same residue as (protein within 5 of resname LIG)|

**within and same**
within <number> of <selection> selects all atoms within the specified distance (in Å) from a selection, including the selection itself.
```tcl
within 5 of name FE
protein within 5 of nucleic
```

same <keyword> as <selection> finds all the atoms which have the same `keyword' as the atoms in the selection.
```tcl
same fragment as resid 35
same resname as (protein within 5 of nucleic)
```

**（2）着色方式**


## Mouse
**快捷键**
|快捷键|操作|
|:----|:----|
|R|旋转|
|T|平移|
|C|选择旋转中心|
|S|缩放|

## 其他
（1）VMD初始化文件(vmd.rc)我的推荐设置
参考：http://sobereva.com/545

## 参考资料
1. [VMD Molecular Graphics](https://warwick.ac.uk/fac/sci/moac/degreescopy/degrees-copy/modules/ch926/workshop/vmd-tutorial.pdf)  [PDF](./vmd图形化界面操作教程/vmd-tutorial.pdf)
2. http://www.ks.uiuc.edu/Research/vmd/vmd_help.html
3. http://www.ks.uiuc.edu/Research/vmd/script_library/
4. https://psb.stanford.edu/psb-online/proceedings/psb97/dalke.pdf
5. https://chemistry.stackexchange.com/questions/155781/how-do-i-find-an-atom-in-vmd-by-using-its-index
6. https://www.ks.uiuc.edu/Research/vmd/vmd-1.3/ug/node132.html
7. [Advanced VMD:Trajectories, movies, scripting](./vmd图形化界面操作教程/vmd20201028.pdf)