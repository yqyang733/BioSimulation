👏 AmberTools|cpptraj详解

---
[TOC]

---
## Amber轨迹文件提取蛋白动画
```shell
parm complex_solvated.prmtop
trajin md.crd 1000 5000 20      # 读取1000到5000帧，每隔20帧读一帧蛋白。
strip :Na+
strip :Cl-
strip :WAT
trajout md.pdb nobox    # 不输出盒子信息。
run
```

## 计算原子间距离
```shell
parm complex_solvated.prmtop
trajin md.crd
distance @1,2,3 @101,102,103 out d1.out      # 计算原子(123)质心与原子(101,102,103)质心之间的距离。
run
```

## 查看轨迹文件帧数
```shell
cpptraj -p complex_solvated.prmtop -y md.crd -tl
```

## 氢键分析
**参考资料：[Amber动力学模拟后分析：氢键计算](https://mp.weixin.qq.com/s/FrS18gr6xpSlcJ27Z9PEjA)**