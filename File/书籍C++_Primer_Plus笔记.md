👏 书籍|C++ Primer Plus笔记

---
[TOC]

---
## 第三章
**3_8.cpp**
```cpp
#include <iostream>

int main(void)
{
    using namespace std;

    cout.setf(ios_base::fixed, ios_base::floatfield);

    float tub = 10.0/3.0;
    const float million = 1.0E6;

    cout << "tub = " << tub << endl;
    cout << "A million tubs = " << million*tub << endl;
    cout << "Ten million tubs = " << 10*million*tub << endl;

    double mint = 10.0/3.0;
    cout << "mint = " << mint << endl;
    cout << "A million mints = " << million*mint << endl;
    cout << "Ten million mints = " << 10*million*mint << endl;

    return 0;
}
```

**3_9.cpp**
```cpp
#include <iostream>

int main(void)
{
    using namespace std;

    float a = 2.34E22;
    float b = a + 1.0;

    cout << "a = " << a << endl;
    cout << "b - a = " << b - a << endl;

    return 0;
}
```

**3_10.cpp**
```cpp
#include <iostream>

int main(void)
{
    using namespace std;

    float hats, heads;

    cout.setf(ios_base::fixed, ios_base::floatfield);

    cout << "Enter a number: ";
    cin >> hats;
    cout << "Enter another number: ";
    cin >> heads;

    cout << "hats = " << hats << ". heads = " << heads << endl;
    cout << "hats + heads = " << hats + heads << endl;
    cout << "hats - heads = " << hats - heads << endl;
    cout << "hats * heads = " << hats * heads << endl;
    cout << "hats / heads = " << hats / heads << endl;

    return 0;
}
```

**3_11.cpp**
```cpp
#include <iostream>

int main(void)
{
    using namespace std;

    cout.setf(ios_base::fixed, ios_base::floatfield);

    cout << "Integer division: 9/5 = " << 9/5 << endl;
    cout << "Float division: 9.0/5.0 = " << 9.0/5.0 << endl;
    cout << "Mixed division: 9.0/5 = " << 9.0/5 << endl;
    cout << "Double division: 1e7/9.0 = " << 1e7/9.0 << endl;
    cout << "Float constance: 1e7f/9.0f = " << 1e7f/9.0f << endl;

    return 0;
}
```

**3_12.cpp**
```cpp
#include <iostream>

int main(void)
{
    using namespace std;

    const int Lbs_per_stn = 14;
    int lbs;

    cout << "Enter your weight in pounds: ";
    cin >> lbs;
    int stone = lbs / Lbs_per_stn;
    int pounds = lbs % Lbs_per_stn;

    cout << lbs << " pounds = " << stone << " stones, " << pounds << " pound(s).\n";

    return 0;
}
```



## 参考资料
1. [C++ Primer Plus (第6版)_中文版](./书籍C++_Primer_Plus笔记/C++_Primer_Plus第6版_中文版.pdf)
2. [C++教程：C++ Primer Plus(第六版)教程-bilibili](https://www.bilibili.com/video/BV1Yv411t7qe?spm_id_from=333.999.0.0)