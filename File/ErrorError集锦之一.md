👏 Error|Error集锦之一

---
[TOC]

---
## 读取文件出现\ufeff情况解决
```python
with open(file,) as f:
        f1 = f.readlines()
    for line in f1:
        line = line.encode('utf-8').decode('utf-8-sig')
```

参考：  
1. [读取文件出现\ufeff情况解决](https://www.jianshu.com/p/f10424bbec98)