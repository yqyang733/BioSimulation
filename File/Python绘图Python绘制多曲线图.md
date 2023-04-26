# 👏 Python绘图|Python绘制多曲线图

---
[封面PPT](../PPT/Python绘图Python绘制多曲线图.pptx)

---
## Contents
- [输入数据格式](#输入数据格式)
- [绘图脚本](#绘图脚本)

---
<a name="输入数据格式" />

## 输入数据格式
输入数据格式见PPT

---
<a name="绘图脚本" />

## 绘图脚本
(1) 创建一个py脚本，内容如下所示：
```python
import sys, math
from matplotlib import cm,colors
from matplotlib import pyplot as plt
from matplotlib.pyplot import figure, show, rc
import numpy as np
import pandas as pd

def col_pic(file):
    df = pd.read_csv(file)
    fig = plt.figure(figsize=(10,8))
    plt.subplots_adjust(left=0.1, right=0.9, top=0.9, bottom=0.2)
    ax = plt.gca()
    b1, = plt.plot(df['epoch'],df['mse'],'r',linewidth=2, label="MSE")
    b2, = plt.plot(df['epoch'],df['per'],'g',linewidth=2, label="Pearson Correlation")
    b3, = plt.plot(df['epoch'],df['con'],'b',linewidth=2, label="Concordance Index")

    plt.xlabel('Epoch', fontproperties="Times New Roman",fontsize=28,weight="bold")
    # plt.ylabel('Frequency',fontproperties="Times New Roman",fontsize=28,weight="bold")   # 设置y轴标签
    plt.xticks(font="Times New Roman",rotation=60,size=20)      # size must be after the font.
    # plt.xticks(df['epoch'],font="Times New Roman",rotation=60,size=20)  # show all xticks.
    plt.yticks(font="Times New Roman",size=20)
    # plt.title('Frequency_vdw', fontproperties='Times New Roman', fontsize=33)   # 设置图片标题
    plt.legend(handles=[b1,b2,b3],loc=(0.7,0.7),ncol=1,frameon=False,prop="Times New Roman")    #显示图例，loc图例显示位置(可以用坐标方法显示），ncol图例显示几列，默认为1列,frameon设置图形边框
    leg = plt.gca().get_legend()
    ltext = leg.get_texts()
    plt.setp(ltext, fontsize=15, weight="bold")
    plt.show()
    fig.savefig('huitu.pdf')

def main():
    file = str(sys.argv[1])
    col_pic(file)
    
if __name__=="__main__":
    main() 
```
(2) 执行下述命令进行绘图
```shell
python py csv
```