👏 FEP|蛋白和多肽突变的FEP计算流程-Gromacs版

---
[TOC]

---
## 应用pmx进行准备双拓扑
**说明：bonded是gmx跑出来的MD轨迹中的一帧，free是NAMD跑出来的MD轨迹中的一帧。在保持盒子大小一致的情况下进行FEP计算。不保留水。**
**bonded状态准备**
（1）从pbc处理完成的轨迹中截出一帧结构。
（2）使用下述命令以及选择对应的力场生成pmx能够识别的输入结构。
```shell
gmx pdb2gmx -f $file -o bonded.pdb -water tip3p -ignh
```
（3）使用pmx mutate进行突变。
```shell
pmx mutate -f bonded.pdb -o mutant.pdb --keep_resid
```
（4）使用gmx pdb2gmx产生mutant.pdb的拓扑。要选择和之前相同的力场。
```shell
gmx pdb2gmx -f mutant.pdb -o conf.pdb -p topol.top -water tip3p
```
（5）使用pmx gentop产生双拓扑文件。
```shell
pmx gentop -p topol.top -o newtop.top
```
（6）常规的构建体系。
```shell
gmx editconf -f conf.pdb -o box.gro -bt cubic -d 0.5   # 需要记住盒子大小，free状态的盒子要和这个一致。
gmx solvate -cp box -cs spc216 -p newtop -o water.gro
gmx grompp -f /public/home/yqyang/file/gmx_file/ions.mdp -c water.gro -p newtop.top -o genion.tpr
gmx genion -s genion.tpr -p newtop.top -neutral -conc 0.15 -o ions.gro
gmx make_ndx -f ions.gro -o index.ndx
```
（7）生成mdp文件。
```shell
/public/home/yqyang/file/fep_zhangd/prepare_file.o -i /public/home/yqyang/file/fep_zhangd/mdp-Protein/ -l /public/home/yqyang/file/fep_zhangd/mdp-Protein/lambdas.dat -o ./
```
（8）运行脚本。
```shell
#PBS -q md
#PBS -l nodes=1:ppn=11:gpus=1
#PBS -S /bin/bash
#PBS -j oe
#PBS -N gmx
#PBS -l walltime=168:00:00

date
echo "CUDA_VISIBLE_DEVICES: $CUDA_VISIBLE_DEVICES"
NP=`cat $PBS_NODEFILE | wc -l`
NP_1=$((NP - 1))
NP_2=$((NP / 2))
echo "NP: $NP"
echo "NP_1: $NP_1"
echo "NP_2: $NP_2"
cd $PBS_O_WORKDIR
echo $PBS_O_WORKDIR
source /public/home/yqyang/software/gromacs-2021.5-installed/bin/GMXRC.bash
export LD_LIBRARY_PATH=/public/software/lib/:$LD_LIBRARY_PATH
source /public/software/compiler/intel/intel-compiler-2017.5.239/bin/compilervars.sh intel64

for a in `seq 1 5`
do
cd dup${a}
for i in `seq 0 29`
do
cd lambda${i}
#############################
mkdir em
cd em
if [ ! -f em.tpr ]; then
    gmx grompp -f ../em.mdp -c ../../../ions.gro -p ../../../newtop.top -r ../../../ions.gro -o em.tpr -maxwarn 2
fi
if [ -f em.tpr ] && [ ! -f em.gro ]; then
    gmx mdrun -s em.tpr -deffnm em -ntomp $NP_1 -ntmpi 1 -nb gpu -gpu_id 0
    #gmx mdrun -s em.tpr -deffnm em -ntomp 10 -ntmpi 1 -gpu_id 0
fi
###########################
mkdir ../nvt
cd ../nvt
if [ ! -f nvt.tpr ];then
    gmx grompp -f ../nvt.mdp -c ../em/em.gro -p ../../../newtop.top -o nvt.tpr -r ../em/em.gro -maxwarn 4 -n ../../../index.ndx
fi
if [ -f nvt.tpr ] && [ ! -f nvt.gro ];then
    gmx mdrun -s nvt.tpr -deffnm nvt -ntmpi 1 -ntomp $NP_1 -nb gpu -bonded gpu -pme gpu -gpu_id 0
fi
##########################
mkdir ../npt
cd ../npt
if [ ! -f npt.tpr ];then
    gmx grompp -f ../npt.mdp -c ../nvt/nvt.gro -t ../nvt/nvt.cpt -p ../../../newtop.top -o npt.tpr -r ../nvt/nvt.gro -maxwarn 4 -n ../../../index.ndx
fi
if [ -f npt.tpr ] && [ ! -f npt.gro ];then
    gmx mdrun -s npt.tpr -deffnm npt -ntmpi 1 -ntomp $NP_1 -nb gpu -bonded gpu -gpu_id 0 -pme gpu
fi
###################################
mkdir ../prod
cd ../prod
if [ ! -f prod.tpr ];then
    gmx grompp -f ../md.mdp -c ../npt/npt.gro -t ../npt/npt.cpt -p ../../../newtop.top -o prod.tpr -r ../npt/npt.gro -maxwarn 4 -n ../../../index.ndx
fi
if [ -f prod.tpr ] && [ ! -f prod.gro ];then
    gmx mdrun -s prod.tpr -deffnm prod -dhdl dhdl -ntmpi 1 -ntomp $NP_1 -nb gpu -bonded gpu -gpu_id 0 -pme gpu
fi
###################################
cd ../..
done
cd ..
done
```

