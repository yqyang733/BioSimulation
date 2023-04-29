👏 MDFF用于将晶体结构匹配电子密度

---
[TOC]

---
官网教程：https://www.ks.uiuc.edu/Research/mdff/documentation.html  
[tutorial_mdff.pdf](vmdMDFF插件用于将晶体结构匹配电子密度/tutorial_mdff.pdf)  

## 跑教程
### A simple MDFF example
文件：  
初始结构：[1ake-initial.pdb](vmdMDFF插件用于将晶体结构匹配电子密度/1ake-initial.pdb)  
目标结构：[4ake-target.pdb](vmdMDFF插件用于将晶体结构匹配电子密度/4ake-target.pdb)   

（1）根据目标结构产生目标电子密度图。  
（1.1）psfgen产生pdb和psf文件。   
```text
package require psfgen
psfcontext reset

topology /public/home/yqyang/file/NAMD_config_file/top_all36_prot.rtf
topology /public/home/yqyang/file/NAMD_config_file/toppar_water_ions.str
pdbalias residue HIS HSE
pdbalias atom ILE CD1 CD     
pdbalias atom SER HG HG1         
pdbalias atom CYS HG HG1   

segment PRO {
    first none
    last none
    pdb 4ake-target.pdb
    }
coordpdb 4ake-target.pdb PRO
guesscoord

writepdb 4ake-target_psfgen.pdb
writepsf 4ake-target_psfgen.psf

exit
```
```shell
/public/home/yqyang/software/vmd-1.9.4a55-installed/bin/vmd -dispdev text -e mk_build.tcl
```

（1.2）产生4ake-target_psfgen.pdb的电子密度  
```text
mol load psf 4ake-target_psfgen.psf pdb 4ake-target_psfgen.pdb
set sel [atomselect top all]
mdff sim $sel -res 2.5 -o 4ake-target.dx

quit
```
```shell
/public/home/yqyang/software/vmd-1.9.4a55-installed/bin/vmd -dispdev text -e tcl
```

（2）将电子密度图转换成MDFF势能  
```shell
package require mdff
mdff griddx -i 4ake-target.dx -o 4ake-target_grid.dx

quit
```

（3）将蛋白结构刚性对接进入电子密度图中  
```shell
mol new 1ake-initial.pdb
set sel [atomselect top "protein and noh"]
voltool fit $sel -res 2.5 -i 4ake-target.dx
$sel writepdb 1ake-initial-docked.pdb

quit
```

（4）初始结构准备  
```text
package require psfgen
psfcontext reset

topology /public/home/yqyang/file/NAMD_config_file/top_all36_prot.rtf
topology /public/home/yqyang/file/NAMD_config_file/toppar_water_ions.str
pdbalias residue HIS HSE
pdbalias atom ILE CD1 CD     
pdbalias atom SER HG HG1         
pdbalias atom CYS HG HG1   

segment PRO {
    first none
    last none
    pdb 1ake-initial-docked.pdb
    }
coordpdb 1ake-initial-docked.pdb PRO
guesscoord

writepdb 1ake-initial-docked_psfgen.pdb
writepsf 1ake-initial-docked_psfgen.psf

exit
```

```shell
package require mdff
mdff gridpdb -psf 1ake-initial-docked_psfgen.psf -pdb 1ake-initial-docked_psfgen.pdb -o 1ake-initial-docked_psfgen-grid.pdb

quit
```

（5）定义二级结构限制  
```shell
package require ssrestraints
ssrestraints -psf 1ake-initial-docked_psfgen.psf -pdb 1ake-initial-docked_psfgen.pdb -o 1ake-extrabonds.txt -hbonds
mol new 1ake-initial-docked_psfgen.psf
mol addfile 1ake-initial-docked_psfgen.pdb
package require cispeptide
cispeptide restrain -o 1ake-extrabonds-cispeptide.txt
package require chirality
chirality restrain -o 1ake-extrabonds-chirality.txt

quit
```

（6）NAMD进行MDFF模拟  
```shell
package require mdff
mdff setup -o adk -psf 1ake-initial-docked_psfgen.psf -pdb 1ake-initial-docked_psfgen.pdb -griddx 4ake-target_grid.dx -gridpdb 1ake-initial-docked_psfgen-grid.pdb -extrab {1ake-extrabonds.txt 1ake-extrabonds-cispeptide.txt 1ake-extrabonds-chirality.txt} -gscale 0.3 -numsteps 500000
mdff setup -o adk -psf 1ake-initial-docked_psfgen.psf -pdb 1ake-initial-docked_psfgen.pdb -griddx 4ake-target_grid.dx -gridpdb 1ake-initial-docked_psfgen-grid.pdb -extrab {1ake-extrabonds.txt 1ake-extrabonds-cispeptide.txt 1ake-extrabonds-chirality.txt} -gscale 10 -minsteps 2000 -numsteps 0 -step 2

quit
```