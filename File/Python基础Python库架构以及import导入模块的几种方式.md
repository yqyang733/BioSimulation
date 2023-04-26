👏 Python基础|Python库架构以及import导入模块的几种方式

---

[封面PPT](../PPT/Python基础Python库架构以及import导入模块的几种方式.pptx)

---

[TOC]

---

（1）import语句的作用：用来导入模块，可以出现在程序中的任何位置。

（2）

import module
关键字 模块名

import math 

最好把导入模块放在代码的开头。

**为什么要把import导入模块放在程序开头使用呢？**
[解释器](https://www.iplaypy.com/jichu/interpreter.html)在执行语句时，遵循作用域原则。因为这和作用域有关系，如果在顶层导入模块，此时它的作用域是全局的；如果在函数内部导入了模块，那它的作用域只是局部的，不能被其它函数使用。如果其它函数也要用到这个模块，还需要再次导入比较麻烦。

在用import语句导入模块时最好按照这样的顺序：

1、python 标准库模块
2、python 第三方模块
3、自定义模块

## from-import



## **模块、常规包与命名空间包**



「模块」、「常规包」与「命名空间包」是理解import机制绕不开的概念。

「模块」：一个以「.py」为后缀的文件就是一个模块「常规包」：「__init__.py」所在目录就是一个常规包「命名空间包」：命名空间包是一种虚拟的概念，它由多个子包构成，这些子包可以在任意位置，可以为zip中的文件或网络上的文件，这些子包在概念上是一个整体，这个整体就是一个命名空间包



「常规包」与「命名空间包」的概念在PEP420被提出，在Python3.3及之后的Python版本中实现，此前只有「常规包」这一种包。





import语句做的是两个操作



1.搜索操作：在相应的路径中搜索指定名称的模块2.绑定操作：将搜索到的结果绑定到当前作用域对应的名称上(即创建module对象，并初始化)





import在导入模块时，会根据sys.path中定义的路径来搜索对应的模块，sys.path是一个list，import时会先从sys.path中下标为0的路径开始搜索，我们可以将需要的路径添加到sys.path。

如果我们没有修改，sys.path中默认的路径为：

1.当前目录的路径

2.环境变量PYTHONPATH中指定的路径列表

3.Python安装路径的lib目录所在路径

写的很好：https://blog.csdn.net/weixin_42232219/article/details/102559601

- 1.单独使用import时，import后面只能接模块或包(常规包或命名空间包)
- 2.使用from xxx import xxx 时，from后只能接模块或包，而此时import后可以接任何变量(模块、包或模块中具体的方法)





from module_name import method_name1,method_name2





