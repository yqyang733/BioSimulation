👏 Python库|pandas|obj.fillna()填充缺失数据

---
[封面PPT](../PPT/Python库pandasobj.fillna()填充缺失数据.pptx)

---
[TOC]

---

## 简介

专门用来处理 NA 或 NaN 值。

---

## 语法参数

语法：DataFrame.fillna(value=None, method=None, axis=None, inplace=False, limit=None, downcast=None)

参数：

- value: 可以是常数、字典、数组、数据框。该选项决定了将NA或者NaN填充的内容。
- method: 可以是{‘backfill’, ‘bfill’, ‘pad’, ‘ffill’, None}，默认是None。pad/ffill：用前一个非缺失值去填充该缺失值；backfill/bfill：用下一个非缺失值填充该缺失值；None：指定一个值去填充缺失值。
- axis: 可以是{0 or ‘index’, 1 or ‘columns’}，决定填充的方向轴，按行填充还是按列填充。
- inplace: True or False，决定是原地替换还是建立备份。
- limit: 限制填充个数。
- downcast: 

## 示例

```python
>>> import pandas as pd
>>> import numpy as np
>>> df = pd.DataFrame([[np.nan, 2, np.nan, 0],
...                    [3, 4, np.nan, 1],
...                    [np.nan, np.nan, np.nan, 5],
...                    [np.nan, 3, np.nan, 4]],
...                   columns=list("ABCD"))
>>> df
     A    B   C  D
0  NaN  2.0 NaN  0
1  3.0  4.0 NaN  1
2  NaN  NaN NaN  5
3  NaN  3.0 NaN  4
>>> df_1 = pd.DataFrame(np.random.randint(0,10,(5,5)))
>>> df_1.iloc[1:4,3] = np.NaN
>>> df_1.iloc[2:4,4] = np.NaN
>>> df_1
   0  1  2    3    4
0  8  6  7  6.0  9.0
1  5  2  4  NaN  8.0
2  2  0  2  NaN  NaN
3  3  9  0  NaN  NaN
4  2  6  1  0.0  1.0
```

例1：用0填充df中的全部NaN

```python
>>> df.fillna(0)
     A    B    C  D
0  0.0  2.0  0.0  0
1  3.0  4.0  0.0  1
2  0.0  0.0  0.0  5
3  0.0  3.0  0.0  4
```

例2：使用字典根据列名对df中的NaN进行填充

```python
>>> df.fillna({"A": 0, "B": 1, "C": 2, "D": 3})
     A    B    C  D
0  0.0  2.0  2.0  0
1  3.0  4.0  2.0  1
2  0.0  1.0  2.0  5
3  0.0  3.0  2.0  4
```

例3：使用数据框进行填充

```python
>>> df_2 = pd.DataFrame(np.zeros((4, 4)), columns=list("ABCE"))
>>> df_2
     A    B    C    E
0  0.0  0.0  0.0  0.0
1  0.0  0.0  0.0  0.0
2  0.0  0.0  0.0  0.0
3  0.0  0.0  0.0  0.0
>>> df.fillna(df_2)
     A    B    C  D
0  0.0  2.0  0.0  0
1  3.0  4.0  0.0  1
2  0.0  0.0  0.0  5
3  0.0  3.0  0.0  4
```

例4：method = 'ffill'/'pad'：用前一个非缺失值去填充该缺失值

```python
>>> df_1.fillna(method='ffill')
   0  1  2    3    4
0  8  6  7  6.0  9.0
1  5  2  4  6.0  8.0
2  2  0  2  6.0  8.0
3  3  9  0  6.0  8.0
4  2  6  1  0.0  1.0
```

例5：method = 'bflii'/'backfill'：用下一个非缺失值填充该缺失值

```python
>>> df_1.fillna(method='bfill')
   0  1  2    3    4
0  8  6  7  6.0  9.0
1  5  2  4  0.0  8.0
2  2  0  2  0.0  1.0
3  3  9  0  0.0  1.0
4  2  6  1  0.0  1.0
```

例6：指定limit，限制填充个数

```python
>>> df_1.fillna(method='bfill', limit=2)
   0  1  2    3    4
0  8  6  7  6.0  9.0
1  5  2  4  NaN  8.0
2  2  0  2  0.0  1.0
3  3  9  0  0.0  1.0
4  2  6  1  0.0  1.0
```

例7：指定axis，确定填充方向

```python
>>> df_1.fillna(method="ffill", limit=1, axis=1)
     0    1    2    3    4
0  8.0  6.0  7.0  6.0  9.0
1  5.0  2.0  4.0  4.0  8.0
2  2.0  0.0  2.0  2.0  NaN
3  3.0  9.0  0.0  0.0  NaN
4  2.0  6.0  1.0  0.0  1.0
```

