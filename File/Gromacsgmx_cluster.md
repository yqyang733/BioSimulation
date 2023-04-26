👏 Gromacs|gmx cluster

---
[TOC]

---
## 简介
对结构进行团簇分析。

## 格式
```text
gmx cluster [-f [<.xtc/.trr/...>]] [-s [<.tpr/.tpb/...>]] [-n [<.ndx>]]
    [-dm [<.xpm>]] [-om [<.xpm>]] [-o [<.xpm>]] [-g [<.log>]]
    [-dist [<.xvg>]] [-ev [<.xvg>]] [-conv [<.xvg>]] [-sz [<.xvg>]]
    [-tr [<.xpm>]] [-ntr [<.xvg>]] [-clid [<.xvg>]]
    [-cl [<.xtc/.trr/...>]] [-nice ] [-b ] [-e ]
    [-dt ] [-tu ] [-[no]w] [-xvg ] [-[no]dista]
    [-nlevels ] [-cutoff ] [-[no]fit] [-max ]
    [-skip ] [-[no]av] [-wcl ] [-nst ]
    [-rmsmin ] [-method ] [-minstruct ]
    [-[no]binary] [-M ] [-P ] [-seed ] [-niter ]
    [-nrandom ] [-kT ] [-[no]pbc]
```

## 参数
**输入/输出文件选项**
|参数|默认值|类型|说明|
|:----|:----|:----|:----|
|-f [<.xtc/.trr/...>]|traj.xtc|输入，可选|轨迹：xtc trr cpt trj gro g96 pdb tng|
|-s [<.tpr/.tpb/...>]|topol.tpr|输入，可选|结构+质量(db)：tpr tpb tpa gro g96 pdb brk ent|
|-n [<.ndx>]|index.ndx|输入，可选|索引文件。|
|-dm [<.xpm>]|rmsd.xpm|输入，可选|X PixMap兼容的矩阵文件。|
|-om [<.xpm>]|rmsd-raw.xpm|输出|X PixMap兼容的矩阵文件。|
|-o [<.xpm>]|rmsd-clust.xpm|输出|X PixMap兼容的矩阵文件。|
|-g [<.log>]|cluster.log|输出|日志文件。|
|-dist [<.xvg>]|rmsd-dist.xvg|输出，可选|xvgr/xmgr文件。|
|-ev [<.xvg>]|rmsd-eig.xvg|输出，可选|xvgr/xmgr文件。|
|-conv [<.xvg>]|mc-conv.xvg|输出，可选|xvgr/xmgr文件。|
|-sz [<.xvg>]|clust-size.xvg|输出，可选|xvgr/xmgr文件。|
|-tr [<.xpm>]|clust-trans.xpm|输出，可选|X PixMap兼容的矩阵文件。|
|-ntr [<.xvg>]|clust-trans.xvg|输出，可选|xvgr/xmgr文件。|
|-clid [<.xvg>]|clust-id.xvg|输出，可选|xvgr/xmgr文件。|
|-cl [<.xtc/.trr/...>]|clusters.pdb|输出，可选|轨迹：xtc trr cpt trj gro g96 pdb tng|

**控制选项**
|参数|默认值|说明|
|:----|:----|:----|
|-nice \<int>|19|设置优先级。|
|-b|0|从轨迹文件中读取的第一帧(ps)。|
|-e|0|从轨迹文件中读取的最后一帧(ps)。|
|-dt|0|只使用t除以dt的余数等于第一帧时间(ps)的帧, 即两帧之间的时间间隔。|
|-tu \<enum>|ps|时间单位：fs，ps，ns，us，ms，s|
|-[no]w|no|程序结束后查看输出的.xvg，.xpm，.eps和.pdb文件。|
|-xvg \<enum>|xmgrace|xvg绘图格式：xmgrace，xmgr，none。|
|-[no]dista|no|利用RMSD距离而不是RMS偏差。|
|-nlevels \<int>|40|离散化RMSD矩阵时使用的水平数。|
|-cutoff \<real>|0.1|定义两个近邻结构所用的RMSD截断距离(nm)。|
|-[no]fit|yes|计算RMSD之前使用最小二乘叠合结构。|
|-max \<real>|-1|RMSD矩阵的最高水平。|
|-skip \<int>|1|每nr帧分析一次。|
|-[no]av|no|输出每一团簇的平均等中间结构。|
|-wcl \<int>|0|将此数目团簇的结构输出到编号文件中。|
|-nst \<int>|1|每个团簇中的结构数超过此数时才会输出团簇中的所有结构。|
|-rmsmin \<real>|0|输出结构与其余团簇的最小rms差异。|
|-method \<enum>|linkage|团簇的确定方法：linkage，jarvis-patrick，monte-carlo，diagonalization，gromos。|
|-minstruct \<int>|1|.xpm文件中着色团簇具有的最小结构数。|
|-[no]binary|no|将RMSD矩阵视为由0和1组成的矩阵, 截断由-cutoff给出。|
|-M \<int>|10|Jarvis-Patrick算法中使用的最近近邻数, 取0时使用截断。|
|-P \<int>|3|形成团簇需要的全同最近近邻数。|
|-seed \<int>|1993|蒙特卡洛团簇化算法的随机数种子: <= 0 代表生成。|
|-niter \<int>|10000|蒙特卡洛的迭代次数。|
|-nrandom \<int>|0|蒙特卡洛的第一次迭代可以完全随机, 以对帧进行混洗。|
|-kT \<real>|0.001|蒙特卡洛优化中使用的Boltzmann权重因子(取0时会关闭上升步骤)。|
|-[no]pbc|yes|PBC检查。|

