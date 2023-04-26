👏 Linux中CentOS操作系统上安装GROMACS

---
[TOC]

---
## 从头编译安装
### 编译安装gcc
**多次尝试，当下服务器cuda只能支持10以下版本的gcc，所以安装9.5版本的gcc。**  
（1）下载9.5版本的gcc压缩包：http://ftp.gnu.org/gnu/gcc/gcc-9.5.0/gcc-9.5.0.tar.gz  
（2）解压缩编译安装   
```shell
mkdir gcc-9.5.0-installed
tar -zxvf gcc-9.5.0.tar.gz
cd gcc-9.5.0
mkdir build
cd build
../configure --disable-checking --enable-languages=c,c++ --disable-multilib --prefix=/home/yqyang/software/gcc-9.5.0-installed/ --enable-threads=posix 
make -j10
make install
```
（3）添加至环境变量
```shell
vi ~/.bashrc
#gcc-9.5.0
export PATH="/home/yqyang/software/gcc-9.5.0-installed/bin:"$PATH
export LD_LIBRARY_PATH="/home/yqyang/software/gcc-9.5.0-installed/lib64:"$LD_LIBRARY_PATH
source ~/.bashrc
```
（4）查看安装成功与否
```shell
gcc -v
```

### 编译安装cmake
**这里安装最新版本3.23.2的cmake**
（1）官网上下载安装包：https://cmake.org/download/  
（2）解压缩编译安装   
```shell
mkdir cmake-3.23.2-installed
tar -zxvf cmake-3.23.2.tar.gz
cd cmake-3.23.2
mkdir build
cd build
../configure --prefix=/home/yqyang/software/cmake-3.23.2-installed/ 
make -j10
make install
```
（3）添加至环境变量
```shell
vi ~/.bashrc
#CMake-3.23.2
export PATH="/home/yqyang/software/cmake-3.23.2-installed/bin:"$PATH
source ~/.bashrc
```
（4）查看安装成功与否
```shell
cmake --version
```

