#PBS -q md
#PBS -l nodes=1:ppn=11:gpus=1
#PBS -S /bin/bash
#PBS -j oe
#PBS -N mem_build

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

init=step5_input
rest_prefix=step5_input
mini_prefix=step6.0_minimization
#equi_prefix=step6.%d_equilibration
prod_prefix=step7_production
prod_step=step7

# Minimization
# In the case that there is a problem during minimization using a single precision of GROMACS, please try to use 
# a double precision of GROMACS only for the minimization step.
#mkdir em
#cd em
#gmx grompp -f ../mdp/${mini_prefix}.mdp -o ${mini_prefix}.tpr -c ../${init}.gro -r ../${rest_prefix}.gro -p ../topol.top -n ../index.ndx
#gmx_d mdrun -v -deffnm ${mini_prefix}
#gmx mdrun -s ${mini_prefix}.tpr -deffnm ${mini_prefix} -ntmpi 1 -ntomp $NP_1 -nb gpu -gpu_id 0
#cd ..

# Equilibration
mkdir equil
cd equil
cnt=1
cntmax=6
cp ../em/${mini_prefix}.gro .
#while ( ${cnt} <= ${cntmax} )
while [ `echo "${cnt} <= ${cntmax}" |bc` -eq 1 ]
do
    pcnt=`echo "${cnt}-1"|bc`
    #set istep = `printf ${equi_prefix} ${cnt}`
    #set pstep = `printf ${equi_prefix} ${pcnt}`
    #if ( ${cnt} == 1 ) set pstep = ${mini_prefix}
    if [ `echo "${cnt} == 1" |bc` -eq 1 ];then pstep=${mini_prefix};else pstep=step6.${pcnt}_equilibration;fi

    gmx grompp -f ../mdp/step6.${cnt}_equilibration.mdp -o step6.${cnt}_equilibration.tpr -c ${pstep}.gro -r ../${rest_prefix}.gro -p ../topol.top -n ../index.ndx
    #gmx mdrun -v -deffnm ${istep}
    gmx mdrun -s step6.${cnt}_equilibration.tpr -deffnm step6.${cnt}_equilibration -ntmpi 1 -ntomp $NP_1 -nb gpu -bonded gpu -gpu_id 0 -pme gpu
    cnt=`echo "${cnt}+1"|bc`
done
cd ..

# Production
mkdir prod
cd prod
cnt=1
cntmax=10
cp ../equil/step6.6_equilibration.gro .
#while ( ${cnt} <= ${cntmax} )
while [ `echo "${cnt} <= ${cntmax}" |bc` -eq 1 ]
do
    #@ pcnt = ${cnt} - 1
    pcnt=`echo "${cnt}-1"|bc`
    istep=${prod_step}_${cnt}
    pstep=${prod_step}_${pcnt}

    if [ `echo "${cnt} == 1" |bc` -eq 1 ];then
        pstep=step6.6_equilibration
        gmx grompp -f ../mdp/${prod_prefix}.mdp -o ${istep}.tpr -c ${pstep}.gro -p ../topol.top -n ../index.ndx
    else
        gmx grompp -f ../mdp/${prod_prefix}.mdp -o ${istep}.tpr -c ${pstep}.gro -t ${pstep}.cpt -p ../topol.top -n ../index.ndx
    fi
    #gmx mdrun -v -deffnm ${istep}
    gmx mdrun -s ${istep}.tpr -deffnm ${istep} -ntmpi 1 -ntomp $NP_1 -nb gpu -bonded gpu -pme gpu -gpu_id 0
    cnt=`echo "${cnt}+1"|bc`
done
cd ..
