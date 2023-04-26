👏 Python库|pandas

---
[TOC]

---
## 选取
**1. 按列标题选出某几列数据**
```python
df = pd.DataFrame(df,columns=['body','predict'])
df = df[['predict', 'body']]
```