👏 Python基础|关键字

---
[TOC]

---
## yield
yield和return的关系和区别:  
- 带yield的函数是一个生成器，而不是一个函数了，这个生成器有一个函数就是next函数，next就相当于“下一步”生成哪个数，这一次的next开始的地方是接着上一次的next停止的地方执行的，所以调用next的时候，生成器并不会从foo函数的开始执行，只是接着上一步停止的地方开始，然后遇到yield后，return出要生成的数，此步就结束。  
- yield首先是个return，看做return之后再把它看做一个是生成器（generator）的一部分（带yield的函数才是真正的迭代器）。

**参考资料**
(1) https://blog.csdn.net/mieleizhi0522/article/details/82142856  

## global