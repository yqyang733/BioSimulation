临时笔记

---

学习李沐的动手学深度学习书和视频的临时笔记。

1. 卷积神经网络是空间上的神经网络，循环神经网络是时间上的神经网络
2. tensor是一个数学上的概念，array是一个计算机上的概念
3. batchsize的设置大或者小，总的计算量是一样的，但是会影响并行度。设置batchsize到一定值会让并行利用度最高，所以会在一定程度上加快速度。
4. 深度学习与药物设计的领域之中，过拟合并不见得就一定是一件坏事。就算没有找到统一的规律，但是分子间的相似性却可能使得获得活性更强的分子。结构相似，边链不同可能活性就会更强。
5. label之间差距减小会减少过拟合的风险，并且会缩小权重的数值。因为可以试想，标签间的差距非常大的时候，点与点之间的上下波动就会变得非常大。这样拟合的曲线波折会更多一些，权重值会更大，过拟合的风险也会更大。特别是对于回归模型来说。
6. 设计模型超参数的时候可以采用随机的做法。把所有的可能的超参数组合列出来，随机100次，挑最好的一次就可以。
7. 训练好模型之后，最后选取模型进行预测的三种做法。1、先用k折交叉验证确定超参数，然后拿全部的数据训练一遍，确定最后的模型来预测。2、直接拿k折里面最好的那个模型来做预测。3、拿所有的k折模型都预测一遍，取均值。
8. SVM和深度学习的比较：svm不用调参且有数学理论基础。深度学习没有理论基础但是实际应用效果非常好。
9. vc维可以简单的理解为模型能记住的数据的多少和特征。
10. 艺术、工程、科学。深度学习是一种艺术。例如蒸汽机先被发明，过了100年热力学才出现。
11. 权重衰退的一般选择参数值：e-2、e-3、e-4。
12. 对于CPU或者GPU来讲的话，做乘法远远比选出来一些元素来的更快。所以不使用a[:]=0，而是用矩阵的乘法来将矩阵中的一些元素设为零。
13. 随机性是件好事。随机性可以让模型变得更加平滑。
14. BN是给卷积层用的，dropout是给全连接层用的。
15. dropout是一个正则项，正则项的唯一一个作用是在更新权重的时候让模型的复杂度变低一点点。
16. 过拟合一点没关系，后面会想办法让过拟合减轻。过拟合会使模型的强度非常强。
17. 学习率会和期望和方差敏感一点。
18. Xavier权重初始化的方差是由输入和输出的维度来定的。
19. 现在很多新的网络都是为了缓解数值稳定性的问题。
20. adam优化器比SGD优化器的好处是对学习率没有那么敏感。
21. nn.module的方法比sequence方法更灵活。

---

技术方面的临时笔记

### 问题背景：

1. if [ "$frut" == '2' ]; then
2. mode='--run'
3. else
4. mode=''
5. fi

