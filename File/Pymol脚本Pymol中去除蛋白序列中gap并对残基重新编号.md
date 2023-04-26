👏 Pymol脚本|Pymol中去除蛋白序列中gap并对残基重新编号

---

[封面PPT](../PPT/Pymol插件Pymol中去除蛋白序列中gap并对残基重新编号.pptx)

---

[TOC]

---

写在前面：该插件是为完成课题中的特定任务写的，所以可能会有不完善的地方，后续会根据出现的问题不断更新。

## 1. 使用场景

在Pymol中我们选择蛋白的特定残基之后，Pymol中显示的残基序列往往不是连续的，中间会出现很多gap。如PPT中图片所示。

本篇文章的Pymol脚本将会去除序列中的gap并对残基重新编号。

## 2. 插件使用

(1) 先将下述内容保存成一个py文件，如 renum_resi.py。

```python
from pymol import cmd

def renumber(objec):
    myspace = {"lst":[]}
    cmd.iterate("name ca and model %s"%(objec),"lst.append((resi,resn))",space = myspace)
    #print(list(set(myspace["lst"])))
    tmp = sorted(set(myspace["lst"]),key = lambda keys:float(keys[0]))
    # print(tmp)

    if float(tmp[0][0]) < 1:
        # print("222")
        cmd.alter("mode %s"%(objec), "resi=str(int(resi)+abs(int(%s))+3)"%(tmp[0][0]))

        tmp_1 = []
        for i in range(len(tmp)):
            tmp_1.append([str(int(tmp[i][0]) + abs(int(tmp[0][0])) + 3),tmp[i][1]])
        tmp = tmp_1

    # print(tmp)

    flg = 1
    for i in tmp:
        cmd.alter("mode %s and resi %s"%(objec, i[0]), "resi=%d"%(flg))
        flg += 1
```

(2) 将需要重新编号的蛋白导入Pymol中，并在Pymol中运行上述脚本：

```text
run e:/renum_resi.py
renumber obj      # obj是Pymol中对象的名字
```

(3) 即可获得PPT中残基重新编号的序列。