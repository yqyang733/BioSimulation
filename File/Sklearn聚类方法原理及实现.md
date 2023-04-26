👏 Sklearn|聚类方法原理及实现

---
[TOC]

---
## Hierarchical clustering
**（scipy.cluster.hierarchy）**
### 根据聚类结果给定cutoff获得分类结果的函数
（1）scipy.cluster.hierarchy.fcluster
（2）scipy.cluster.hierarchy.fclusterdata
（3）scipy.cluster.hierarchy.leaders

### 聚类函数
（1）scipy.cluster.hierarchy.linkage
（2）scipy.cluster.hierarchy.single
（3）scipy.cluster.hierarchy.complete
（4）scipy.cluster.hierarchy.average
（5）scipy.cluster.hierarchy.weighted
（6）scipy.cluster.hierarchy.centroid
（7）scipy.cluster.hierarchy.median
（8）scipy.cluster.hierarchy.ward

### 聚类结果可视化
（1）scipy.cluster.hierarchy.dendrogram

### 将聚类结果看作树并返回数据结构的树性质
（1）scipy.cluster.hierarchy.ClusterNode
（2）scipy.cluster.hierarchy.leaves_list
（3）scipy.cluster.hierarchy.to_tree
（4）scipy.cluster.hierarchy.cut_tree
（5）scipy.cluster.hierarchy.optimal_leaf_ordering