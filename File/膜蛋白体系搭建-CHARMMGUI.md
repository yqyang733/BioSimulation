👏 膜蛋白体系搭建-CHARMMGUI

---
[TOC]

---
## 一些常用的大膜
### 线粒体膜
配比：POPC:40%   POPE:30%   POPA:5%   POPS:5%   POCL1:20%
[10nm × 10nm](yqyang_1@10.77.14.27:/public/home/yqyang_1/work_zju/Others/Membrane/mitochondria/charmm-gui_1010.tgz)
[20nm × 20nm](yqyang_1@10.77.14.27:/public/home/yqyang_1/work_zju/Others/Membrane/mitochondria/charmm-gui_2020.tgz)

40 × 40 的大膜可由其他的小膜例如 20 × 20 拼接而来，具体如下：   
（1）在CHARMM-GUI上构建 20 × 20 的双层纯膜。
（2）将文件下载下来后，gromacs文件夹中有step5_input.pdb文件，根据此文件进行坐标扩展，并生成扩展后的pdb以及gro文件。其中盒子的x和y可在step5_input.gro的最后一行看到。   
```python
def extend_mem(pdb):

    with open(pdb) as f:
        f1 = f.readlines()
    
    name = str(pdb).replace(".pdb", "")
    extend_x = open(name+"_x.pdb", "w")
    extend_y = open(name+"_y.pdb", "w")
    extend_x_y = open(name+"_x_y.pdb", "w")
    for i in f1:
        if i.startswith("ATOM"):
            x = float(i[30:38])
            y = float(i[38:46])
            x_new = '{:8.3f}'.format(x+200.0805)
            y_new = '{:8.3f}'.format(y+200.0805)
            extend_x.write(i[:30]+x_new+i[38:])
            extend_y.write(i[:38]+y_new+i[46:])
            extend_x_y.write(i[:30]+x_new+y_new+i[46:])
        else:
            extend_x.write(i)
            extend_y.write(i)
            extend_x_y.write(i)

    extend_x.close()
    extend_y.close()
    extend_x_y.close()
```
```python
def pdb_for_gro():

    from collections import defaultdict
    resi_lines = defaultdict(list)

    with open("step5_input.pdb") as f:
        f1 = f.readlines()
    for i in f1:
        if i.startswith("ATOM"):
            resi_lines[i[17:21]].append(i)

    with open("step5_input_x.pdb") as f:
        f1 = f.readlines()
    for i in f1:
        if i.startswith("ATOM"):
            resi_lines[i[17:21]].append(i)

    with open("step5_input_y.pdb") as f:
        f1 = f.readlines()
    for i in f1:
        if i.startswith("ATOM"):
            resi_lines[i[17:21]].append(i)

    with open("step5_input_x_y.pdb") as f:
        f1 = f.readlines()
    for i in f1:
        if i.startswith("ATOM"):
            resi_lines[i[17:21]].append(i)

    rt = open("gro_input.pdb", "w")
    lst = ["POPE", "POCL", "POPC", "POPS", "POPA", "SOD ", "CLA ", "TIP3"]  # topol文件中的顺序
    for i in lst:
        for j in resi_lines[i]:
            rt.write(j)
    rt.close()
```
```shell
gmx editconf -f gro_input.pdb -o step5_input.gro -box 40.0161 40.0161 8.5
```
```shell
gmx make_ndx -f step5_input.gro -o index.ndx
```

（3）修改topol.top文件中各个脂分子的数目，将对应的分子数目乘以4。
（4）进行平衡。
[mdp](膜蛋白体系搭建-CHARMMGUI/mdp/)
[job.sh](膜蛋白体系搭建-CHARMMGUI/job.sh)
（5）将平衡完成的膜的gro文件转换成pdb文件拿去用就行。   
