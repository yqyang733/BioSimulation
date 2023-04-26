👏 Python基础|读文件|file，file.readline()和file.readlines()

---
[TOC]

---

写在前面：这里以以下test文件进行举例：   
test文件内容如下：   
> sssdddecrfvgbtvfcedcd   
> sssssssssssss,2   
> dddddddd,1   
> wswedewdxw,5   
> ervrtvbtbgvdgtvt,9   

---
## file,file.readline()和file.readlines()区别
```python
file = open(./test)
print(type(file))    # <class '_io.TextIOWrapper'>
oneline = file.readline()
print(type(oneline))    # <class 'str'>
multilines = file.readlines()
print(type(multilines))   # <class 'list'>
```
如上述代码：

(1) file是一个TextIOWrapper类型的变量，类似iterators，在每次迭代中生成一行文本。是只可以循环一次的东西。   

(2) file.readline()会读取文件的第一行生成字符串类型的变量。   

(3) file.readlines()会读取整个文件生成列表，其中每行是列表中的一个元素。   

---
## file的一次性
(1)
```python
file = open(./test)
oneline = file.readline()
multilines = file.readlines()
print(multilines)    # ['sssssssssssss,2\n', 'dddddddd,1\n', 'wswedewdxw,5\n', 'ervrtvbtbgvdgtvt,9\n']
```
如上述代码：

为什么该代码中multilines不包括第一行的内容呢？这就是因为TextIOWrapper对象file的一次性，file迭代器只能被读一次。也就是说oneline = file.readline()这里已经读了第一行的内容，所以file中第一行的内容已经消失了。所以之后用multilines = file.readlines()只能读出除过第一行之外所有其他行的内容。

(2)
```python
file = open(./test)
oneline = file.readline()
multilines = file.readlines()
print(multilines)    # ['sssssssssssss,2\n', 'dddddddd,1\n', 'wswedewdxw,5\n', 'ervrtvbtbgvdgtvt,9\n']
multilines_1 = file.readlines()
print(multilines_1)    # []
```
如上述代码：

file只能被读一次，当oneline = file.readline()和multilines = file.readlines()将file的内容全部读完的时候，使用multilines_1 = file.readlines()进行读取的时候就是[]。

---
## file.readline()的一行性
(1)

file = open(./test)
oneline = file.readline()
print(oneline)   # sssdddecrfvgbtvfcedcd
如上述代码：

file.readline()只读取文件的一行。

(2) 可用循环和file.readline()结合读取整个文件，这样无需像file.readlines()一样将整个文件读取到内存中，节省内存。
```python
file = open(./test)
while True:
    oneline = file.readline().strip()
    if oneline:
        print(oneline)
    else:
        break   
# output
# sssdddecrfvgbtvfcedcd
# sssssssssssss,2
# dddddddd,1
# wswedewdxw,5
# ervrtvbtbgvdgtvt,9
```

---
## file.readlines()构建列表和多次使用性与for i in file按行迭代的一次性
(1) 使用file.readlines()构建列表保存文件，可多次使用。
```python
file = open(./test)
multilines_1 = file.readlines()
for line in multilines_1:
    print(line.strip())
for line in multilines_1:
    print(line.strip())
# sssdddecrfvgbtvfcedcd
# sssssssssssss,2
# dddddddd,1
# wswedewdxw,5
# ervrtvbtbgvdgtvt,9
# sssdddecrfvgbtvfcedcd
# sssssssssssss,2
# dddddddd,1
# wswedewdxw,5
# ervrtvbtbgvdgtvt,9
```
(2) for i in file按行迭代的一次性，省内存，推荐。
```python
file = open(./test)
for line in file:
    print(line.strip())
for line in file:
    print(line.strip())
# sssdddecrfvgbtvfcedcd
# sssssssssssss,2
# dddddddd,1
# wswedewdxw,5
# ervrtvbtbgvdgtvt,9
```

## Some warnings or errors
**1. UnicodeDecodeError: 'gbk' codec can't decode byte 0x82 in position 345: illegal multibyte sequence**
**解决方案：** 将代码改为改为如下：
```python
# 加上encoding="UTF-8"
for line in open(filename, 'r', encoding="UTF-8"):
    self.add_history(lineobj.ReadLineTextBuffer(ensure_unicode(line.rstrip())))
```
