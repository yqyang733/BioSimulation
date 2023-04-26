👏 C++|main()函数及其参数
---
[TOC]
---
## 格式
```cpp
int main(int argc, char *argv[])
```
## 参数解析
（1）第一个参数argc，指明有多少个参数将被传递给主函数main()。
（2）真正的参数以字符串数组（即第2个参数argv[]）的形式来传递。
（3）argc代表参数的数量, main()函数本身是在索引为0的第一个参数。 所以, argc总是至少为1 。
## 实例
（1）如下：
```cpp
#include <cstdio>
using namespace std;
int main(int argc, char *argv[])
{
    int i;
    for (i = 0; i < argc; i++)
        printf("Argument %d is %s.\n", i, argv[i]);
    return 0;
}
// Argument 0 is ./test.o.
// Argument 1 is a.
// Argument 2 is b.
// Argument 3 is c.
// Argument 4 is d.
// Argument 5 is e.
// Argument 6 is f.
```
## 参考资料


