👏 Python库|pandas|obj.drop_duplicates()去重函数

---
[封面PPT](../PPT/Python库pandasobj.drop_duplicates()去重函数.pptx)

---
[TOC]

---

## 简介

该函数就是去重，各种花样的去重，可以对整行去重，也可以对所有行的一些列元素去重。

## 语法参数

语法：DataFrame.drop_duplicates(subset=None, keep='first', inplace=False, ignore_index=False)

参数：
subset: 列标签或者列标签列表，这里可选对哪些列进行去重，默认对整行进行去重。

keep: 该参数决定保留哪一个重复值，可选{"first", "last", False}。first: 保留第一次出现的值。last: 保留最后一次出现的值。False: 重复值全部删去，一个都不保留。

inplace: 是否原地操作，还是建立一份拷贝。

ignore_index: 是否将index进行重新排序。

## 示例

```python
>>> import pandas as pd
>>> df = pd.DataFrame({
...     'brand': ['Yum Yum', 'Yum Yum', 'Indomie', 'Indomie', 'Indomie'],
...     'style': ['cup', 'cup', 'cup', 'pack', 'pack'],
...     'rating': [4, 4, 3.5, 15, 5]
... })
>>> df
     brand style  rating
0  Yum Yum   cup     4.0
1  Yum Yum   cup     4.0
2  Indomie   cup     3.5
3  Indomie  pack    15.0
4  Indomie  pack     5.0
```

例1：所有参数全部默认，根据整行进行去重。

```python
>>> df.drop_duplicates()
     brand style  rating
0  Yum Yum   cup     4.0
2  Indomie   cup     3.5
3  Indomie  pack    15.0
4  Indomie  pack     5.0
```

例2：根据brand列进行去重。

```python
>>> df.drop_duplicates(subset=['brand'])
     brand style  rating
0  Yum Yum   cup     4.0
2  Indomie   cup     3.5
```

例3：根据brand和style两列进行去重，保留最后一次出现的行。

```python
>>> df.drop_duplicates(subset=['brand', 'style'], keep='last')
     brand style  rating
1  Yum Yum   cup     4.0
2  Indomie   cup     3.5
4  Indomie  pack     5.0
```



