👏 Linux常用语法语句

---
[TOC]

---
**写在前面：注意：有的linux语句命令行执行和在脚本中执行的正确性可能是不一致的，就是说有的语句敲在命令行中不会报错，但是写在脚本中运行可能会报错。**

## 循环语句

## 比较语句以及算数
**在Bash shell环境中，可以使用let、(())、[]执行基本的算术操作，使用expr和bc可以进行高级操作**
（1）字符串比较。
```shell
if [ "${i}" = "${name_first}" ]
then
cat ./${i}/do_all_fep.sh >> ${name_first}.sh
else
cat ./${i}/do_all_fep.sh >> temp.sh
```

（2）比较数值大小。
**-eq, -lt, -gt, -ne比较的其实都是整数，不能用于浮点数比较。**
```shell
if [ `echo "$a < 1.0" |bc` -eq 1 ];then echo ${line} >> lig_twomore_pdb_1.txt;fi    # 先用逻辑运算输出0或1，然后再与整数比较。
if [ `echo "$a == 1.0" |bc` -eq 1 ];then echo 111111;fi
```

（3）累加运算。
例1：使用bc进行累加运算。
```shell
a=`echo "${a} + ${auc}" | bc`
```

（4）基本算数
```shell
echo "4*5"|bc      # output: 20
```

（5）设定小数精度的算数
```shell
echo "scale=2; 3/8" | bc      # output: .37
```

（6）平方根运算
```shell
echo "sqrt(100)" | bc      # output: 10
```

（7）平方运算
```shell
echo "10^3" | bc       # output: 1000
```

## 函数

## 特殊变量
|变量|含义|
|:----:|:----:|
|\$?|上个命令的退出状态，或函数的返回值。如果返回值是0，就是执行成功；如果是返回值是0以外的值，就是失败。|
|$@|表示命令行所有参数（不包含$0），将每个参数都看作一份数据，彼此之间是独立的。|
|$*|表示命令行所有参数（不包含$0），将所有的参数从整体上看做一份数据，而不是把每个参数都看做一份数据。|
|$$|Shell本身的PID（ProcessID，即脚本运行的当前 进程ID号）|
|$#|传递给脚本或函数的参数个数。|

## 读写文件
（1）一行一行的读入文件。
```shell
while read line
do
  echo "aaaaa" > temp
done < file
```
