👏 Gromacs|AMBER/GAFF力场下蛋白配体复合物分子动力学模拟_bcc_tleap

---
[TOC]

---
## antechamber计算配体BCC电荷
（1）检查配体三维结构的完整性，特别注意加氢处理，保证配体分子的结构完整性。
（2）采用bcc方法拟合电荷：
```shell
antechamber -i lig.mol2 -fi mol2 -o LIG.mol2 -fo mol2 -c bcc -nc 0 -pf y

# -i 指定输入文件
# -fi 指定输入文件类型
# -o 指定输出文件
# -fo 指定输出文件类型
# -nc 指定体系所带总电量，此处为-1（如果没有-nc这一命令，则默认体系呈中性）
# -pf y 不产生中间文件 （此命令可以不加，只是这样会产生一系列中间文件）
# -c bcc  指定拟合原子电荷的方法，此处为采用bcc方法
```
（3）生成配体分子的frcmod文件：
```python
parmchk2 -i LIG.mol2 -f mol2 -o LIG.frcmod
```

## tleap建模
（1）蛋白pdb文件去氢
（2）tleap
```shell
tleap
source oldff/leaprc.ff14SB
source leaprc.water.tip3p
source leaprc.gaff2
loadamberparams frcmod.ionsjc_tip3p
loadamberparams LIG.frcmod
LIG = loadmol2 LIG.mol2
REC = loadpdb rec.pdb
list
complex=combine {REC LIG}
solvatebox complex TIP3PBOX 15.0 iso
# setbox complex vdw {15 15 15}
# solvatebox complex TIP3PBOX 0
charge complex
# 计算电荷：residue*0.0187*浓度
addions complex Na+ 119
addions complex Cl- 0
saveamberparm complex complex_solvated.prmtop complex_solvated.inpcrd
quit
```
（3）将prmtop和inpcrd转成pdb观察结构的正确性。
```shell
ambpdb -p complex_solvated.prmtop < complex_solvated.inpcrd > check.pdb
```

## 转 gro top
```python
import parmed as pmd

parm = pmd.load_file('complex_solvated.prmtop', 'complex_solvated.inpcrd')
parm.save('gromacs.top', format='gromacs')
parm.save('gromacs.gro')
```

## MD
**位置限制：**
```python
def get_atomid(file_in, obje, flag):
    from pymol import cmd
    cmd.delete("all")
    cmd.load(file_in)
    if flag:
        cmd.remove("hydrogens")
    return cmd.identify(obje, 0)

def generate_porse_single(file_in, sele, out):
    atomid = get_atomid(file_in, sele, True)
    atomid_ = get_atomid(file_in, sele, False)[0]
    result = open(out, "w")
    result.write("[ position_restraints ]\n; atom  type      fx      fy      fz\n")
    if atomid_ == 1:
        for i in atomid:
            result.write("{:>6d}".format(i) + "     1  1000  1000  1000 \n")
    else:
        for i in range(len(atomid)):
            atomid[i] = atomid[i] - atomid_ + 1
        for i in atomid:
            result.write("{:>6d}".format(i) + "     1  1000  1000  1000 \n")

def generate_porse(file_in):
    generate_porse_single(file_in, "polymer.protein", "posre_pro.itp")
    generate_porse_single(file_in, "resn LIG", "posre_lig.itp")

def main():
    import sys
    generate_porse(sys.argv[1])

if __name__=="__main__":
    main()
```

将下述命令加入到对应的topol.top文件中
```text
; Include Position restraint file
#ifdef POSRES
#include "posre_pro.itp"
#endif

; Include Position restraint file
#ifdef POSRES
#include "posre_lig.itp"
#endif
```

**mdp文件：**
[em.mdp](GromacsAMBERGAFF力场下蛋白配体复合物分子动力学模拟_bcc_tleap/em.mdp)
[nvt.mdp](GromacsAMBERGAFF力场下蛋白配体复合物分子动力学模拟_bcc_tleap/nvt.mdp)
[npt.mdp](GromacsAMBERGAFF力场下蛋白配体复合物分子动力学模拟_bcc_tleap/npt.mdp)
[prod.mdp](GromacsAMBERGAFF力场下蛋白配体复合物分子动力学模拟_bcc_tleap/prod.mdp)

