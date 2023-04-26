#PBS -N namd-com21
#PBS -q md
#PBS -l nodes=1:ppn=11:gpus=1
#PBS -S /bin/bash
#PBS -j oe

# date
# echo "CUDA_VISIBLE_DEVICES: $CUDA_VISIBLE_DEVICES"
# export LD_LIBRARY_PATH=/public/software/lib/:$LD_LIBRARY_PATH
# source /public/software/compiler/intel/intel-compiler-2017.5.239/bin/compilervars.sh intel64
# cd $PBS_O_WORKDIR
# echo $PBS_O_WORKDIR

NAMD="/home/yqyang/software/NAMD_3.0alpha13/namd3 +p15 +devices 0"
# path="/public/home/yqyang/COVID-19/drug_design/MD-200ns/com21"

# cd ${path}

## min
cd prod
# base_min=pro-lig-min
# $NAMD $base_min > $base_min.log
#base_min=pro-lig-min
#$NAMD $base_min > $base_min.log

## nvt npt
base_equil=pro-lig-equil
$NAMD $base_equil > $base_equil.log

## prod
# base_prod=pro-lig-prod
# $NAMD $base_prod > $base_prod.log



# for (( ii=1; ii<=2; ii++ )); do
#     rsync -avh eq/fep.namd eq/fep.tcl eq/equilibrate.coor eq/equilibrate.vel eq/equilibrate.xsc t$ii
#     $NAMD t$ii/fep.namd >& t$ii/LOG_fep
# done
