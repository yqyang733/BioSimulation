👏 REMD方法|REST2

---
[TOC]

---
## 摘要
&emsp;&emsp;与[REST1](https://mp.weixin.qq.com/s/fZEhYSnKCRHRXsASY7nHNw)方法相似，本文中的REST2也是对每个副本的哈密顿函数进行变形使得副本交换时仅考虑溶质，忽略溶剂的自由度。案例验证表明REST2比REST1更节省计算时间，采样效率更高。

## 研究背景
&emsp;&emsp;复杂生物体系的构象分布地形图崎岖复杂，有盆地，也有包围盆地的高能垒，给构象采样带来了挑战。为解决此问题，TREM（Temperature Replica Exchange Method）通过设置多个副本温度执行副本交换提高采样效率，但是该方法副本数量与体系自由度平方根成正比，限制了该方法在复杂体系中的应用。为克服此问题，研究人员提出REST1用于减少副本数目，增强采样。但是后来发现其在模拟具有巨大构象变化的大体系中效果不如TREM。受两篇文章（[J. Chem. Theory Comput. 2011, 7, 231–237.](https://pubs.acs.org/doi/full/10.1021/ct100493v)和[J. Comput. Chem. 2011, 32, 1228–1234](https://onlinelibrary.wiley.com/doi/full/10.1002/jcc.21703)）提出的REFT方法启发，本文使用两篇文章中提出的新的哈密顿量修饰因子代替REST1中原始的修饰因子并使用测试体系对REST1、REST2和TREM进行了比较。


## 研究方法
&emsp;&emsp;REST1中体系的总能量分为三部分，分别是蛋白内部能量$E_{pp}$、蛋白-水之间能量$E_{pw}$和水内部能量$E_{ww}$。$T_m$温度下副本的势能如公式（2.1）所示。其中，$X$代表体系的坐标，$\beta_m=\frac{1}{k_BT_m}$，$T_0$是待研究的温度。  
$$
E_m^{REST1}=E_{pp}(X)+\frac{\beta_0+\beta_m}{2\beta_m}E_{pw}(X)+\frac{\beta_0}{\beta_m}E_{ww}(X)...(2.1)
$$

&emsp;&emsp;两个副本m和n之间的交换律取决于两个副本之间的能量差，如公式（2.2）所示。
$$
\Delta_{mn}(REST1)=(\beta_m-\beta_n)[(E_{pp}(X_n)+\frac{1}{2}E_{pw}(X_n))-(E_{pp}(X_m)+\frac{1}{2}E_{pw}(X_m))]...(2.2)
$$

&emsp;&emsp;REST1中不同副本具有不同的温度和势能，并且温度和势能的关系如公式（2.3）所示。REST2则是在同一温度下通过对哈密顿函数变形建立具有不同的势能面的多个副本从而实现副本交换。
$$
\frac{T_m}{T_0}=\frac{E_m}{E_0}...(2.3)
$$
在REST2中，所有副本都是在$T_0$温度下进行的，第m个副本的势能如公式（2.4）所示。REST1中蛋白构象的增强采样是通过增加蛋白温度使多个副本在整个蛋白势能面上交换实现的。REST2中增强采样则是通过使用比例因子（$\frac{\beta_m}{\beta_0}$）对蛋白势能进行缩放，使得不同副本中分隔不同蛋白构象的能垒不同程度降低，而后进行副本交换达到增强采样的目的。两种方法思路不同但是达到了相同的增强采样目的。REST2中为了与REST1相呼应，仍旧使用“温度”项来表示副本。
$$
E_m^{REST2}=\frac{\beta_m}{\beta_0}E_{pp}(X)+\sqrt{\frac{\beta_m}{\beta_0}}E_{pw}(X)+E_{ww}(X)...(2.4)
$$

&emsp;&emsp;REST2公式（2.4）中的相互作用能可以使用比例因子$\frac{\beta_m}{\beta_0}$、$\frac{\beta_m}{\beta_0}$和$\sqrt{\frac{\beta_m}{\beta_0}}$分别对键相互作用项、LJ势ε参数项和溶质原子电荷项进行缩放。并且注意对于溶质-溶剂间互作比例因子，REST2使用的是$\sqrt{\frac{\beta_m}{\beta_0}}$，而REST1使用的是$\frac{\beta_0+\beta_m}{2\beta_0}$。REST2使用的比例因子$\sqrt{\frac{\beta_m}{\beta_0}}$更自然的复合LJ势标准结合规范，该比例因子对REST2采样性能提升有重要作用。另外，文中发现缩放键长和键角项对采样没有帮助，因此实际操作只缩放了溶质键合相互作用中的二面角项，这使得溶质不同构象之间的转换更快。

&emsp;&emsp;REST2不同于REST1的另一方面是决定交换律的势能差公式，如公式（2.5）所示。
$$
\Delta_{mn}(REST2)=(\beta_m-\beta_n)[(E_{pp}(X_n)-E_{pp}(X_m))+\frac{\sqrt{\beta_0}}{\sqrt{\beta_m}+\sqrt{\beta_n}}(E_{pw}(X_n))-E_{pw}(X_m)]...(2.5)
$$

## 研究结果


## 参考文献
1. Wang L, Friesner RA, Berne BJ. Replica exchange with solute scaling: a more efficient version of replica exchange with solute tempering (REST2). J Phys Chem B. 2011, 115: 9431-9438. doi: 10.1021/jp204407d. [文献pdf](REMD方法REST2/jp204407d.pdf)

## 实操教程


**实操教程参考：**
1. [REST2](REMD方法REST2/REST2_Tutorial.pdf)