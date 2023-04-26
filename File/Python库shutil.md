👏 Python库|shutil

---
[TOC]

---
## 常用函数
**（1）shutil.copy()-复制文件**
```python
import shutil
shutil.copy("list_contacts_shortest.py", "test")
```

**（2）shutil.rmtree()-删除文件目录**
```python
>>> import shutil
>>> shutil.rmtree("temp")
```

**（3）shutil.move()-移动文件**
```python
shutil.move("hybrid.pdb", os.path.join(".", "common", "hybrid.pdb"))
shutil.move("hybrid.rtf", os.path.join(".", "common", "hybrid.rtf"))
```