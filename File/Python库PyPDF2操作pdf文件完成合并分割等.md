👏 Python库|PyPDF2操作pdf文件完成合并分割等

---
[TOC]

---
## PyPDF2库简介


## 


## 顺序合并多个pdf文件
```python
import PyPDF2

lst = ["aaa", "bbb", "ccc", "ddd", "eee", "fff"]

merge = PyPDF2.PdfFileMerger()
for pdfFile in lst:
    merge.append(pdfFile + ".pdf")

merge.write('merged.pdf')
merge.close()
```

参考资料：  
1. https://pypdf2.readthedocs.io/en/latest/index.html  

这个库好好完整的写一下，锻炼一下自己的写作。  
促进知识的不断积累。  