## 常规使用方法
（1）gmx cluster可以使用几种不同的方法团簇化结构。结构之间的距离可由轨迹来确定，或使用-dm选项从.xpm矩阵文件读取。结构间的距离可以由叠合后的RMS偏差或原子对距离的RMS偏差来定义。

（2）确定团簇的方法有以下几种：
* single linkage(单连接)：当一个结构到团簇中任何一个原子的距离小于cutoff时，就将此结构加入到团簇中。
* Jarvis Patrick：当一个结构和团簇中的一个结构互为近邻结构，并且至少有P个相同的邻近结构时，将这个结构加入到团簇中。一个结构的近邻结构是指距离它最近的M个结构或在cutoff以内的全部结构。
* Monte Carlo(蒙特卡洛)：利用蒙特卡洛方法重新排列RMSD矩阵，以使帧的排列具有尽可能小的递增顺序。这样做可以使从一个结构到另一个结构的动画尽量平滑，并且结构之间的具有最大的可能(例如)RMSD，但中间步骤应尽可能小。这种方法可用于显示模拟的平均力势能系综或牵引模拟。显然用户要仔细地准备轨迹文件(例如不能存在叠加的帧)。最终的结果可以通过.xpm矩阵文件进行直观的检查，此文件从下到上都应该平滑地变化。
* diagonalization(对角化)：对角化RMSD矩阵。
* gromos：利用Daura等介绍的算法(Angew. Chem. Int. Ed. 1999, 38, pp 236-240)。使用截断来数算近邻结构的个数，把具有最多近邻的结构及其所有近邻作为一个团簇，并从团簇池中将这个团簇移除。然后对团簇池中剩下的结构重复以上算法。

当团簇化算法(single linkage，Jarvis Patrick和gromos) 将每个结构都精确地分配到了一个团簇，并且提供了轨迹文件时，在每一个团簇中相对于其他结构或平均结构或所有结构拥有最小平均距离的结构将被写入到轨迹文件中。当输出所有结构时对每个团簇会使用单独编号的文件。

（3）程序总会给出两个输出文件：
* **-o：** 输出矩阵左上半区域的RMSD值，团簇图像的右下半区域。当-minstruct = 1时，若两个结构属于同一团簇，相应的图像点为黑色；当-minstruct > 1时对每一个团簇使用不同的颜色。
* **-g：** 输出所用选项的信息和所有团簇及其成员的详细列表。

（4）此外，程序也可以给出多个可选的输出文件：
* -dist：输出RMSD的分布
* -ev：输出RMSD矩阵对角化的特征向量
* -sz：输出团簇的大小
* -tr：输出两个团簇之间的转变次数矩阵
* -ntr：输出从/到每个团簇的总转变次数
* -clid：输出团簇数随时间变化的函数
* -cl：输出每个团簇的平均(利用-av选项)或中心结构，或对所选的一组团簇，将团簇成员输出到带编号的文件(利用-wcl选项，取决于-nst和-rmsmin选项)。一个团簇的中心是指团簇中与所有其他结构具有最小平均RMSD的结构。

## 源码位置及解析
gromacs-2022.2/src/gromacs/gmxana/gmx_cluster.cpp

## 示例
（1）将动力学轨迹进行聚类。聚类之前最好先处理周期性。
```shell
gmx cluster -s npt.pdb -f md.xtc -g -dist -sz -clid -cl -method linkage -cutoff 0.2
```