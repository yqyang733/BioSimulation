👏 Plumed-gromacs应用

---
[TOC]

---
[Plumed官网](https://www.plumed.org/doc-v2.7/user-doc/html/index.html)
[Plumed教程-pdf](Plumed-gromacs应用/manual.pdf)

## 安装

**注意事项：一般安装plumed，需要安装mpi，并且需要在mpi下先编译安装plumed，这样plumed就可以支持mpi，然后再用mpi的plumed去安装mpi版的Gromacs。**

### 浙定28上安装
（1.1）安装plumed（没有mpi）
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
（1.2）安装plumed（有mpi，要用mpi的C++编译）
```shell
mkdir plumed-2.8.1-installed_mpi
tar zxvf plumed-2.8.1.tgz
cd plumed-2.8.1
CC=/public/software/mpi/openmpi/4.1.4/bin/mpicc CXX=/public/software/mpi/openmpi/4.1.4/bin/mpic++ ./configure --prefix=/public/home/yqyang/software/plumed-2.8.1-installed_mpi
make -j10
make install
```

（2.1）安装gromacs-plumed
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
（2.2）安装gromacs-plumed-mpi (2023.1的没装上，2022.3的装上了。)
```shell
CC=/bin/gcc CXX=/bin/g++ cmake .. -DGMX_FFT_LIBRARY=fftw3 -DFFTWF_LIBRARY=/public/home/yqyang/software/fftw-3.3.10-installed/lib/libfftw3f.so -DFFTWF_INCLUDE_DIR=/public/home/yqyang/software/fftw-3.3.10-installed/include -DGMX_GPU=CUDA -DCUDA_TOOLKIT_ROOT_DIR=/usr/local/cuda -DCMAKE_INSTALL_PREFIX=/public/home/yqyang/software/gmx_2301_plumed-installed -DGMX_MPI=ON # -DMPI_CXX_COMPILER=/opt/mpich/3.4.2/bin/mpicxx -DMPI_C_COMPILER=/opt/mpich/3.4.2/bin/mpicc 前面放CC和CXX这两个可以不放，前面不放，这里要放。

CC=/public/home/yqyang/software/gcc-11.1.0-installed/bin/gcc CXX=/public/home/yqyang/software/gcc-11.1.0-installed/bin/g++ cmake .. -DGMX_FFT_LIBRARY=fftw3 -DFFTWF_LIBRARY=/public/home/yqyang/software/fftw-3.3.10-installed/lib/libfftw3f.so -DFFTWF_INCLUDE_DIR=/public/home/yqyang/software/fftw-3.3.10-installed/include -DGMX_GPU=CUDA -DCUDA_TOOLKIT_ROOT_DIR=/usr/local/cuda -DCMAKE_INSTALL_PREFIX=/public/home/yqyang/software/gmx_2203_plumed-installed -DGMX_MPI=ON
```

## HREMD