```python
import sys

def gen_job(wins, step):

    job_lst = [(i, i+step-1) for i in range(0, wins, step)]

    for i in range(len(job_lst)):
        rt = open("job_"+str(i)+".sh", "w")
        rt.write(
"""
#PBS -q quick
#PBS -l nodes=1:ppn=11:gpus=1
#PBS -S /bin/bash
#PBS -j oe
#PBS -N gmx_fep

date
echo "CUDA_VISIBLE_DEVICES: $CUDA_VISIBLE_DEVICES"
NP=`cat $PBS_NODEFILE | wc -l`
NP_1=$((NP - 1))
NP_2=$((NP / 2))
echo "NP: $NP"
echo "NP_1: $NP_1"
echo "NP_2: $NP_2"
cd $PBS_O_WORKDIR
echo $PBS_O_WORKDIR
source /public/home/yqyang/software/gromacs-2021.5-installed/bin/GMXRC.bash
export LD_LIBRARY_PATH=/public/software/lib/:$LD_LIBRARY_PATH
source /public/software/compiler/intel/intel-compiler-2017.5.239/bin/compilervars.sh intel64

for i in `seq {0} {1}`
do
cd lambda${{i}}
#############################
mkdir em
cd em
if [ ! -f em.tpr ]; then
    gmx grompp -f ../em.mdp -c ../../../ions.gro -p ../../../newtop.top -r ../../../ions.gro -o em.tpr -maxwarn 2
fi
if [ -f em.tpr ] && [ ! -f em.gro ]; then
    gmx mdrun -s em.tpr -deffnm em -ntomp $NP_1 -ntmpi 1 -nb gpu -gpu_id 0
    #gmx mdrun -s em.tpr -deffnm em -ntomp 10 -ntmpi 1 -gpu_id 0
fi
###########################
mkdir ../nvt
cd ../nvt
if [ ! -f nvt.tpr ];then
    gmx grompp -f ../nvt.mdp -c ../em/em.gro -p ../../../newtop.top -o nvt.tpr -r ../em/em.gro -maxwarn 4 -n ../../../index.ndx
fi
if [ -f nvt.tpr ] && [ ! -f nvt.gro ];then
    gmx mdrun -s nvt.tpr -deffnm nvt -ntmpi 1 -ntomp $NP_1 -nb gpu -bonded gpu -pme gpu -gpu_id 0
fi
##########################
mkdir ../npt
cd ../npt
if [ ! -f npt.tpr ];then
    gmx grompp -f ../npt.mdp -c ../nvt/nvt.gro -t ../nvt/nvt.cpt -p ../../../newtop.top -o npt.tpr -r ../nvt/nvt.gro -maxwarn 4 -n ../../../index.ndx
fi
if [ -f npt.tpr ] && [ ! -f npt.gro ];then
    gmx mdrun -s npt.tpr -deffnm npt -ntmpi 1 -ntomp $NP_1 -nb gpu -bonded gpu -gpu_id 0 -pme gpu
fi
###################################
mkdir ../prod
cd ../prod
if [ ! -f prod.tpr ];then
    gmx grompp -f ../md.mdp -c ../npt/npt.gro -t ../npt/npt.cpt -p ../../../newtop.top -o prod.tpr -r ../npt/npt.gro -maxwarn 4 -n ../../../index.ndx
fi
if [ -f prod.tpr ] && [ ! -f prod.gro ];then
    gmx mdrun -s prod.tpr -deffnm prod -dhdl dhdl -ntmpi 1 -ntomp $NP_1 -nb gpu -bonded gpu -gpu_id 0 -pme gpu
fi
###################################
cd ../..
done
""".format(str(job_lst[i][0]), str(job_lst[i][1]))
        )
        rt.close()

def main():

    wins = int(sys.argv[1])
    step = int(sys.argv[2])
    gen_job(wins, step)

if __name__=="__main__":
    main() 
```

**free状态准备**
与bonded状态的准备差不多一致。
盒子设置命令：
```shell
gmx editconf -f conf.pdb -o box.gro -bt cubic -box 10.995 10.995 10.995
```

**说明：bonded是gmx跑出来的MD轨迹中的一帧，free是NAMD跑出来的MD轨迹中的一帧。在保持盒子大小一致的情况下进行FEP计算。保留MD中水的位置信息。**