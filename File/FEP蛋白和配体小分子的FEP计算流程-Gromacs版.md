👏 FEP|蛋白和配体小分子的FEP计算流程-Gromacs版

---
[TOC]

---
## 安装pmx-python3.7
（1）pmx的github地址：https://github.com/deGrootLab/pmx
（2）安装python3.6
```shell
conda create --name pmx python=3.6
```
（3）安装依赖环境
```shell
conda install numpy
conda install scipy
conda install matplotlib
```
（4）克隆安装pmx
```shell
git clone https://github.com/deGrootLab/pmx pmx
cd pmx
git checkout develop
python setup.py install
```

## 安装pmx-python2.7
（1）pmx的github地址：https://github.com/deGrootLab/pmx
（2）安装python2.7
```shell
conda create --name pmx-2.7 python=2.7
```
（3）安装依赖环境
```shell
conda install numpy
conda install scipy
conda install matplotlib
```
（4）克隆安装pmx
```shell
git clone https://github.com/deGrootLab/pmx pmx
cd pmx
$env/python setup.py install  # often meet error, need to specify the absolute path of python used.
```