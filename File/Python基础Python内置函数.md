# 👏 内置函数

---
[TOC]

---
## 1. 数学运算
### 1.1 abs()
简介：求绝对值。   

### 1.2 divmode()
简介：同时得到商和余数   
例1：
```python
divmod(10, 3)
```

### 1.3 round()
简介：四舍五入保留小数   
例1：   
```python
a = 3.6
print(round(a))
b = 3.4
print(round(b))
```

### 1.4 pow()
### 1.5 sum()
### 1.6 min()
### 1.7 max()
简介：求最大值
例1：求列表中长度最长的元素
```python
>>> a = ["123","12","1234","0673344"]
>>> max(a)
'1234'
>>> max(a, key=len)
'0673344'
```

--- 
## 2. 交互操作
### 2.1 print()
简介：打印   
语法：print(self, *args, sep=' ', end='\n', file=None)    
例1：同时打印多个字符串
```python
# print()接受多个字符串，以逗号隔开，输出时每遇到一个逗号就打印一个空格
print("I","am","a student")
```
例2：打印多个字符串同时设定分隔符
```python
print("I","am","a student",sep="%")
```
例3：字符串前面加f格式化字符串引入变量
```python
str = '888888'
print(f"今年要{str}")
# output:今年要888888
```
### 2.2 input()

---
## 3. 数字相关
### 3.1 bool()
### 3.2 int()
### 3.3 complex()
### 3.4 float()

---
## 4. 编码相关
### 4.1 bin()
### 4.2 ord()
### 4.3 bytearray()
### 4.4 bytes()
### 4.5 oct()
### 4.6 hex()
### 4.7 chr()

---
## 5. 容器相关
### 5.1 list()
### 5.2 str()
### 5.3 dict()
### 5.4 tuple()
### 5.5 frozenset()
### 5.6 set()
简介：该函数创建一个无序不重复元素集，可进行关系测试，删除重复数据，还可以计算交集、差集、并集等。
示例：
例1：向集合中添加元素。
```python
>>> aa = set()
>>> aa.add("a")
>>> aa.add("b")
>>> aa.add("c")
>>> aa
{'a', 'c', 'b'}
>>> aa.add("b")
>>> aa
{'a', 'c', 'b'}
```

---
## 6. 序列操作
### 6.1 reversed()
### 6.2 all()
简介：用于判断给定的可迭代参数中的所有元素是否都为 TRUE，如果是返回 True，否则返回 False。    
批注：空元组、空列表返回值为True，这里要特别注意。    
例1：
```python
all(['a', 'b', 'c', 'd'])  # 列表list，元素都不为空或0
# True
all(['a', 'b', '', 'd'])   # 列表list，存在一个为空的元素
# False
all([0, 1，2, 3])          # 列表list，存在一个为0的元素
# False
############################################   
all(('a', 'b', 'c', 'd'))  # 元组tuple，元素都不为空或0
# True
all(('a', 'b', '', 'd'))   # 元组tuple，存在一个为空的元素
# False
all((0, 1, 2, 3))          # 元组tuple，存在一个为0的元素
# False
############################################   
all([])             # 空列表
# True
all(())             # 空元组
# True
```

### 6.3 fiter()
### 6.4 zip()
简介：用于将可迭代的对象作为参数，将对象中对应的元素打包成一个个元组，然后返回由这些元组组成的列表。在读取由zip()生成的对象时，需要通过循环的方式将zip()生成的可迭代对象打印出来。    
批注：zip()函数创建的是一个只能访问一次的迭代器。    
例1：
```python
a = [1, 2, 3]
b = [4, 5, 6]
zipped = zip(a, b)
for i in zipped:
    print(i)
```   
例2：zip()函数创建的是一个只能访问一次的迭代器。 
```python
dic = {"a":1, "b":2, "c":3, "d":4}
num = zip(dic.values(), dic.keys())
print(min(num))
print(max(num))    # 报错，因为只能访问一次。
```

### 6.5 map()
简介：根据提供的函数对指定序列做映射。    
语法：map(function, iterable, ...)    
参数：    
function: 函数    
iterable: 一个或多个序列    
例1：生成一个序列的平方
```python
list(map(lambda x: x ** 2, [1, 2, 3, 4, 5]))   # output: [1, 4, 9, 16, 25]
```
例2：对两个列表对应数据进行相加
```python
list(map(lambda x, y: x + y, [1, 3, 5, 7, 9], [2, 4, 6, 8, 10]))   # output: [3, 7, 11, 15, 19]
```

### 6.6 sorted()
简介：对所有可迭代的对象排序。    
语法：sorted(iterable, key=None, reverse=False)    
批注：   
sort 与 sorted 区别：    
(1) sort 是应用在 list 上的方法，sorted 可以对所有可迭代的对象进行排序操作    
(2) list 的 sort 方法返回的是对已经存在的列表进行操作，而内建函数 sorted 方法返回的是一个新的 list，而不是在原来的基础上进行的操作    
例1：根据绝对值的大小进行排序
```python
a = [-9, 5, 6, -7, 3, -11]
print(sorted(a, key=abs))
```
例2：根据相反数进行排序
```python
list = [5, 0, 6, 1, 2, 7, 3, 4]
test_list = sorted(list, key=lambda x: x*-1)   # 根据相反数进行排序
print(test_list)
```
例3：根据反函数进行排序
```python
list = [5, 6, 1, 2, 7, 3, 4]
test_list = sorted(list, key=lambda x: x**-1)   # 根据反函数进行排序
print(test_list)
```
例4：根据二维列表中的某一属性值进行排序  
```python
>>> list = [(1,"a"),(5,"b"),(3,"n"),(10,"z"),(52,"m")]
>>> tmp = sorted(list, key = lambda keys:float(keys[0]))
>>> print(tmp)
[(1, 'a'), (3, 'n'), (5, 'b'), (10, 'z'), (52, 'm')]
>>> tmp = sorted(list, key = lambda keys:(keys[1]))
>>> print(tmp)
[(1, 'a'), (5, 'b'), (52, 'm'), (3, 'n'), (10, 'z')]
```

