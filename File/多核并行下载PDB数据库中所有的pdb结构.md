👏 多核并行下载PDB数据库中所有的pdb结构

---

[封面PPT](../PPT/多核并行下载PDB数据库中所有的pdb结构.pptx)

---

[TOC]

---

写在前面：本文章操作均在Linux操作系统中进行~

## 1. 获取所有pdb的id并进一步处理

(1) 获取所有 PDB ID 的文件下载地址，如PPT中图片所示。

(2) 下载PPT中图片所示所有文件并放在同一目录下。

(3) 合并所有 PDB ID至一个文件中，并且一个 ID 是一行。

```bash
for i in `ls`;do echo "," >> ${i};done    # 在每个文件末尾加逗号
for i in `ls`;do sed -e $'s/,/\\\n/g' ${i} >> pdb_211021.txt;done     # 将文件中所有逗号换成换行符，保证一个ID是一行
sed -i '/^\s*$/d' pdb_211021.txt   # 去除文件中的空行
```

## 2. 多核并行下载

(1) 创建存储子任务和pdb结构的文件夹

```bash
mkdir subjob
mkdir pdb_211021
```

(2) 运行下述脚本将任务分成100个子任务进行多核批量下载

```bash
line=`wc -l pdb_211021.txt | awk '{print int($1/100)+1}'`
split -l ${line} pdb_211021.txt -d -a 2 ./subjob/pdb_

function submit_job(){
for i in `cat ./subjob/pdb_$1`; do mkdir ./pdb_211021/${i};wget -q -N -O ./pdb_211021/${i}/${i}.pdb http://www.rcsb.org/pdb/files/${i}.pdb;done &
}

submit_job 00
submit_job 01
submit_job 02
submit_job 03
submit_job 04
submit_job 05
submit_job 06
submit_job 07
submit_job 08
submit_job 09
submit_job 10
submit_job 11
submit_job 12
submit_job 13
submit_job 14
submit_job 15
submit_job 16
submit_job 17
submit_job 18
submit_job 19
submit_job 20
submit_job 21
submit_job 22
submit_job 23
submit_job 24
submit_job 25
submit_job 26
submit_job 27
submit_job 28
submit_job 29
submit_job 30
submit_job 31
submit_job 32
submit_job 33
submit_job 34
submit_job 35
submit_job 36
submit_job 37
submit_job 38
submit_job 39
submit_job 40
submit_job 41
submit_job 42
submit_job 43
submit_job 44
submit_job 45
submit_job 46
submit_job 47
submit_job 48
submit_job 49
submit_job 50
submit_job 51
submit_job 52
submit_job 53
submit_job 54
submit_job 55
submit_job 56
submit_job 57
submit_job 58
submit_job 59
submit_job 60
submit_job 61
submit_job 62
submit_job 63
submit_job 64
submit_job 65
submit_job 66
submit_job 67
submit_job 68
submit_job 69
submit_job 70
submit_job 71
submit_job 72
submit_job 73
submit_job 74
submit_job 75
submit_job 76
submit_job 77
submit_job 78
submit_job 79
submit_job 80
submit_job 81
submit_job 82
submit_job 83
submit_job 84
submit_job 85
submit_job 86
submit_job 87
submit_job 88
submit_job 89
submit_job 90
submit_job 91
submit_job 92
submit_job 93
submit_job 94
submit_job 95
submit_job 96
submit_job 97
submit_job 98
submit_job 99
```

## 3. 说明

下载会收到网速的限制，增加子任务数目，会提高下载速度直到网速饱和，网速饱和之后再增加子任务数不会进一步提高下载速度。