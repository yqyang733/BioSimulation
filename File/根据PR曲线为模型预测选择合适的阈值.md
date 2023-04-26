👏 根据PR曲线为模型预测选择合适的阈值

---
[TOC]

---
## 背景说明
由于模型训练集的阴阳性数据不平衡或者其他原因导致二分类模型的阈值选择困难。所以需要根据模型结果选择一个合适的阈值进行预测值分类。  

## 解决方案
选择PR曲线与y=x相交点对应的阈值作为模型分类的阈值。  

## 实现脚本
```python
from sklearn.metrics import precision_recall_curve

lr_precision, lr_recall, thre = precision_recall_curve(y_lab, y_pre)
lr_pre_recall = abs(np.array(lr_precision) - np.array(lr_recall))
threshold = min(zip(lr_pre_recall, thre))[1]
print(threshold)
```

## 参考资料
(1) https://blog.csdn.net/guzhao9901/article/details/107961184  
(2) https://blog.csdn.net/weixin_38314865/article/details/107793043  