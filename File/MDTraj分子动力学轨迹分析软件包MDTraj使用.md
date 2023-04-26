👏 MDTraj|分子动力学轨迹分析软件包MDTraj使用

---
[TOC]

---
## Function
（1）md.load()
```python
traj = md.load(trajectory, top=topology)
```
（2）md.join()
```python
traj_cluster = md.join(traj[i] for i in cluster_frames)
```
（3）md.rmsd()
```python
distances[i] = md.rmsd(traj_cluster, traj_cluster, i, atom_indices=atom_indices)
```
（4）obj.save()
```python
traj.save("cluster1_centroid.pdb")
```

## Atom selection
```python
atom_indices = [a.index for a in traj.topology.atoms if ((a.residue.name == 'LIG') and (a.element.symbol != 'H'))]
```

## Cluster



## Align


## 参考资料
1. [MDTraj官方文档](https://www.mdtraj.org/1.9.8.dev0/index.html)