### 6.7 next()
简介：返回迭代器的下一个项目。next() 函数要和生成迭代器的 iter() 函数一起使用。    
语法：next(iterable[, default])   
参数：    
default -- 可选，用于设置在没有下一个元素时返回该默认值，如果不设置，又没有下一个元素则会触发 StopIteration 异常。   
例1：
```python
a = [1,2,3,4,5,5,6]
b = iter(a)
next(b)  # 1
next(b)  # 2
next(b)  # 3
next(b)  # 4
next(b)  # 5
next(b)  # 5
next(b)  # 6
next(b)  # StopIteration
```
例2：
```python
a = [1,2,3,4,5,5,6]
b = iter(a)
next(b,"None")  # 1
next(b,"None")  # 2
next(b,"None")  # 3
next(b,"None")  # 4
next(b,"None")  # 5
next(b,"None")  # 5
next(b,"None")  # 6
next(b,"None")  # None
```

### 6.8 any()

---
## 7. 对象操作
### 7.1 help()
### 7.2 dir()
### 7.3 id() 
简介：用于获取对象的内存地址。
例1：相同的整数数字和字符数字的内存地址是不同的。
```python
>>> a = ["1","2","3"]
>>> id(a[0])
2747293046064
>>> id(a[0])
2747293046064
>>> id(a[1])
2747298064112
>>> a[0] = 1
>>> a
[1, '2', '3']
>>> id(a[0])
140724700522272
>>> id(a[1])
2747298064112
```

### 7.4 hash()
### 7.5 type()
简介：查看变量的类型   
例1：
```python
a = "abc"
print(type(a))
```

### 7.6 len()
### 7.7 ascii()
### 7.8 format()
### 7.9 vars()
简介：返回对象object的属性和属性值的字典对象。    
语法：vars([object])    

---
## 8. 文件操作
### 8.1 open()

---
## 9. 变量操作
### 9.1 locals()
Description: The locals() function returns all local variables at the current locations as a dictionary. 
Example 1:   
```python
>>> def runoob(arg):
...     z = 1
...     print(locals())

>>> runoob(4)
{'arg': 4, 'z': 1}
```

### 9.2 globals()

---
## 10. 反射操作
### 10.1 \_\_import\_\_()
### 10.2 delattr()
### 10.3 callable()
### 10.4 setattr()
### 10.5 getattr()
### 10.6 hasattr()
### 10.7 isinstance()
语法：判断一个对象是否是一个已知的类型，类似 type()，返回一个布尔型变量。    
批注：sinstance() 与 type() 区别：     
1) type() 不会认为子类是一种父类类型，不考虑继承关系。     
2) isinstance() 会认为子类是一种父类类型，考虑继承关系。     
3) 如果要判断两个类型是否相同推荐使用 isinstance()。    
语法：isinstance(object, classinfo)    
参数：    
classinfo: 可以是直接或间接类名、基本类型或者由它们组成的元组     
例1：
```python
str1 = "aaaa"
isinstance(str1,str)   # output: True
isinstance(str1,int)   # output: False
isinstance(str1,(int,str,float))   # output: True
```
例2：判断数组内所有对象类型并返回一个布尔型数组。   
```python
a = [1, 2, "aaa", 3, 3.1]
[isinstance(i,str) for i in a]      # output: [False, False, True, False, False]
```

### 10.8 issubclass()

---
## 11. 装饰器相关
### 11.1 property()
### 11.2 staticmethod()
### 11.3 classmethod()

---
## 12. 编译执行
### 12.1 eval()
### 12.2 exec()
### 12.3 repr()
### 12.4 compile()

---
## 13. 其他
### 13.1 iter()
简介：用来生成迭代器。    
语法：iter(object[, sentinel])    

### 13.2 enumerate()
简介：用于将一个可遍历的数据对象(如列表、元组或字符串)组合为一个索引序列，同时列出数据下标和数据。    
语法：enumerate(sequence, [start=0])   
例1：
```python
a = ["aaa","bbb","ccc","ddd"]
for idx, ele in enumerate(a):
    print(idx, ele)
# output:
# 0 aaa
# 1 bbb
# 2 ccc
# 3 ddd    
```
例2：
```python
a = ["aaa","bbb","ccc","ddd"]
for idx, ele in enumerate(a, 1):
    print(idx, ele)
# output:
# 1 aaa
# 2 bbb
# 3 ccc
# 4 ddd    
```

### 13.3 range()
### 13.4 slice()
简介：创建一个切片对象，可以用在任何切片允许使用的地方。   

### 13.5 super()
### 13.6 object()
### 13.7 memoryview()