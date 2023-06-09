# 👏 Python基础|Python读写文件

---
[封面PPT](../PPT/Python基础Python读写文件.pptx)

---
## Contents
- [with同时打开多个文件](#with同时打开多个文件)
- [读取txt等本文格式文件](#读取txt等本文格式文件)
- [取csv文件](#取csv文件)
- [temp.readlines()的一次性](#temp.readlines()的一次性)
- [写出文件](#写出文件)

---
<a name="with同时打开多个文件" />

## with同时打开多个文件
```python
with open(path+"test1.txt","w") as f1, open(path+"test2.txt","w") as f2:
    f1.write("000000")
    f2.write("000000")
```

---
<a name="读取txt等本文格式文件" />

## 读取txt等本文格式文件
```python
# 例如读取txt、sdf、mol2等本文格式文件
path = "F:/temp/"
filename = "temp.txt"
with open(path+filename) as f1:
    flines = f1.readlines()
    print(type(flines))
    print(flines)     # 这里可以看出读取txt文件和读取csv文件是不一样的。读取txt文件readlines会直接生成一个列表，每一行就是一个元素。
    
with open("F:/temp/" + filename) as f1:
    flines = f1.readlines()
```

---
<a name="取csv文件" />

## 读取csv文件
```python
# 例如读取csv文件
import csv
with open("F:/temp/temp.csv") as temp:
    temp = csv.reader(temp)
    print(type(temp))
    print(temp)        # 可以看出csv读取数据之后并没有保存成一个数组的形式，而是需要一行一行读取，存成列表。
    for line in temp:
        print(line)   # 可以看出读取csv文件，是把每一行都存成一个列表，然后每行中的单元格都作为一个列表中的元素。
```

---
<a name="temp.readlines()的一次性" />

## temp.readlines()的一次性

如PPT中所示，将文件读成一个临时temp，在循环中使用temp.readlines()时它仅能用一次。第二次即会变空。    
所以，最好的方式就是使用fl = temp.readlines()，先将临时temp存成一个列表，之后可以多次使用这个列表。    

---
<a name="写出文件" />

## 写出文件
### write()
```python
with open("test.txt", "w") as f:
    f.write("test")
# 生成test.txt文件，文件内部内容是test
```

### writelines()
```python
##############################################################
# 示例1：不加换行符，一股脑的将列表元素全部迭代写入文件
a=["little","few","much","many"]
with open("test.txt", "w") as f:
    f.writelines(a)
# 生成test.txt文件，文件内部内容是littlefewmuchmany
###############################################################
# 示例2：加换行符，将列表中元素写入文件，一个元素一行
a=["little","few","much","many"]
with open("test.txt", "w") as f:
    f.writelines([line+'\n' for line in a])
# 生成test.txt文件，文件的内容如下：
# little
# few
# much
# many
###############################################################
```

---