### 驱动安装
这里使用的系统是ubuntu。
（1）ubuntu中打开Software & Updates。
（2）点击Additional Drivers。
（3）选择第一个（括号里有tested的那一个）。
（4）Apply Changes。
（5）重启。
（6）nvidia-smi进行测试。
参考资料：
1. [新装的ubuntu系统如何安装nvidia驱动、CUDA、CUDNN](https://blog.csdn.net/ashiners/article/details/118612897)
2. 

### 编译安装cuda
**安装cuda-11.3时在编译后续gromacs时会报错，所以这里安装cuda-11.2**
（1）官网上下载对应型号和版本的安装包：https://developer.nvidia.com/downloads  
（2）安装
```shell
mkdir cuda-11.2-installed
sh cuda_11.2.0_460.27.04_linux.run --toolkit --toolkitpath=/home/yqyang/software/cuda-11.2-installed --defaultroot=/home/yqyang/software/cuda-11.2-installed
```
**注意：在cuda install界面只选中”CUDA Toolkit 11.2”（前面带X默认选中）**
（3）由于本机安装了多个版本的cuda，为了不影响其他软件的正常运行，这里不将其加入bashrc中，只在当前界面export。
```shell
export PATH="/home/yqyang/software/cuda-11.2-installed/bin:"$PATH
export LD_LIBRARY_PATH="/home/yqyang/software/cuda-11.2-installed/lib64:"$LD_LIBRARY_PATH
```

### 编译安装openmpi
**这里安装4.0.7版本的openmpi**
（1）官网上下载对应型号和版本的安装包：https://www.open-mpi.org/software/ompi/v4.0/  
（2）解压缩编译安装
```shell
mkdir openmpi-4.0.7-installed
tar -zxvf openmpi-4.0.7
cd openmpi-4.0.7
mkdir build
cd build
../configure --prefix=/home/yqyang/software/openmpi-4.0.7-installed/
make -j10
make install
```
（3）添加至环境变量
```shell
vi ~/.bashrc
#openmpi-4.0.7
export PATH="/home/yqyang/software/openmpi-4.0.7-installed/bin:"$PATH
export LD_LIBRARY_PATH="/home/yqyang/software/openmpi-4.0.7-installed/lib:"$LD_LIBRARY_PATH
source ~/.bashrc
```
（4）查看安装成功与否
```shell
mpirun --version
```

### 编译安装plumed
**这里安装2.8版本的plumed**
（1）官网上下载对应型号和版本的安装包：https://www.plumed.org/download    
（2）解压缩编译安装
```shell
mkdir plumed2-2.8.0-installed
tar -zxvf plumed2-2.8.0
cd plumed2-2.8.0
./configure --prefix=/home/yqyang/software/plumed2-2.8.0-installed/  
make -j10
make install
```
（3）添加至环境变量
```shell
vi ~/.bashrc
#plumed2-2.8.0
export PATH="/home/yqyang/software/plumed2-2.8.0-installed/bin:"$PATH
export LD_LIBRARY_PATH="/home/yqyang/software/plumed2-2.8.0-installed/lib:"$LD_LIBRARY_PATH
source ~/.bashrc
```
（4）查看安装成功与否
```shell
plumed help
```
（5）将plumed插件嵌入gromacs中，切换目录至解压缩后的gromacs文件夹中，运行：
```shell
cd gromacs-2021.5
plumed patch -p
# 选择最接近的版本即可
```

### 编译安装gromacs
**这里安装2021.5版本的gromacs**  
（1）官网上下载对应型号和版本的安装包：https://manual.gromacs.org/2021.5/download.html      
（2）解压缩编译安装
```shell
mkdir gromacs-2021.5-installed
tar -zxvf gromacs-2021.5.tar.gz
cd gromacs-2021.5
mkdir build
cd build
CC=/home/yqyang/software/gcc-9.5.0-installed/bin/gcc CXX=/home/yqyang/software/gcc-9.5.0-installed/bin/g++ cmake ../ -DCMAKE_INSTALL_PREFIX=/home/yqyang/software/gromacs-2021.5-installed/ -DGMX_GPU=CUDA -DGMX_BUILD_OWN_FFTW=on -DGMX_MPI=on  
make -j10
make install
```
（3）添加至环境变量
```shell
vi ~/.bashrc
#gromacs
export PATH="/home/yqyang/software/gromacs-2021.5-installed/bin:"$PATH
export LD_LIBRARY_PATH="/home/yqyang/software/gromacs-2021.5-installed/lib64:"$LD_LIBRARY_PATH
source ~/.bashrc
```
（4）查看安装成功与否
```shell
gmx_mpi --version
```
### 安装时需注意的问题
* **一定要完全退出Conda环境后再进行编译安装，否则可能报错。**


## conda简洁安装
**（1）首先需要安装合适版本的gcc、cmake、openmpi、cuda和plumed**  
**（2）另外由于gcc高版本自带的库版本不符合要求，所以同时加上了需要的sysroot_linux-64的版本。这样创建环境的好处是即使这几个包版本不兼容，也可以一起装在一个环境里面。**  
### conda一键安装依赖软件
```shell
conda create -n gcc_8_5 -c conda-forge gxx_linux-64=8.5.0 gcc_linux-64=8.5.0 sysroot_linux-64=2.17 python=3.8 openmpi
conda activate gcc_8_5
```
### 安装gromacs软件
```shell
tar xvf gromacs-2022.tar.gz
cd gromacs-2022
mkdir build
cd build
cmake ../ -DCMAKE_INSTALL_PREFIX=/home/chpeng/software/gmx-2022 -DGMX_GPU=CUDA -DGMX_BUILD_OWN_FFTW=on -DGMX_MPI=on
make -j10
make install
```

## 浙定28服务器上安装Gromacs
（1）首先安装fftw。
```shell
mkdir fftw-3.3.10-installed
tar -zxvf fftw-3.3.10.tar.gz
cd fftw-3.3.10/
./configure --enable-float --enable-sse2 --enable-avx2 --enable-shared --prefix=/public/home/yqyang/software/fftw-3.3.10-installed    # --enable-float 单精度
make -j10
make install -j10
```
（2）安装gromacs。
```shell
mkdir gromacs-2021.5-installed
tar -zxvf gromacs-2021.5.tar.gz
cd gromacs-2021.5
mkdir build
cd build
cmake .. -DGMX_FFT_LIBRARY=fftw3 -DFFTWF_LIBRARY=/public/home/yqyang/software/fftw-3.3.10-installed/lib/libfftw3f.so -DFFTWF_INCLUDE_DIR=/public/home/yqyang/software/fftw-3.3.10-installed/include -DGMX_GPU=CUDA -DCUDA_TOOLKIT_ROOT_DIR=/usr/local/cuda -DCMAKE_INSTALL_PREFIX=/public/home/yqyang/software/gromacs-2021.5-installed
make -j10
make install -j10
```