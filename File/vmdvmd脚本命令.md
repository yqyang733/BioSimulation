👏 vmd|vmd脚本命令

---
[TOC]

---
## File
### 保存输出图片
```shell
render TachyonInternal lllllll.tga -fullshade -aasamples 12 %s -res 6000 4000 -o %s.tga # 首先保存为tga格式图片
convert lllllll.tga lllllll.png  # convert将tga转成png图片
```

### 加载分子
```shell
mol new xxxx.pdb # download structure from RCSB according to PDB ID.
```

## Display
### 更改背景颜色
```shell
color Display {Background} white
```

## Extensions
### 运行tcl脚本
```shell
play test.tcl
```

## 周期性处理PBCTools
语法：
```tcl
pbc subcommand [options]...
```

```tcl
pbc wrap -centersel "protein" -center com -compound residue -all
animate write dcd my.dcd
pbc wrap -all -compound segid -center com -centersel "segname PRO"
```

## 加水盒子
（1）solvate
```shell
vmd > solvate
Info) Usage: solvate <psffile> <pdbfile> <option1?> <option2?>...
Info) Usage: solvate <option1?> <option2?>...  to just create a water box
Info) Options:
Info)     -o <output prefix> (data will be written to output.psf/output.pdb)
Info)     -s <segid prefix> (should be either one or two letters; default WT)
Info)     -b <boundary> (minimum distance between water and solute, default 2.4)
Info)     -minmax {{xmin ymin zmin} {xmax ymax zmax}}
Info)     -rotate (rotate molecule to minimize water volume)
Info)     -rotsel <selection> (selection of atoms to check for rotation)
Info)     -rotinc <increment> (degree increment for rotation)
Info)     -t <pad in all directions> (override with any of the following)
Info)     -x <pad negative x>
Info)     -y <pad negative y>
Info)     -z <pad negative z>
Info)     +x <pad positive x>
Info)     +y <pad positive y>
Info)     +z <pad positive z>
Info)     The following options allow the use of solvent other than water:
Info)       -spsf <solventpsf> (PSF file for nonstandard solvent)
Info)       -spdb <solventpdb> (PDB file for nonstandard solvent)
Info)       -stop <solventtop> (Topology file for nonstandard solvent)
Info)       -ws <size> (Box length for nonstandard solvent)
Info)       -ks <keyatom> (Atom occuring once per residue for nonstandard solvent)
```
（2）VMD默认在xyz轴上进行延申一定距离加水。
```shell
solvate merged.psf merged.pdb -t 11.5 -o solvated
```
（3）根据体系的长宽高坐标设置水盒子。
```shell
solvate merged.psf merged.pdb -minmax {{xmin ymin zmin} {xmax ymax zmax}} -o solvated
```

## 常用脚本
（1）打印选中残基的残基号码：
```tcl
set sel [atomselect top "(protein and noh and (same residue as within 6 of (chain P and resid 15))) && (name CA)"]
foreach resid [$sel get resid] chid [$sel get chain] {
    puts "$chid $resid"
}
```

## 参考资料
1. [VMD Tcl Text Interface](http://www.ks.uiuc.edu/Research/vmd/current/ug/node117.html)
2. https://www.bbsmax.com/A/amd0qEmWzg/