👏 Pymol脚本|分子对接盒子可视化

---

[封面PPT](../PPT/Pymol脚本分子对接盒子可视化.pptx)

---

[TOC]

---

## 1. 脚本功能

在进行分子对接的时候，对接盒子的设置非常重要，特别是针对像smina或者autodock等的对接软件，需要输入对接中心坐标以及对接盒子的尺寸来进行对接。那么设置的对接盒子是否真的合适，是否包裹了要对接的位点呢？这个是个重要的问题，所以在本篇文章中就针对这个问题运用pymol脚本进行对接盒子的可视化，以方便用户调整对接盒子的大小进行精准的分子对接。

首先用户需要知道对接盒子的中心坐标和大致盒子的长宽高尺寸，后期可调整。如下：

> center_x = 14
> center_y = 28.5
> center_z = 29.3
> size_x = 10
> size_y = 10
> size_z = 10

## 2. 使用方式

(1) 先将下述脚本内容保存成box.py文件：

```python
import pymol
from pymol import cmd
from pymol.wizard import Wizard
from chempy import cpv
from pymol.cgo import *
from random import randint

def gridbox(center_x,center_y,center_z,size_x,size_y,size_z,name="gridbox",r1=0,g1=0,b1=1,trasp=0.2):
	center_x=float(center_x)
	center_y=float(center_y)
	center_z=float(center_z)
	size_x=float(size_x)
	size_y=float(size_y)
	size_z=float(size_z)
	r1=float(r1)
	g1=float(g1)
	b1=float(b1)
	trasp=float(trasp)
	
	p1=[center_x-size_x/2,center_y-size_y/2,center_z+size_z/2]
	p2=[center_x-size_x/2,center_y+size_y/2,center_z+size_z/2]
	p3=[center_x+size_x/2,center_y+size_y/2,center_z+size_z/2]
	p4=[center_x+size_x/2,center_y-size_y/2,center_z+size_z/2]
	p5=[center_x-size_x/2,center_y-size_y/2,center_z-size_z/2]
	p6=[center_x-size_x/2,center_y+size_y/2,center_z-size_z/2]
	p7=[center_x+size_x/2,center_y+size_y/2,center_z-size_z/2]
	p8=[center_x+size_x/2,center_y-size_y/2,center_z-size_z/2]
	obj=[
			 ALPHA,trasp,
			 COLOR,r1,g1,b1,
			 BEGIN, TRIANGLE_STRIP,
			 VERTEX,p1[0],p1[1],p1[2],
			 VERTEX,p2[0],p2[1],p2[2],
			 VERTEX,p4[0],p4[1],p4[2],
			 VERTEX,p3[0],p3[1],p3[2],
			 END,
			 
			 BEGIN, TRIANGLE_STRIP,
#			 COLOR,1,0,0,
			 VERTEX,p1[0], p1[1], p1[2],
			 VERTEX,p5[0], p5[1], p5[2],
			 VERTEX,p4[0], p4[1], p4[2],
			 VERTEX,p8[0], p8[1], p8[2],
			 END,
			 
			 BEGIN, TRIANGLE_STRIP,
			 VERTEX,p4[0],p4[1],p4[2],
			 VERTEX,p8[0],p8[1],p8[2],
			 VERTEX,p3[0],p3[1],p3[2],
			 VERTEX,p7[0],p7[1],p7[2],
			 END,
			 
			 
			 BEGIN, TRIANGLE_STRIP,
			 VERTEX,p7[0],p7[1],p7[2],
			 VERTEX,p3[0],p3[1],p3[2],
			 VERTEX,p6[0],p6[1],p6[2],
			 VERTEX,p2[0],p2[1],p2[2],
			 END,
			 
			 BEGIN, TRIANGLE_STRIP,
#			 COLOR,0,1,0,
			 VERTEX,p2[0],p2[1],p2[2],
			 VERTEX,p6[0],p6[1],p6[2],
			 VERTEX,p1[0],p1[1],p1[2],
			 VERTEX,p5[0],p5[1],p5[2],
			 END,
			 
			 BEGIN, TRIANGLE_STRIP,
			 VERTEX,p1[0],p1[1],p1[2],
			 VERTEX,p5[0],p5[1],p5[2],
			 VERTEX,p4[0],p4[1],p4[2],
			 VERTEX,p8[0],p8[1],p8[2],
			 END,
			 
			 BEGIN, TRIANGLE_STRIP,
			 VERTEX,p5[0],p5[1],p5[2],
			 VERTEX,p8[0],p8[1],p8[2],
			 VERTEX,p6[0],p6[1],p6[2],
			 VERTEX,p7[0],p7[1],p7[2],
			 END,
			 ]
	cmd.load_cgo(obj, name)

	minX = center_x-size_x/2
	maxX = center_x+size_x/2
	minY = center_y-size_y/2
	maxY = center_y+size_y/2
	minZ = center_z-size_z/2
	maxZ = center_z+size_z/2
	boundingBox = [
                LINEWIDTH, float(2),

                BEGIN, LINES,
                COLOR, float(1), float(0), float(0),

                VERTEX, minX, minY, minZ,
                VERTEX, minX, minY, maxZ,
                VERTEX, minX, maxY, minZ,
                VERTEX, minX, maxY, maxZ,
                VERTEX, maxX, minY, minZ,
                VERTEX, maxX, minY, maxZ,
                VERTEX, maxX, maxY, minZ,
                VERTEX, maxX, maxY, maxZ,
                VERTEX, minX, minY, minZ,
                VERTEX, maxX, minY, minZ,
                VERTEX, minX, maxY, minZ,
                VERTEX, maxX, maxY, minZ,
                VERTEX, minX, maxY, maxZ,
                VERTEX, maxX, maxY, maxZ,
                VERTEX, minX, minY, maxZ,
                VERTEX, maxX, minY, maxZ,
                VERTEX, minX, minY, minZ,
                VERTEX, minX, maxY, minZ,
                VERTEX, maxX, minY, minZ,
                VERTEX, maxX, maxY, minZ,
                VERTEX, minX, minY, maxZ,
                VERTEX, minX, maxY, maxZ,
                VERTEX, maxX, minY, maxZ,
                VERTEX, maxX, maxY, maxZ,

                END
        ]

	p4 = '_4' + str(randint(0, 100))
	p5 = '_5' + str(randint(0, 100))
	p6 = '_6' + str(randint(0, 100))
	p7 = '_7' + str(randint(0, 100))
	cmd.pseudoatom (pos=[minX, minY, minZ], object=p4)
	cmd.pseudoatom (pos=[minX, minY, maxZ], object=p5)
	cmd.pseudoatom (pos=[minX, maxY, minZ], object=p6)
	cmd.pseudoatom (pos=[maxX, minY, minZ], object=p7)
	cmd.distance(None, p4, p7)
	cmd.distance(None, p4, p6)
	cmd.distance(None, p4, p5)
	cmd.hide("nonbonded")

	boxName = "box_IABB_" + str(randint(0, 100))
	cmd.load_cgo(boundingBox,boxName)

cmd.extend('gridbox', gridbox)
```

