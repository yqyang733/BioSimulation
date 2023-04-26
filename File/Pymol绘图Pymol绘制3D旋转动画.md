👏 Pymol绘图|Pymol绘制3D旋转动画

---

[封面PPT](../PPT/Pymol绘图Pymol绘制3D旋转动画.pptx)

---

[TOC]

---

## 1. 载入蛋白

Pymol中导入蛋白，在Pymol中调好蛋白的位置。

## 2. Pymol中命令行操作

```python
mset 1 x180 #由目标分子复制生成180帧（frame）相同的状态（state）;
util.mroll 1, 180, 1 #以当前视野的Y轴旋转分子，得到180个state;
OR util.mrock(1,60,10,1,1)  # issues mdo commands to create +/- 10 deg. rock over 60 frames
set ray_shadows, off  #关掉立体光线阴影
Movie >> 勾选Ray Trace Frames; OR set ray_trace_frames=1
File >> Save Movie As >> PNG Images，选择路径，输入name，会自动保存180个分子旋转过程中的图片，png格式;
OR mpng E:/path/name  #保存所有states的png图片
另，File >> Save session，保存pse文件以备后续使用。
```

## 3. 生成GIF动态图

**(1) Linux下使用convert命令生成**

将所有png图片上传到服务器，在LINUX环境下，运用convert命令生成动画图片，gif格式。

```bash
convert  *.png  t_rotate.gif  #convert功能十分强大，可另行学习各种图片编辑和转换操作
```

**(2) Python脚本生成**

参考文章 [Python绘图|Python将多张静态图片合成gif动态图片](./Python绘图Python将多张静态图片合成gif动态图片.md)

## 4. 裁剪尺寸

PPT中裁剪gif动图成自己想要的尺寸即可。
