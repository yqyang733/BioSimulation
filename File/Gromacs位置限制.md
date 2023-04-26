👏 Gromacs|位置限制

---
[TOC]

---
## 文件设置
（1）constraints.itp文件里面设置对哪个原子限制，限制力大小。
```text
[ position_restraints ]
; atom  type      fx      fy      fz
     5     1  2000  2000  2000
```

（2）topol.top文件
需要在对应的体系最后面加上下面内容。
```text
; Include Position restraint file
#ifdef POSRES
#include "constraints.itp"
#endif
```
或者将constraints.itp内容直接粘贴替换“#include "constraints.itp"”行
（3）prod.mdp文件
在mdp文件中加入下面内容，**其中DPOSRES中的D非常重要**。
```text
; position constraints
;----------------------------------------------------
define                  = -DPOSRES ; position restrain
```
（4）run