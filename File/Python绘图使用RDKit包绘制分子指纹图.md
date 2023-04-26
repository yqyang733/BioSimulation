# 👏 Python绘图|使用RDKit包绘制分子指纹图

---
[封面PPT](../PPT/Python绘图使用RDKit包绘制分子指纹图.pptx)

---
## Contents
- [原理分析](#原理分析)
- [绘图](#绘图)

---
<a name="原理分析" />

## 1. 原理分析
绘制该图需要两个分子，一个参考分子和另一个目标分子，首先脚本先将目标分子与参考分子的指纹进行相似性比较，然后在目标分子中将指纹的相似性结果可视化出来。

---
<a name="绘图" />

## 2. 绘图
安装RDKit包之后，运行以下命令：
```shell
python py
```
py脚本的内容如下所示：
```python
from rdkit import Chem
from rdkit.Chem import Draw
from rdkit.Chem.Draw import SimilarityMaps
import matplotlib.pyplot as plt

# 目标分子
targetmol = Chem.MolFromSmiles(
    'CC#CC(=O)N1CCC[C@H]1C1=NC(=C2N1C=CN=C2N)C1=CC=C(C=C1)C(=O)NC1=CC=CC=N1')
# 参考分子
refmol = Chem.MolFromSmiles('O=C(/C=C/[C@@H](NC([C@H](CC(C)C)NC([C@@H](NC([C@@H](NC(OC(C)(C)C)=O)[C@H](O)C)=O)C(C)C)=O)=O)C[C@@H]1CCNC1=O)OCC')

d = Draw.MolDraw2DSVG(800, 400)
d.ClearDrawing()
target_mol_simi_fig, maxweight = SimilarityMaps.GetSimilarityMapForFingerprint(
    refmol,
    targetmol,
    lambda m, i: SimilarityMaps.GetMorganFingerprint(
        m, i, radius=2, fpType='bv'),
    draw2d=d
)

d.FinishDrawing()

with open('IC1.svg', 'w+') as outf:
    outf.write(d.GetDrawingText())
```
运行完成之后，则在相应文件夹下生成IC1.svg，即PPT中所示。可用浏览器打开查看，也可导入到PPT中再导出为png或其他格式图片。