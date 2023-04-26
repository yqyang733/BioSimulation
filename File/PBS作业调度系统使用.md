👏 PBS作业调度系统使用

---
[TOC]

---
## 基础命令
### 作业提交和管理命令
|命令|功能|示例|
|:----|:----|:----|
|qsub|提交作业|qsub job.pbs|
|qdel|删除作业|qdel 12583|
|qhold|挂起作业|qhold 12583|
|qrls|释放作业|qrls 12583|

### 作业状态查询
```shell
qstat -q            列出所有队列
qstat -a            列出所有作业
qstat -u user_id    列出user_id的所有作业
qstat -r            列出所有正在运行的作业
qstat -f job_id     列出作业job_id的信息
qstat -fQ queue     列出队列queue的信息
qstat -B            列出所有作业状态的汇总
pbsnodes            列出所有节点的详细信息
pestat              列出所有节点的状态
```

### 作业参数
**可以在shell脚本头部添加也可以在命令行中指定。**
（1）参数
```shell
-l  申请资源
    nodes=4/node168 申请节点数目或具体节点名称
    ppn 申请的核数
    walltime 最大运行时间
-q  申请队列名称
-N  作业名称
-o  重定向STDOUT的输出
    默认: Job_Name.oSequence_Number
-e  重定向STDERR的输出
    默认: Job_Name.eSequence_Number
-j  STDERR和STDOUT是否输出到一起
    oe: 一起输出到STDOUT
    eo: 一起输出到STDERR
-d  作业的工作目录
    默认: 在用户主目录运行作业
```
（2）参数写入脚本
在shell脚本头部写入如下内容。
```shell
#!/bin/bash
#PBS -l nodes=1:ppn=16
#PBS -l walltime=1000:00:00
#PBS -q high
#PBS -N Job_Name
#PBS -j oe
#PBS -l mem=2gb                        ##分配多少内存

your_commands_goes_here
```

（3）命令行参数提交
```shell
qsub -l nodes=1:ppn=14 -l walltime=1000:00:00 -q high -N Job_Name -oe job.sh
```

## 批量提交作业
可以通过任务模板与qsub -v选项，进行作业提交时的参数传递，利用一个模板脚本，直接提交多个任务。如下所示：
**模板任务执行文件：template.sh**
```shell
#!/bin/bash
#PBS -j oe
#PBS -q high
#PBS -l nodes=1:node:ppn=4
#PBS walltime=1000:00:00

hisat2 -p 4 -x hg19 -1 ./reads/${SAMPLE}_1.fq.gz -2 ./reads/${SAMPLE}_2.fq.fz -t | samtools view -bS > ./align/${SAMPLE}.bam
```
**任务文件**
```text
sample1
sample2
sample3
```
**任务投递脚本qsub.py**
```python
#!/usr/bin/env python
import csv
import subprocess
import time

param_file = './job_params.csv'
cwd = '/home/bioinfo/RNA-seq'

with open(param_file, 'r') as f:
    reader = csv.reader(f)
    for sample in reader:
        qsub_cmd = 'qsub -N {0} -d {1} -v SAMPLE={0} template.sh'.format(sample, cwd)
        # print qsub_cwd
        
        exit_status = subprocess.call(qsub_cmd, shell=True)
        if exit_status is 1:
            print 'Job "{}" failed to submit'.format(qsub_cwd)
        time.sleep(1)

print "Done submitting jobs!"
```
**提交任务**
```shell
python qsub.py
```
对于更加复杂的分析流程，我们可以在csv中指定更多的参数，并通过-v PARAM1=param1,PARAM2=param2的格式进行多个参数的传递，实现相同的效果。  
**NOTE:** 脚本中加入time.sleep(1)在每提交一个任务后暂停一秒，如果不加该命令，会导致在同一时间向PBS系统提交任务数目过多，可能出现作业没有运行直接结束的bug…

## 批量删除作业
（1）结合qselect和xargs命令，进行作业编号提取和对指定编号作业的删除。
```shell
qselect -u user_id | xargs qdel
```
（2）使用qselect参数进一步缩减选择的作业范围。
```shell
-N  指定作业名字
-s  指定状态 [E/H/Q/R/T/W]
-u  指定用户列表
```
例如删除某用户所有正在排队的任务:
```shell
qselect -u user_name -s Q | xargs qdel 
```

## 参考资料
1. https://mp.weixin.qq.com/s/gO--Yv9ZakXGaCCqMksUVA
