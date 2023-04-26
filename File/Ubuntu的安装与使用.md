👏 Ubuntu的安装与使用

---
[TOC]

---
## 安装ubuntu后如何将电脑变成服务器。
```shell
ssh localhost  # 测试是否能连
sudo apt-get update   
sudo apt-get install openssh-server  # 安装openssh-server
ssh localhost  # 再次测试是否连接建立成功
ifconfig  # 查看ip地址
```
参考资料：
1. [环境配置——把Ubuntu 20.04系统电脑变成一台服务器，并用win10的电脑进行访问](https://blog.csdn.net/jindaxiaoooo/article/details/110160931)

## 安装chrome。
```shell
wget https://dl.google.com/linux/direct/google-chrome-stable_current_amd64.deb
sudo dpkg -i google-chrome-stable_current_amd64.deb
```
参考资料：
1. [Ubuntu安装谷歌浏览器Chrome](https://blog.51cto.com/u_12369060/5119932?b=totalstatistic)

## 安装vmd。
（1）首先在vmd官网下载软件，解压缩：tar -xvf *.gz。
（2）进入到解压后的文件中，修改安装路径和设置lib的路径：
```shell
vi configure
$install bin dic…（修改为要安装的路径）  # 设置安装路径
$lib bin …….（修改为要安装的路径）  # 设置安装路径
```
（3）选择需要安装的东西。
```shell
./configure LINUXAMD64 OPENGL TCL COLVARS
```
（4）安装
```shell
cd src   #进入src文件中
make install   #安装
```
（5）测试
进入安装目录bin目录下：
```shell
./vmd  #查看是否安装成功
```
（6）将vmd添加到bashrc中。

参考资料：
1. [VMD在Ubuntu系统下的安装](https://blog.sciencenet.cn/blog-2506040-1041334.html)

## 安装vscode
```
# 官网下载code_1.71.2-1663191218_amd64.deb
sudo dpkg -i code_1.71.2-1663191218_amd64.deb
# 安装完成之后命令行输入code即可运行
```

## ZJU有线网配置

参考资料：
1. [ZJU的ubuntu有线网配置](https://www.cc98.org/topic/4848071)
2. [zjunet](https://github.com/QSCTech/zjunet)

## 连接蓝牙键盘
（1）安装可以显示配对密码的蓝牙插件。
```shell
sudo apt-get install bluez-hcidump
```
（2）将蓝牙键盘设置为可发现状态，打开Linux的蓝牙，选择连接蓝牙键盘
（3）此时终端上会出现配对密码，在蓝牙键盘上输入终端显示的6个数字的密码后按Enter

参考资料：
1. [ubuntu 20.04 连接 罗技 K480键盘](https://blog.csdn.net/sinat_38679789/article/details/118737602)