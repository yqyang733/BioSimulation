👏 Plumed-gromacs应用

---
[TOC]

---
[Plumed官网](https://www.plumed.org/doc-v2.7/user-doc/html/index.html)
[Plumed教程-pdf](Plumed-gromacs应用/manual.pdf)

## 安装
### 浙定28上安装
（1）安装plumed
```shell
mkdir plumed-2.8.1-installed
tar zxvf plumed-2.8.1.tgz
cd plumed-2.8.1
./configure -h
CC=/bin/gcc CXX=/bin/g++ ./configure --prefix=/public/home/yqyang/software/plumed-2.8.1-installed
make -j10
make install

# plumed
export PATH="/public/home/yqyang/software/plumed-2.8.1-installed/bin:"$PATH
export LD_LIBRARY_PATH="/public/home/yqyang/software/plumed-2.8.1-installed/lib:"$LD_LIBRARY_PATH
```
（2）安装gromacs
```shell
mkdir gromacs-2021.5-plumed-installed
tar zxvf gromacs-2021.5.tar.gz
cd gromacs-2021.5
plumed patch -p
mkdir build 
cd build
CC=/bin/gcc CXX=/bin/g++ cmake .. -DGMX_FFT_LIBRARY=fftw3 -DFFTWF_LIBRARY=/public/home/yqyang/software/fftw-3.3.10-installed/lib/libfftw3f.so -DFFTWF_INCLUDE_DIR=/public/home/yqyang/software/fftw-3.3.10-installed/include -DGMX_GPU=CUDA -DCUDA_TOOLKIT_ROOT_DIR=/usr/local/cuda -DCMAKE_INSTALL_PREFIX=/public/home/yqyang/software/gromacs-2021.5-plumed-installed
make -j10
make install
```

## HREMD