如上代码, 执行shell报错：-bash: [: ==: 期待一元表达式

### 解决方法：

看见 $frut 变量了吗，必须加上引号，因为这里用的是 [ ] 条件修饰符; 所以反过来如果用了 [[]] 条件修饰符就没问题了

---

4**.决策树的验证**

**A.Y-Scrambling**

以随机的方式把25个 PPI抑制剂的分类标签重新分配到数据集上，再尝试用混杂的活性数据构建决策树。如果分类规则是基于机会相关性的，那么产生的随机数据集应是此模型与基于真实数据的模型相似的。但置换测试的结果显示，在所有情况下所获得的模型都没有任何有用的预测能力，且这些模型无法可靠地识别数据集中的真阳性。

一般的办法是：  
（1）先使用正确的数据，正确的标签训练一个模型1，使用正确数据、正确标签对模型1进行评估。  
（2）第二次将数据标签打乱再训练一个模型2，使用正确数据、正确标签对模型2进行评估。  
（3）如果两个模型的评估结果近似，则模型1基本没啥预测性能。如果模型1的评估结果远远好于模型2，则模型1是好用的。  

---


---
周期性边界盒子的解释：
![](temp/temp0.png)
截断半径
![](temp/temp1.png)

---
找时间写一下自由能景图的生成的python脚本

---
Python dict字典update()方法

update() 方法可使用一个字典所包含的键值对来更新己有的字典。

在执行 update() 方法时，如果被更新的字典中己包含对应的键值对，那么原 value 会被覆盖；如果被更新的字典中不包含对应的键值对，则该键值对被添加进去。

例如：
```python
a = {'one': 1, 'two': 2, 'three': 3}
a.update({'one':4.5, 'four': 9.3})
print(a)
```
运行结果为：
> {'one': 4.5, 'two': 2, 'three': 3, 'four': 9.3}

从上面的执行过程可以看出，由于被更新的 dict 中已包含 key 为“one”的键值对，因此更新时该键值对的 value 将被改写；但如果被更新的 dict 中不包含 key 为“four”的键值对，那么更新时就会为原字典增加一个键值对。


绘制热图：
1. https://blog.csdn.net/weixin_42555080/article/details/121108340



2012年至今，细数深度学习领域这些年取得的经典成果
https://baijiahao.baidu.com/s?id=1674272525338770378&wfr=spider&for=pc

https://blog.csdn.net/u014134327/article/details/110930011

Linux终端连接服务器打开薛定谔。
```shell
ssh -X g16@172.21.85.11
ssh g16@172.21.85.11
```


## torch.nn.BCELoss() 与F.binary_cross_entropy
(1) https://www.freesion.com/article/18531252901/  
(2) https://blog.csdn.net/laolu1573/article/details/82925747  
(3) 


## RDKit
(1) https://blog.csdn.net/qq_36801966/article/details/106432725  
(2) https://www.manongdao.com/article-1684891.html  
(3) https://blog.csdn.net/weixin_39417324/article/details/122334990  
(4) https://www.rdkit.org/docs/cppapi/classRDKit_1_1Atom.html#a8a82ae947ebbbc48f5ec5128f5c3e724  
(5) https://www.rdkit.org/docs/cppapi/classRDKit_1_1Bond.html#ae91dd8e72b495a48f46775c874882165  


git pull和git fetch。  
(1) https://www.cnblogs.com/runnerjack/p/9342362.html  



分子对接专辑：  
smina:  
参数文件中num_modes可以设置输出构象的个数。  

蛋白口袋探测软件：  
![](temp/temp_2022-03-04-22-22-40.png)


(1) 想办法把分子指纹摩根算法那篇用Python复现出来。  
(2) 弄明白生成小分子化合物邻接矩阵的内在原理。  
(3) 如何去看RDKit的源码？？  


分子对接文献：   
(1) https://mp.weixin.qq.com/s/50fSgOAV-ZdabXETGBx2_A  


python压缩gif图片：https://blog.csdn.net/weixin_41855010/article/details/120723943  

根据SMD计算PMF的方法与公式。

VMD:
![](temp/temp_2022-10-09-18-40-42.png)

vmdrc
![](temp/temp_2022-10-09-19-30-57.png)

关于RNA适配体：
1. 小分子药效团的概念，可以在RNA的结构中引入类似药效团的概念吗？例如固定的RNA片段形成的固定的3D结构是否对特定的氨基酸片段有特定的偏好性？

Smina:
https://sourceforge.net/p/smina/code/ci/master/tree/  


位置限制：
1. ![](temp/temp_2022-10-13-13-07-11.png)
2. https://blog.csdn.net/CocoCream/article/details/126116755
3. http://sobereva.com/10?ivk_sa=1024320u
4. https://manual.gromacs.org/current/reference-manual/topologies/topology-file-formats.html?highlight=intermolecular_interactions
5. http://bbs.keinsci.com/thread-15380-1-2.html
6. http://bpg.utoledo.edu/gromacs/online/mdp_opt.html
7. https://manual.gromacs.org/current/onlinehelp/gmx-genrestr.html


pymol index:
1. https://pymolwiki.org/index.php/Index

pymol id:
1. https://pymolwiki.org/index.php/Id_Atom

pymol Identify:
1. https://pymolwiki.org/index.php/Identify

FEP-setup: gxf
https://github.com/gxf1212/FEbuilder

NAMD配置文件：http://www.ks.uiuc.edu/Training/Tutorials/namd/namd-tutorial-unix-html/node26.html

psfgen User’s Guide：https://www.ks.uiuc.edu/Research/vmd/plugins/psfgen/ug.pdf


NAMD or FEP
1. http://zarbi.chem.yale.edu/ligpargen/namd_tutorial.html
2. https://github.com/quantaosun/NAMD-MD
3. https://github.com/quantaosun/making-it-rain
4. https://gxf1212.github.io/notes/#/MD/FYP-notes
5. http://mackerell.umaryland.edu/charmm_ff.shtml#charmm
6. 



VMD 处理周期性问题：
https://www.ks.uiuc.edu/Research/vmd/plugins/pbctools/

[Schrödinger(薛定谔) 入门级Q&A](https://zhuanlan.zhihu.com/p/401697711)

qsub
1. https://zhuanlan.zhihu.com/p/500313444
2. https://www.csdn.net/tags/MtjaQg4sNzQ5MDUtYmxvZwO0O0OO0O0O.html
3. 


利用python自动向动态网页提交信息：https://blog.csdn.net/weixin_43336305/article/details/125943932

https://aip.scitation.org/doi/10.1063/5.0021399


[分子间相互作用力——氢键，卤键，硫键，π-π堆积，盐桥，阳离子-π，疏水作用力](https://blog.csdn.net/qq_37728400/article/details/115908457)


VMD：biliili:
移动分子：
![](temp/temp_2022-10-22-16-46-05.png)
![](temp/temp_2022-10-22-16-48-25.png)
![](temp/temp_2022-10-22-16-50-48.png)
![](temp/temp_2022-10-22-16-51-54.png)
![](temp/temp_2022-10-22-16-53-14.png)
![](temp/temp_2022-10-22-16-54-12.png)
![](temp/temp_2022-10-22-16-55-00.png)
![](temp/temp_2022-10-22-16-55-30.png)
![](temp/temp_2022-10-22-16-59-11.png)
![](temp/temp_2022-10-22-16-59-46.png)
![](temp/temp_2022-10-22-17-03-18.png)
![](temp/temp_2022-10-22-17-07-05.png)
![](temp/temp_2022-10-22-17-08-30.png)
![](temp/temp_2022-10-22-17-09-31.png)
![](temp/temp_2022-10-22-17-10-05.png)
![](temp/temp_2022-10-22-17-10-54.png)

![](temp/temp_2022-10-23-16-22-09.png)
![](temp/temp_2022-10-23-16-24-02.png)
![](temp/temp_2022-10-23-16-26-03.png)
![](temp/temp_2022-10-23-16-27-08.png)
![](temp/temp_2022-10-23-16-27-50.png)
![](temp/temp_2022-10-23-16-29-52.png)
![](temp/temp_2022-10-23-16-30-54.png)
![](temp/temp_2022-10-23-16-32-09.png)
![](temp/temp_2022-10-23-16-33-32.png)



编译Gromacs-Colvars进行自由能计算:
http://bbs.keinsci.com/thread-19967-1-1.html

https://viennarna.github.io/forgi/graph_tutorial.html

http://nibiru.tbi.univie.ac.at/forna/

![](temp/temp_2022-11-04-20-07-14.png)
https://blog.csdn.net/sinat_41858359/article/details/126212975

李沐：动手学深度学习：
https://courses.d2l.ai/zh-v2/

TIES MD is a package for the preparation, running and analysis of binding free energy calculations. In this document we will outline what commands should be run to calculate binding free energies. To start with any free energy calculations we must first outline what are the expected input files to the TIES MD program.
https://ucl-ccs.github.io/TIES_MD/tutorial.html

NAMD-FEP:
https://github.com/yqyang733/NAMD-FEP

alchemlyb Documentation
https://alchemlyb.readthedocs.io/_/downloads/en/latest/pdf/

he python package  is an implementation of the Common Core / Serial-Atom-Insertion (CC-SAI) approach1 for calculating free energy differences. It does so by connecting the physical endstates of two molecules via alchemical pathways.
https://wiederm.github.io/transformato/index.html


VMD cluster:
http://www-s.ks.uiuc.edu/Research/vmd/vmd-1.9.1/ug/node136.html

![](temp/temp_2022-12-06-20-27-07.png)
![](temp/temp_2022-12-06-20-42-11.png)

![](temp/temp_2022-12-06-20-57-02.png)


20221206
（1）核酸课题的FEP后分析。
（2）多肽设计的global聚类做完，并进行local docking并进行聚类。（想想怎么更准确的确定结合位点，很重要。）
（3）小分子I3C-1的FEP以及教程的制作。