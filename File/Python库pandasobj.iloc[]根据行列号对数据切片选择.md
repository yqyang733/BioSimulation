👏 Python库|pandas|obj.iloc[]根据行列号对数据切片选择

---
[封面PPT](../PPT/Python库pandasobj.iloc[]根据行列号对数据切片选择.pptx)

---

[TOC]

---

## 简介

obj.iloc[]函数可以根据pandas数据框的行号和列号或者数组的位置号码对数据进行选择。注意该函数的操作对象是数据框的行号和列号或者位置号码，是整数。这点需要和obj.loc[]函数进行区分。

---

## 参数

输入必须是基于整数的行列号，也可以是布尔类型的数组。如下所示：

- 一个整数，例如5
- 一个整数列表或者数字，例如[4, 3, 0]
- 一个整数切片，例如1:7
- 一个布尔类型的数组，例如[True, False, True]
- 一个函数，例如obj.iloc[lambda x: x.index == '小王']

## 示例

```python
>>> mydict = [{'a': 1, 'b': 2, 'c': 3, 'd': 4},
          {'a': 100, 'b': 200, 'c': 300, 'd': 400},
          {'a': 1000, 'b': 2000, 'c': 3000, 'd': 4000 }]
>>> obj = pd.DataFrame(mydict)
>>> obj
      a     b     c     d
0     1     2     3     4
1   100   200   300   400
2  1000  2000  3000  4000
```

例1：输入整数

```python
>>> obj.iloc[1]
a    100
b    200
c    300
d    400
Name: 1, dtype: int64
>>> obj.iloc[2]
a    1000
b    2000
c    3000
d    4000
Name: 2, dtype: int64
>>> obj.iloc[1,2]
300
```

例2：输入一个整数列表索引行列

```python
>>> obj.iloc[[0]]
   a  b  c  d
0  1  2  3  4
>>> obj.iloc[[0, 1]]
     a    b    c    d
0    1    2    3    4
1  100  200  300  400
>>> obj.iloc[[0, 2], [1, 3]]
      b     d
0     2     4
2  2000  4000
```

例3：输入整数切片

```python
>>> obj.iloc[:3]
      a     b     c     d
0     1     2     3     4
1   100   200   300   400
2  1000  2000  3000  4000
>>> obj.iloc[1:3, 0:3]
      a     b     c
1   100   200   300
2  1000  2000  3000
```

例4：输入布尔类型的数组

```pytho
>>> obj.iloc[[True, False, True]]
      a     b     c     d
0     1     2     3     4
2  1000  2000  3000  4000
>>> obj.iloc[:, [True, False, True, False]]
      a     c
0     1     3
1   100   300
2  1000  3000
```

例5：输入函数

```python
>>> obj.iloc[lambda x: x.index % 2 == 0]
      a     b     c     d
0     1     2     3     4
2  1000  2000  3000  4000
>>> obj.iloc[:, lambda obj: [0, 2]]
      a     c
0     1     3
1   100   300
2  1000  3000
```