(2) 在pymol中导入蛋白结构，运行下述命令：

```python
run D:/box.py
gridbox 14, 28.5, 29.3, 10, 10, 10, 0.7, 0.7, 0.7 ,0.2    # 参数解释：盒子中心为[14, 28.5, 29.3]，盒子尺寸为[10, 10, 10],盒子RGB颜色为[0.7, 0.7, 0.7],透明度为0.2。这些参数用户可自行调整。
```

(3) 运行之后即可得下图，可反复运行，调整到用户需要的一个盒子尺寸，再确定参数进行分子对接。

![img](https://pic2.zhimg.com/80/v2-8f96b3f2a766b3fd11765eac6bb73ecd_720w.jpg)



参考：

1. [高级教程 - PyMOL中文教程 2020.09 文档](https://link.zhihu.com/?target=http%3A//pymol.chenzhaoqiang.com/intro/advanceManual.html%23id4)
2. [https://raw.githubusercontent.com/Pymol-Scripts/Pymol-script-repo/master/Draw_Protein_Dimensions.py](https://link.zhihu.com/?target=https%3A//raw.githubusercontent.com/Pymol-Scripts/Pymol-script-repo/master/Draw_Protein_Dimensions.py)
3. [Pymol脚本|显示蛋白/小分子的长宽高](./Pymol脚本显示蛋白小分子的长宽高.md)