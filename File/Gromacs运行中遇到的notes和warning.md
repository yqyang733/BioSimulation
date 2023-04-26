👏 Gromacs|运行中遇到的notes和warning

---
[TOC]

---
## 异常现象
（1）em时水盒子是正常的，nvt的时候水盒子会发生形变，npt的时候水盒子形状又会恢复。
这种是正常现象，由于加的水的密度不够，nvt的时候会产生真空的气泡，npt的时候会再次自动调整水盒子大小。只要npt水盒子形状恢复则不会影响模拟。出现该现象后npt的盒子大小相较nvt的盒子会缩小。

## notes
（1）
![](Gromacs运行中遇到的notes和warning/Gromacs运行中遇到的notes和warning_2022-10-14-10-42-06.png)
解决办法：
* nstcomm: 每多少步消除一次整体运动，默认100步。
* nstcalcenergy = 100；计算能量的频率，默认为100。应该和nstlist相同或者是nstlist的倍数。
* nstlist：定义更新邻居列表的频率(默认为10)。1表示每步都更新，nstlist= 1。0:用于真空模拟；-1: 自动。
* nstenergy：每多少步输出一次能量信息到.edr文件，默认为1000。应是计算能量频率(由nstcalcenergy设定。默认100)的倍数.
