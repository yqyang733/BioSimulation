👏 使用molvs包进行Smiles标准化处理

---
[TOC]

---

## 简介

相同的化合物可能有各种各样的Smiles，将同一个化合物生成唯一确定Smiles的过程称为Smiles标准化或者正则化。有诸多工具可以实现这样的功能，本文介绍使用molvs包进行Smiles标准化处理。

---

## 输入数据格式

输入数据格式，文件包括两列，第一列是小分子ID编号，第二列是小分子Smiles，两列以逗号隔开。

---

## 脚本

脚本内容如下所示，将下列文件保存成py文件。

```python
import sys
from molvs import standardize_smiles

def standard(file):
    with open(file) as f:
        f1 = f.readlines()
    output = ""
    for i in f1:
        name = i.split(",")[0]
        smiles = i.split(",")[1].strip()
        smiles_stand = standardize_smiles(smiles)
        output = output + name + "," + smiles_stand + "\n"
    with open("drug_smiles_standard.txt","w") as rt:
        rt.write(output)

def main():
    file = str(sys.argv[1])
    standard(file)

if __name__ == '__main__':
    main()
```

运行如下命令：

```shell
python py txt
```

即可得到标准化后的 drug_smiles_standard.txt 文件。