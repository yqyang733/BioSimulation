👏 Gromacs|续跑

---
[TOC]

---
## 意外情况续跑
有时会遇到死机、断电等意外情况导致动力学模拟中断，如果中断的任务是如下运行的：
```shell
gmx mdrun -deffnm md
```
动力学作业中断后可以继续使用mdrun接着续跑，使用以下命令：
```shell
gmx mdrun -s md.tpr -cpi md.cpt -deffnm md -dhdl dhdl -nb gpu -bonded gpu -pme gpu -gpu_id 0 -ntmpi 1 -ntomp 10
```
mdrun默认会将新产生的轨迹添加到原始文件末尾，最终文件会包括续跑前与续跑后的所有内容。

## 延长时间续跑
### 续跑生成新的文件
（1）完整的分子动力学作业已经完成，如果我们需要延续之前的参数再续跑一段时间，**如10ns并单独生成文件，** 可通过以下命令来实现：
```shell
gmx convert-tpr -s md1.tpr -extend 10000 -o md2.tpr
gmx mdrun -v deffnm md2 -cpi md1.cpt -noappend
```
> 其中"md1.tpr"为之前动力学产生的轨迹文件，使用convert-tpr重新生成新的“md2.tpr”，使用“-extend”来指定延长动力学模拟的时长。  

计算完成后，会单独生成新的文件md2.part0002.xtc/log/edr/gro/cpt。

（2）完整的分子动力学作业已经完成，如果我们需要延续之前的参数再续跑一段时间，**如10ns并且直接在之前md1的输出文件上续写。** 可以使用convert-tpr直接修改原始的"md1.tpr"。输入以下命令来实现：
```shell
gmx convert-tpr -s md1.tpr -extend 10000 -o md1.tpr
gmx mdrun -v deffnm md1 -cpi md1.cpt
```

（3）有时我们需要**提取某帧构象及速度进行续跑**，即从上一步动力学模拟中提取某一系统状态进行新的动力学模拟。**这时可以使用grompp生成一个新的.tpr文件，首先要在.mdp文件中设置gen-vel为no，以使grompp不再为系统中的原子指定随机速度；修改tinit参数，指定新的模拟开始的时间**，如提取20ns时的构象和速度续跑，就设置“tinit=20000”。运行以下命令进行续跑：
```shell
gmx grompp -f grompp.mdp -p topol.top -c state.cpt -time 20000 -e ener.edr -o new.tpr
gmx mdrun -s new.tpr -cpi state.cpt
```
> 其中-f：指定.mdp文件；-p：指定拓扑文件；-c：上一步模拟的系统坐标文件；-time：坐标文件对应的模拟时间，需和系统的坐标文件一致；-e：上一步的能量文件（可选）；-o：输出tpr文件

（4）生成新的文件进行续跑。
```shell
gmx grompp -f ${mdp_path}/prod.mdp -c ../prod/prod.gro -t ../prod/prod.cpt -p ${JOBPATH}/${CASE}/topol.top -o prod1.tpr -r ../prod/prod.gro -maxwarn 4
gmx mdrun -s prod1.tpr -deffnm prod1 -dhdl dhdl -ntmpi 1 -ntomp $NP_1 -nb gpu -bonded gpu -pme gpu -gpu_id 0
```

## 删除原子后续跑
（1）找到续跑前轨迹的最后一帧或你想续跑的任意帧的时间。
（2）复合物居中。
```shell
echo 1 0|gmx trjconv -f prod1.xtc -s prod1.tpr -o prod1_out.xtc -pbc mol -center
```
（3）生成新的gro文件。
```shell
gmx trjconv -s prod1.tpr -f prod1_out.xtc -o prod1_last.gro -b 150000 -e 150000
```
（4）.gro文本删除原子所在行并且修改头部的原子数目。
（5）切记电荷平衡，删除阳离子和阴离子后要记得删除相反电荷离子保证电荷平衡。
（5）topol.top文件中修改对应体系的原子数目。
（6）em，nvt，npt，prod即可。

## 参考资料
1. https://mp.weixin.qq.com/s/6smzfRf9t9FM-DYGuS6o9Q