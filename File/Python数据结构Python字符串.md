👏 Python数据结构|Python字符串.md

---
[TOC]

---
### 1. 去除空格或特殊符号
#### 1.1 strip()
简介：用于移除字符串头尾指定的字符（默认为空格）或字符序列。只能删除开头或是结尾的字符，不能删除中间部分的字符。    
语法：str.strip([chars])   
例1：    
```python
## 删除首尾的字符串，不删除中间的字符。
str = "*****this is **string** example....wow!!!*****"
print (str.strip( '*' ))  # 指定字符串 *
```

例2：
```python
# 1、strip() 处理的时候，如果不带参数，默认是清除两边的空白符，例如：/n, /r, /t, ' ')。
# 2、strip() 带有参数的时候，这个参数可以理解一个要删除的字符的列表，是否会删除的前提是
# 从字符串最开头和最结尾是不是包含要删除的字符，如果有就会继续处理，没有的话是不会删除中间的字符的。
str = "123abcrunoob321"
print (str.strip( '12' ))  # 字符序列为 12
```

例3：
```python
### 删除的是首尾含有的字符的列表，无需一定要按照顺序
str = '123132231213321312==321312213231123132'
print(str.strip('123'))
```

#### 1.2 lstrip()
简介：截除字符串左边的空格或指定字符。   
例1：
```python
a = "    yqyqang"
print(a.lstrip())
```

例2：
```python
a = "*******yqyqang"
print(a.lstrip("*"))
```

#### 1.3 rstrip()
简介：截除字符串结尾的空格或指定字符    
例1：
```
a = "yqyqang    "
print(a.rstrip())
```

例2：
```python
a = "*******yqyqang"
print(a.rstrip("ng"))
```

---
### 2. 字符串搜索和替换
#### 2.1 str.find()
简介：检测字符串中是否包含子字符串str。如果包含子字符串返回开始的索引值，否则返回-1。   
例1：   
```python
str = "wjiklanmdb"
str.find("k")         # output: 3
=======================================
str = "wjiklanmdbkkk"
str.find("k")         # output: 3    仅能输出第一次出现的字符的索引。
==================================================================
str = "wjiklanmdbkkk"
str.find("k",5)       # output: 10     指定查找的范围。5-末尾。
```

#### 2.2 count()
简介：统计字符串里某个字符出现的次数。可选参数为在字符串搜索的开始与结束位置。   
语法：str.count(sub, start= 0,end=len(string))   
参数：   
str为字符串对象   
sub为搜索的子字符串   
start为字符串开始搜索的位置。默认为第一个字符,第一个字符索引值为0。   
end为字符串中结束搜索的位置。字符中第一个字符的索引为 0。默认为字符串的最后一个位置。   
例1：
```python
a = 'national day'
a.count('a',2,20) 
```

#### 2.3 upper()
简介：把所有字符中的小写字母转换成大写字母。   
例1：
```python
str = "wenjsERYjjh"
print(str.upper())       # output: WENJSERYJJH
```

#### 2.4 lower()
简介：把所有字符中的大写字母转换成小写字母。   
例1：
```python
str = "wenjsERYjjh"
print(str.lower())       # output: wenjseryjjh
```

#### 2.5 capitalize()
简介：把第一个字母转化为大写字母，其余小写。   
例1：
```python
str = "wenjsERYjjh"
print(str.capitalize())      # output: Wenjseryjjh
```

#### 2.6 title()
简介：把每个单词的第一个字母转化为大写，其余小写。    
例1：
```python
str = "wen.jsERY.jjh"
print(str.title())          # output: Wen.Jsery.Jjh

str1 = "wen jsERY jjh"
print(str.title())          # output: Wen Jsery Jjh
```

#### 2.7 str.index()
简介：功能与find一样，在指定字符串中查找字符，不同处是找不到会报exception。
语法：
```python
str.index(str, beg = 0 end = len(string))
# 参数意义：
# str：需要查找的字符
# beg：查找起始位置，默认0
# end：查找结束位置，默认字符串长度
```

示例：
```python
>>> str1 = "this is string example....wow!!!"
>>> str2 = "exam"
>>> print(str1.index(str2))
15
>>> print(str1.index(str2, 10))
15
>>> print(str1.index(str2, 40))
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
ValueError: substring not found
```

---
### 3. 字符串测试
#### 3.1 isdigit()
简介：检测字符串是否只由数字组成。   
例1：   
```python
a = "123456"
a.isdigit()

a = "123456abz"
a.isdigit()
```

#### 3.2

---
### 4. 字符串分割
#### 4.1 split()
语法：str.split(str="", num=string.count(str))   
参数：   
str -- 分隔符，默认为所有的空字符，包括空格、换行(\n)、制表符(\t)等。   
num -- 分割次数。默认为 -1, 即分隔所有。    
例1：
```python
str = "Line1-abcdef \nLine2-abc \nLine4-abcd"
print (str.split( ))       # 以空格为分隔符，包含 \n
print (str.split(' ', 1 )) # 以空格为分隔符，分隔成两个
```

例2：
```python
txt = "Google#Runoob#Taobao#Facebook"
 
# 第二个参数为 1，返回两个参数列表
x = txt.split("#", 1)
 
print (x)
```

#### 4.2 rsplit()
简介：与split()的区别在于rsplit()是从右侧开始根据分隔符进行分割。    
语法：str.rsplit(str="", num=string.count(str))    
例1：   
```python
str = "Line1-abcdef \nLine2-abc \nLine4-abcd"
print(str.rsplit(' ', 1)) # 以空格为分隔符，分隔成两个
```

---
### 5. 字符串连接
#### 5.1 join()
简介：用于将序列中的元素以指定的字符连接生成一个新的字符串。    
语法：str.join(sequence)   
例1：   
```python
list=[ 'He is','a',' boy!']
str1='/'.join(list)
print(str1)    # output: He is/a/ boy!
```

例2：
```python
list=['He is']
str1='/'.join(list)
print(str1)    # He is
```

---
### 6. 字符串切片

---
<a name="转义" />

### 7. 转义
例1：
```python
temp = "abc123\n"
temp1 = r"abc123\n"
# \n 换行
print(temp)
# 打印原生字符串,字符串前面加r或R
print(temp1)
```

例2： 常用转义字符。   
```python
print("aaaaa \
... bbbbb\
... ccccc")      # output: aaaaa bbbbbccccc   # 末尾的\代表续行符，即下一行紧跟在上一行后面。
======================================================================================
print("\\")      # output: \    # 对\进行了转义
===============================================
print('\'')      # output: '     # 对'进行了转义
===============================================
print("\"")      # output: "     # 对"进行了转义
===============================================
print("Hello \b World!")      # output: Hello World!    # \b代表空格
===================================================================
print("\000")      # output:        # 输出为空
==============================================
print("\n")        # output:        # 换行
==============================================
print("Hello \v World!")       # output: Hello
                                              #   World!   # \v纵向制表符
=========================================================================
print("Hello \t World!")       # Hello    World!     # \t横向制表符
==================================================================   
```

---
### 8. 字符串格式化
#### 8.1 zfill()
简介：返回指定长度的字符串，原字符串右对齐，前面填充0。    
语法：str.zfill(width)    
例1：给1-15的数字前面填充0都变成三位数字。   
```python
for i in range(1,16):
    print(str(i).zfill(4))
```

---

### 其他
#### 判断字符串大小写
**str.isupper()**  
简介：检测一个字符串之中的所有字母字符是否为大写，返回的也是一个布尔类型值True和False。如果全为大写则返回True，只要有一个字母为小写那么就会返回False。  
```python

```