**index.ndx**
```shell
gmx make_ndx -f solv_ions.gro -o index.ndx
```

**job.sh：**
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

#############################
mkdir em
cd em
if [ ! -f em.tpr ]; then
    gmx grompp -f ../mdp/em.mdp -c ../solv_ions.gro -p ../topol-nvpt.top -r ../solv_ions.gro -o em.tpr -maxwarn 2
fi
if [ -f em.tpr ] && [ ! -f em.gro ]; then
    gmx mdrun -s em.tpr -deffnm em -ntomp ${NP_1} -ntmpi 1 -nb gpu -gpu_id 0
fi
#############################
mkdir ../nvt
cd ../nvt
if [ ! -f nvt.tpr ];then
    gmx grompp -f ../mdp/nvt.mdp -c ../em/em.gro -p ../topol-nvpt.top -o nvt.tpr -r ../em/em.gro -maxwarn 4 -n ../index.ndx
fi
if [ -f nvt.tpr ] && [ ! -f nvt.gro ];then
    gmx mdrun -s nvt.tpr -deffnm nvt -ntmpi 1 -ntomp ${NP_1} -nb gpu -bonded gpu -pme gpu -gpu_id 0
fi
############################
mkdir ../npt
cd ../npt
if [ ! -f npt.tpr ];then
    gmx grompp -f ../mdp/npt.mdp -c ../nvt/nvt.gro -t ../nvt/nvt.cpt -p ../topol-nvpt.top -o npt.tpr -r ../nvt/nvt.gro -maxwarn 4 -n ../index.ndx
fi
if [ -f npt.tpr ] && [ ! -f npt.gro ];then
    gmx mdrun -s npt.tpr -deffnm npt -ntmpi 1 -ntomp ${NP_1} -nb gpu -bonded gpu -pme gpu -gpu_id 0
fi
#########################
mkdir ../prod
cd ../prod
if [ ! -f prod.tpr ];then
    gmx grompp -f ../mdp/prod.mdp -c ../npt/npt.gro -t ../npt/npt.cpt -p ../topol-prod.top -o prod.tpr -r ../npt/npt.gro -maxwarn 4 -n ../index.ndx
fi
if [ -f prod.tpr ] && [ ! -f prod.gro ];then
    gmx mdrun -s prod.tpr -deffnm prod -dhdl dhdl -ntmpi 1 -ntomp ${NP_1} -nb gpu -bonded gpu -pme gpu -gpu_id 0
fi
```

## PBC处理
使用常用的四条 gmx pbc 处理命令进行 pbc 处理。

## MMGB/PBSA
mmgbsa.in
```text
Sample input file for GB calculation
This input file is meant to show only that gmx_MMPBSA works. Althought,
we tried to used the input files as recommended in the Amber manual,
some parameters have been changed to perform more expensive calculations
in a reasonable amount of time. Feel free to change the parameters
according to what is better for your system.

&general
sys_name="Prot-Lig",
startframe=500,
endframe=1000,
interval=10,
forcefields="leaprc.protein.ff14SB,leaprc.gaff",
#Interaction Entropy (IE)(https://pubs.acs.org/doi/abs/10.1021/jacs.6b02682) approximation
interaction_entropy=1, ie_segment=25, temperature=310
/
&gb
igb=5, saltcon=0.150
/
&decomp
idecomp=2, dec_verbose=3,
# This will print all residues that are less than 10 Å between
# the receptor and the ligand
print_res="within 100"
/
```
计算命令：在有有机小分子时，需要额外提供一个mol2文件，这个mol2文件就是前方 antechamber 命令产生的 LIG.mol2 文件。
```shell
 gmx_MMPBSA -O -i mmgbsa.in -cs ../pbc/new.pdb -ci ../pbc/index_new.ndx -cg 1 13 -lm LIG.mol2 -ct ../pbc/md_pbcfit_all_new.xtc -o FINAL_RESULTS_MMPBSA.dat -eo FINAL_RESULTS_MMPBSA.csv -do FINAL_DECOMP_MMPBSA.dat -deo FINAL_DECOMP_MMPBSA.csv -nogui
```