👏 Linux查看与使用配置信息

---
[TOC]

---
### 1. 配置信息查看
#### 1.1 查看Linux系统核数
```shell
cat /proc/cpuinfo|grep processor|wc -l
```
#### 1.2 查看CPU使用状态
```shell
top
```
#### 1.3 查看GPU使用状态
```
nvidia-smi
```
#### 1.4

---
### 2. 配置使用
#### 2.1 使用指定的GPU
```shell
CUDA_VISIBLE_DEVICES=1 nohup python my_script.py &
```
参考资料：     
1. https://www.cnblogs.com/darkknightzh/p/6836568.html
---
### 3. 软件安装
#### 3.1 Linux GLIBC_2.14更新
(1) 查看当前系统中所支持的glibc版本    
```shell
strings /lib64/libc.so.6 |grep GLIBC_
```
(2) 下载GLIBC_2.14软件包：http://ftp.gnu.org/gnu/glibc/glibc-2.14.tar.gz      
(3) 解压&安装
```shell
tar -xvf  glibc-2.14.tar.gz
mkdir glibc-2.14/build
cd glibc-2.14/build
../configure  --prefix=/home/jawang/software/glibc-2.14-lib   # 在build生成参数文件。
make -j 8
make install
```
(4) export    
```shell
export LD_LIBRARY_PATH=/home/jawang/software/glibc-2.14-lib/lib:$LD_LIBRARY_PATH
```
(5) vi ~/.bashrc
```shell
#glibc-2.14
export LD_LIBRARY_PATH=/home/jawang/software/glibc-2.14-lib/lib:$LD_LIBRARY_PATH
```
参考资料：      
1. https://jingyan.baidu.com/article/d2b1d102ec07565c7e37d4c0.html     

#### 3.2 Linux GLIBC_2.17更新
(1) 下载安装包更新与上面2.14版本的方式一样     
(2) 可以使用管理员权限直接升级     
```shell
su
yum updata glibc
```

#### 3.3 aria2c 安装
(1) 文件下载：
```shell
wget https://github.com/aria2/aria2/releases/download/release-1.33.0/aria2-1.33.0.tar.gz
```
(2) 解压：
```shell
tar -zxvf aria2-1.33.0.tar.gz
```
(3) 建安装文件夹
```shell
mkdir aria2
```
(4) 编译
```
cd aria2-1.33.0
mkdir build
cd build/
../configure  --prefix=/home/g16/software/aria2
```
(5) 安装
```shell
make -j 8
make install
```
(6) 将命令加入环境变量
```shell
vi ~/.bashrc
export PATH=$PATH:/home/g16/software/aria2/bin
alias aria2c='/home/g16/software/aria2/bin/aria2c'
source ~/.bashrc
```

#### 3.4 安装cppunit
```shell
sudo yum install cppunit-devel
```
