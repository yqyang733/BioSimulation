👏 vmd|绘制模拟盒子

---
[TOC]

---
## 命令
```text
pbc get   # 获取模拟盒子尺寸
set cell [pbc set {31.0 15.64 15.64} -all]   # 设置盒子边界。单位为埃。
pbc box -center origin -color black -width 1 
pbc box -on 
pbc wrap -all
```

---
## 参考资料
1. [如何在VMD中画出模拟盒子](https://www.bilibili.com/read/cv4466929/)
2. [VMD中自用的常规命令](https://zhuanlan.zhihu.com/p/397228030)