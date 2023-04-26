# 👏 计算两个小分子空间结构的RMSD用于分子对接挑选分子

---
[封面PPT](../PPT/计算两个小分子空间结构的RMSD用于分子对接挑选分子.pptx)

---
## Contents
- [使用场景](#使用场景)
- [计算思路](#计算思路)
- [具体操作](#具体操作)

---
<a name="使用场景" />

## 使用场景
在进行分子对接或者虚拟筛选的时候，晶体结构中的配体结构对虚拟筛选挑选分子的结果有着很大参考意义，我们往往想选出与晶体结构中配体三维结构上最相似的分子。这篇文章提供了一个粗略的方式用于计算两个配体分子在三维结构上的RMSD。

---
<a name="计算思路" />

## 计算思路
假设有两个3D配体分子，dock_A和dock_B。首先遍历dock_A的所有原子，计算距离每个dock_A原子最近的dock_B的原子间距离并求和。然后使用距离和除以dock_A的原子数，得dis_A；其次遍历dock_B的所有原子，计算距离每个dock_B原子最近的dock_A的原子间距离并求和。然后使用距离和除以dock_B的原子数，得dis_B；然后取dis_A和dis_B的平均值作为最终结果。

---
<a name="具体操作" />

## 具体操作
(1) 将分子结构导入pymol中
(2) 保存下述python脚本为rmsd_3d_structure.py。
```python
from pymol import cmd

def shortest_permol(mol_1, mol_2):
    '''
    ref: mol_1
    compute rmsd of mol_2
    '''
    mol1_model = cmd.get_model(mol_1)
    mol2_model = cmd.get_model(mol_2)
    num = 0
    lst_atom_dis = []
    dis_shorest = 1000
    for a in mol2_model.atom:
        # num += 1
        for b in mol1_model.atom:
            # print("model %s and index %d"%(mol_2, a.index),"model %s and index %d"%(mol_1, b.index))
            dis = cmd.get_distance("model %s and index %d"%(mol_2, a.index), "model %s and index %d"%(mol_1, b.index))
            if dis_shorest > dis:
                dis_shorest = dis
        lst_atom_dis.append(dis_shorest)
        num += dis_shorest
    return num/len(lst_atom_dis)

def rmsd_three(mol_1, mol_2):
    refmol1_commol2 = shortest_permol(mol_1, mol_2)
    refmol2_commol1 = shortest_permol(mol_2, mol_1)
    rmsd = round((refmol1_commol2 + refmol2_commol1)/2,2)
    # print(refmol1_commol2, refmol2_commol1)
    print("The rmsd of " + mol_1 + " between " + mol_2 + " is " + str(rmsd))
    return rmsd

cmd.extend("rmsd_three", rmsd_three)
```
(3) pymol中运行下述命令：
```shell
run e:/rmsd_3d_structure.py
rmsd_three mol_1, mol_2    # mol_1和mol_2是需要进行比较的两个小分子object的名称
```