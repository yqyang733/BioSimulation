👏 Python库|os

---
[TOC]

---
## 获取目录或文件路径
**（1）os.getcwd()-返回当前工作目录**
```python
>>> import os
>>> os.getcwd()
'D:\\drug_working\\code\\DEEPScreen\\bin'
```

**（2）os.path.join()-文件路径拼接**
windows上进行路径拼接
```python
>>> import os
>>> os.path.join("D:\eeee","file","drug design")
'D:\\eeee\\file\\drug design'
```
Linux上进行路径拼接
```python
>>> import os
>>> os.path.join("D:","file","drug design")
'D:/file/drug design'
```

**（3）os.chdir()-切换目录**
```python
import os
os.chdir("../")
```

## 对文件或目录进行判断
**（1）os.path.exists()-测试文件/目录是否存在**
```python
import os
os.path.exists("C:/Users/Lenovo")   # output: True
os.path.exists("C:/Users/Lenovo/test")    # output: False
```

**（2）os.path.isfile()-测试文件是否是普通文件**
```python
import os
os.path.isfile("./do.sh")   # output: False, 文件不存在，返回False。
os.path.isfile("./do1.sh")   # output: True
os.path.isfile("./result_0")   # output: 文件是目录，返回False。
```

**（3）os.path.isdir()-测试文件是否是目录**
```python
import os
os.path.isdir("./result_0")   # output: True, 文件是目录，返回True。
```

**（4）os.path.islink()-测试文件是否是链接**
```python
import os
os.path.islink("/usr/local/bin/python3")  
```

## 创建删除文件或目录
**（1）os.mkdir()-创建一级目录**
```python
os.mkdir('./results')
```

**（2）os.makedirs()-递归创建目录**
```python
os.makedirs('./results/result_1/')
```

**（3）删除文件**
```python
import os
os.remove("temp.txt")  # 删除temp.txt
```

## 其他常用函数
**（1）os.listdir(path)**
返回path指定的文件夹包含的文件或文件夹的名字的列表。
```python
import os
os.listdir()     # 输出当前文件夹下的所有文件的名字。
```