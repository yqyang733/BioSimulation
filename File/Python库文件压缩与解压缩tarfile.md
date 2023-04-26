👏 Python库|文件压缩与解压缩tarfile

---
[TOC]

---
## 解压缩
**.tgz文件解压缩**
```python
tarobj = tarfile.open("charmm-gui.tgz", "r:gz")
for tarinfo in tarobj:
    a_name = tarinfo
    break
for tarinfo in tarobj:
    tarobj.extract(tarinfo.name, "./")
f_name = str(a_name).split("'")[1].split("/")[0]
```