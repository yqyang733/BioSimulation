👏 Pymol|Pymol图形界面操作

---
[TOC]

---
## 基本操作
### pymol中执行python脚本
打开Pymol在命令行中输入如下python脚本命令。    
```python
python   # 唤醒pymol中的python
for i in range(10):
    print(i)    # 输入python脚本时可以直接复制大量脚本内容到pymol，pymol会自动对其进行换行。
python end   # 最后必须输入python end才能运行上述脚本内容。
```
### 分子移动
**打开编辑模式或者Builder模块**
![](PymolPymol图形界面操作/PymolPymol图形界面操作_2022-11-27-16-56-06.png)
|操作|效果|
|:----|:----|
|shift+left|分子原位旋转|
|shift+middle|分子平移|
|shift+right|分子原位缩放|
|ctrl+left|移动单个原子|
|ctrl+right|以键为轴进行旋转|

## 编辑三维结构-Builder

## 透明度设置相关
**透明模式设置：**
（1）Uni-layer; 点击菜单栏中的Setting->Transparency->Uni-layer
（2）Multi-Layer; 点击菜单栏中的Setting->Transparency->Multi-Layer
（3）Multi-Layer(real time oit); 点击菜单栏中的Setting->Transparency->Multi-layer(real time oit)
（4）Fast-ugly; 点击菜单栏中的Setting->Transparency->Fast-ugly
![](PymolPymol图形界面操作/PymolPymol图形界面操作_2022-11-09-18-29-22.png)

## 雾化处理
PyMOL中支持各种雾化处理，保证depth_cue是开启的。前面清晰，后面雾化可通过滚动鼠标中键进行调节。向上滚动，雾化程度减轻；向上滚动，雾化程度加深。如下图所示：
![](PymolPymol图形界面操作/PymolPymol图形界面操作_2022-11-09-18-31-19.png)
完全不想要雾化处理，可以点击 Display->Depth cue (Fogging) 取消。或者使用下述命令关闭。
```shell
set depth_cue, 0
```

## 光照模式
**（1）PyMOL中内置5种不同的光照模式：** default, metal(金属), plastic(塑料), rubber(橡胶), X-ray。点击Plugin->lighting Settings进行设置不同的光照，如下图所示图。
![](PymolPymol图形界面操作/PymolPymol图形界面操作_2022-11-09-18-32-46.png)
除了默认的5种模式外，你也可以通过设置光源的参数，达到自己想要的效果。下面，我们查看蛋白的静电势表面在不同光照模式下的效果。
![](PymolPymol图形界面操作/PymolPymol图形界面操作_2022-11-09-18-33-45.png)

**（2）Pymol渲染（Ray）** 设置Ray_trace_mode设置可以修改最终成图的时候的PyMOL的内部渲染蛋白的模式。
```shell
set ray_trace_mode, 0   # 普通颜色
set ray_trace_mode, 1   # 普通颜色+黑色线
set ray_trace_mode, 2   # 仅包含黑色线
set ray_trace_mode, 3   # 倍色+黑色线
set antialias, 2   # 建议设置
set ray_trace_color, magenta   # 更改线的颜色
```
参考：[Pymol渲染（Ray）笔记](https://www.jianshu.com/p/ba89f50e0c0c)

## atom id/rank/index
（1）ID (int): PDB atom id (not guaranteed to be unique)
（2）rank (int): atom index from original file import (not guaranteed to be unique)
（3）index (int): internal atom index (unique per object, sensitive to sorting and removing of atoms, cannot be altered)

## pymol自动记录界面操作命令行
![](PymolPymol图形界面操作/PymolPymol图形界面操作_2022-12-14-10-28-05.png)
![](PymolPymol图形界面操作/PymolPymol图形界面操作_2022-12-14-10-28-59.png)
![](PymolPymol图形界面操作/PymolPymol图形界面操作_2022-12-14-10-29-37.png)
![](PymolPymol图形界面操作/PymolPymol图形界面操作_2022-12-14-10-31-40.png)
![](PymolPymol图形界面操作/PymolPymol图形界面操作_2022-12-14-10-31-58.png)

## 核酸相关Show
### Examples of nucleic acid cartoons
参考链接：https://pymolwiki.org/index.php/Examples_of_nucleic_acid_cartoons

## 蛋白骨架以及侧链Show
### Cartoon side chain helper
效果如下：
```python
set cartoon_side_chain_helper, off            
set cartoon_side_chain_helper, 0              #off
```
![](PymolPymol图形界面操作/PymolPymol图形界面操作_2023-03-27-21-48-57.png)
```python
set cartoon_side_chain_helper, on             
set cartoon_side_chain_helper, 1              #on
```
![](PymolPymol图形界面操作/PymolPymol图形界面操作_2023-03-27-21-50-04.png)