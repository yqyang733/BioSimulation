👏 tleap构建MD模拟体系的流程化shell脚本

---
[TOC]

---
## 脚本
```shell
if [ -e leap.log ];then rm leap.log;fi
if [ -e tleap.in ];then rm tleap.in;fi
if [ -e tleap1.in ];then rm tleap1.in;fi
echo "source leaprc.protein.ff14SB" >> tleap.in
echo "source leaprc.DNA.OL15" >> tleap.in
echo "source leaprc.RNA.OL3" >> tleap.in
echo "source leaprc.water.tip3p" >> tleap.in
echo "source leaprc.gaff2" >> tleap.in
echo "loadamberparams frcmod.ionsjc_tip3p" >> tleap.in
echo "mol = loadpdb complex.pdb" >> tleap.in
echo "solvatebox mol TIP3PBOX 12.0" >> tleap.in
echo "charge mol" >> tleap.in
echo "quit" >> tleap.in
tleap -s -f tleap.in
charge=`awk '$0 ~ /> charge mol/{getline score; print score}' leap.log |head -n1|awk -F ":" '{print $2}'|sed s/[[:space:]]//g`
char=`printf "%.0f\n" $(echo "scale=2;${charge}" | bc)`
#charr=`echo "${char}*(-1)" | bc`
water=`awk '$0 ~ /Total mass/{getline score; print score}' leap.log|awk '{print $2}'|sed s/[[:space:]]//g`
echo "!!!!"${water}
wat=`printf "%.0f\n" $(echo "scale=2;${water}" | bc)`
echo "!!!!"${wat}
watt=`echo "${wat}*0.0187*0.15" | bc`
echo "!!!!"${watt}
wattt=`printf "%.0f\n" ${watt}`
echo "!!!!"${wattt}
echo "source leaprc.protein.ff14SB" >> tleap1.in
echo "source leaprc.DNA.OL15" >> tleap1.in
echo "source leaprc.RNA.OL3" >> tleap1.in
echo "source leaprc.water.tip3p" >> tleap1.in
echo "source leaprc.gaff2" >> tleap1.in
echo "loadamberparams frcmod.ionsjc_tip3p" >> tleap1.in
echo "mol = loadpdb complex.pdb" >> tleap1.in
echo "solvatebox mol TIP3PBOX 12.0" >> tleap1.in
if [ `echo "$char < 0.0" |bc` -eq 1 ];then echo "addions mol Na+ ${wattt}" >> tleap1.in;echo "addions mol Cl- 0" >> tleap1.in;fi
if [ `echo "$char > 0.0" |bc` -eq 1 ];then echo "addions mol Cl- ${wattt}" >> tleap1.in;echo "addions mol Na+ 0" >> tleap1.in;fi
echo "saveamberparm mol complex_solvated.prmtop complex_solvated.inpcrd" >> tleap1.in

#echo "source oldff/leaprc.ff14SB" >> tleap1.in
#echo "loadAmberParams frcmod.ionsjc_tip3p" >> tleap1.in
#echo "PTEN=loadpdb PTEN.pdb" >> tleap1.in
#echo "TFEB=loadpdb TFEB.pdb" >> tleap1.in
#echo "complex=loadpdb A_157_${i}_PTEN-TFEB.pdb" >> tleap1.in
#echo "saveamberparm PTEN PTEN.prmtop PTEN.inpcrd" >> tleap1.in
#echo "saveamberparm TFEB TFEB.prmtop TFEB.inpcrd" >> tleap1.in
#echo "saveamberparm complex complex.prmtop complex.inpcrd" >> tleap1.in
#if [ `echo "$char < 0.0" |bc` -eq 1 ];then echo "addions complex Na+ ${charr}" >> tleap1.in;fi
#if [ `echo "$char > 0.0" |bc` -eq 1 ];then echo "addions complex Cl- ${char}" >> tleap1.in;fi
#echo "solvatebox complex TIP3PBOX 9.0" >> tleap1.in
#echo "saveamberparm complex complex_solvated.prmtop complex_solvated.inpcrd" >> tleap1.in
echo "quit" >> tleap1.in
tleap -s -f tleap1.in
ambpdb -p complex_solvated.prmtop < complex_solvated.inpcrd > check.pdb
#cd ..
#done
```