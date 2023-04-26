👏 Python脚本|使用sklearn对RMSD矩阵进行聚类

---
[TOC]

---
## 脚本
```python
import sys
import numpy as np
from sklearn.cluster import SpectralClustering

def cluster(file_in):
    with open(file_in) as f:
        f1 = f.readlines()
    matrix = []
    for i in f1:
        if i.startswith("#"):
            name_ = np.array(i.strip("#").split(), dtype=np.str_)
            # print(name_)
        else:
            matrix.append(i.split(","))
    matrix = np.matrix(matrix, dtype=np.float64)
    result_cluster = SpectralClustering(2).fit_predict(matrix)
    # print(result_cluster)
    # index = np.where(result_cluster==0)
    print("The first class: ", name_[np.where(result_cluster==0)])
    print("The second class: ", name_[np.where(result_cluster==1)])

def main():
    file_in = sys.argv[1]
    cluster(file_in)

if __name__ == '__main__':
    main()
```

**使用：**
```shell
python py matrix.txt
```
**参考资料：**
1. https://stackoverflow.com/questions/30089675/clustering-cosine-similarity-matrix
2. https://realpython.com/k-means-clustering-python/