👏 Python库|collections

---
[TOC]

---
## Counter
（1）统计列表中元素出现频数。
```python
>>> from collections import Counter
>>> Counter(tmp)
Counter({8: 8, 5: 5, 4: 4, 6: 3, 7: 3, 3: 2, 1: 1, 2: 1})
>>> a = Counter(tmp)
>>> a
Counter({8: 8, 5: 5, 4: 4, 6: 3, 7: 3, 3: 2, 1: 1, 2: 1})
>>> for i in a:
...     print(a[i])
...
1
1
2
4
5
3
3
8
```

## defaultdict
