👏 Python库|sys|查看python版本号

---
[TOC]

---
## sys.version_info
简介：用于返回所使用的python版本号。  
(1) 返回值解析  
```python
>>> import sys
>>> sys.version_info
sys.version_info(major=3, minor=8, micro=8, releaselevel='final', serial=0)
```
- major：主版本号，比如python3或python2
- minor: 表示python版本为3.8
- micro：再次一级，比如3.8.0、3.8.1等
- releaselevel："alpha" 、"beta"、"candidate"以及"final"。

(2) 版本比较  
```python
>>> import sys
>>> sys.version_info < (3, 7)
False
>>> sys.version_info > (3, 7)
True
>>> sys.version_info[:3]
(3, 8, 8)
>>> sys.version_info[:4]
(3, 8, 8, 'final')
>>> sys.version_info[:5]
(3, 8, 8, 'final', 0)
```
其返回值打印出来是一个类似字典的形式，但是实际可以将其当作一个元组来操作。  