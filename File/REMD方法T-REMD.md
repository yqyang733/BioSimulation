👏 REMD方法|T-REMD

---
[TOC]

---
## 摘要
&emsp;&emsp;文中为副本交换分子动力学算法开发了公式。文章使用一个五肽甲硫脑啡肽测试了该方法对于蛋白折叠问题的有效性。这个方法在多个温度下对体系进行平行模拟并通过交换的方式克服多重最小值问题。

## 研究背景
&emsp;&emsp;在蛋白折叠模拟中，由于低温的模拟容易陷入大量的局部最小值状态，所以通过常规的方法在低温下模拟很难获得准确的标准分布。克服这个多最小值问题的一种方法是基于非玻尔兹曼概率权重因子进行模拟以至于能够在能量空间中随机游走。与传统方法比，随机游走允许模拟翻越能垒对更广的相空间进行采样。


## 参考文献
1. Sugita Y, Okamoto Y. Replica-exchange molecular dynamics method for protein folding. Chemical physics letters, 1999, 314: 141-151. [文献pdf](REMD方法T-REMD/1-s2.0-S0009261499011239-main.pdf)

## 操作教程
1. [Tutorial on setting up the Replica Exchange Molecular Dynamics (REMD) simulation using alanine dipeptide as a toy model](REMD方法T-REMD/REMD_Tutorial.pdf)