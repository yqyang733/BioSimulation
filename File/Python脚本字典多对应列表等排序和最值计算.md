👏 Python脚本|字典/多对应列表等排序和最值计算

---
[TOC]

---
## 求取两个对应列表的最值
(1) 有两个长度相同的列表，列表对应位置元素成对匹配，以其中一个列表为计算标准，找对最值对应的另一列表的值。  
```python
>>> a = [1, 2 ,3, 4, 7, 6]
>>> b = ["a", "c", "b", "d", "e", "z"]
>>> min(zip(a, b))
(1, 'a')
>>> max(zip(a, b))
(7, 'e')
```

## 根据字典的键值进行排序输出
(1) 有一个字典，键是字符，值是数值，根据键值进行排序并输出键和值。  
```python
>>> TCM_num_dict = {"a":1, "b":2, "c":9, "d":3}
>>> sorted(zip(TCM_num_dict.values(), TCM_num_dict.keys()), reverse=True)
[(9, 'c'), (3, 'd'), (2, 'b'), (1, 'a')]
```

## 二维列表根据某一维数据进行排序
```python
fin_lst.sort(key=lambda ele: ele[0])
fin_lst.sort(key=lambda ele: ele[1])
```

## 参考资料
(1) https://blog.csdn.net/windy135/article/details/84576609  