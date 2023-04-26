👏 Tcl代码书写教程

---
[TOC]

---
## 终端使用tcl
输入tclsh即可唤起tcl。输入exit即可退出。

## 特殊变量
|Variable Name|Remark|
|:----|:----|
|argc|引用许多命令行参数。|
|argv|引用包含命令行参数的列表。|
|argv0|指正在解释的文件的文件名或调用脚本的名称。|
|||

## 基本语法
（1）注释
```tcl
#!/usr/bin/tclsh

# 我在 Tcl 中的第一个程序
puts "Hello World!" 
```
```tcl
#!/usr/bin/tclsh

if 0 {
   my first program in Tcl program
   Its very simple
}
puts "Hello World!" 
```
```tcl
#!/usr/bin/tclsh

puts "Hello World!" ;# 我在 Tcl 程序中的第一次打印
```

## 内置命令
（1）puts - 打印
```tcl
% puts "Hello World!!"
Hello World!!
```

## 参考资料
1. [Tcl入门教程-无涯教程](https://www.learnfk.com/tcl-tk/tcl-tk.html)