👏 Pymol|Pymol脚本命令(cmd命令)

---
[TOC]

---

写在前面：后面会根据学习情况不断更新文章~~

Pymol是操作像蛋白，RNA之类的大分子专业软件，可以用于制作精美的图片以及对分子进行操作。但是图形化界面适用于小批量的操作，大批量操作则使用程序化处理更为简便。Pymol也提供了可用于批量化操作的Python包。首先安装Pymol的Python包，然后使用下述命令编写批量化操作脚本即可。导入python的cmd模块：   
```python
from pymol import cmd
```

---
### 1. cmd.remove()
(1) 去除溶剂分子
```python
cmd.remove('solvent')
```
(2) 删除编号为3的残基
```python
cmd.remove("resi 3")
```
(3) 删除所有氢原子
```python
cmd.remove("hydrogens")
```

---
### 2. cmd.h_add()
(1) 加氢
```python
cmd.h_add("6dh0_protein")
```

---
### 3. cmd.get_fastastr()
(1) 获取蛋白残基序列信息
```python
print(cmd.get_fastastr("3u7s_po"))
```
(2) 获取蛋白中某条链的残基序列信息
```python
print(cmd.get_fastastr("model 6A2B and chain C"))
```

---
### 4. cmd.find_pairs()
(1) 获取两个object之间的所有原子对
```python
print(cmd.find_pairs("3u7s_protein","3u7s_ligand",mode=0,cutoff=100))     # cutoff代表的是距离，100埃范围内的所有原子对。
```

---
### 5. cmd.load()
(1) 载入目录中的蛋白或配体
```python
cmd.load("test_protein.pdb", "test")      # "test_protein.pdb"是要载入pymol的蛋白，"test"是载入之后在pymol中显示的object的名字。
```
(2) 将多个pdb文件保存成多帧的一个pdb文件
```python
cmd.load(os.path.join(".", "pdb", lst[i]), "new", state = i + 1)
```

---
### 6. cmd.create()
(1) 获取蛋白口袋一定距离范围内的配体原子
```python
cmd.create("test", "(resn MOL) within 2.5 of test_protein")     # MOL是配体的名字
```

---
### 7. cmd.save()
(1) 保存文件。
```python
cmd.save("test_2.5A.pdb", "test_2.5A")     # "test_2.5A.pdb"是保存成的文件的名字，"test_2.5A"是在pymol中需要保存的object。
cmd.save("test.pdb", "all")
```
(2) 保存pse文件，保持当前状态。
```python
cmd.save(name_+"_color.pse",state=0)
```
(3) 保存轨迹中某一帧的结构。
```python
cmd.save("./pdb/new_" + str(nums[i]) + ".pdb", state = 666)
```
(4) 保存所有state的结构到一个pdb文件中。
```python
cmd.save("test.pdb", state = 0)
```

---
### 8. cmd.delete()
(1) 删除文件
```python
cmd.delete("all")     # 删除所有object
```

---
### 9. cmd.get_chains()
(1) 获取蛋白的链信息
```python
cmd.get_chains("PTEN-TFEB")     # 获取PTEN-TFEB对象中的链信息，输出是["A","B"]列表   

```

---
### 10. cmd.select()
(1) 选择蛋白的某一条链
```python
cmd.select("chain A")
```
(2) 选择某个残基
```python
cmd.select("resn F90")   # F90是残基名称。
```
(3) 选择某残基一定距离范围内的所有残基
```python
cmd.select("(br. all within 1 of resn %s) and (not resn %s)"%(i[2],i[2]))
```
(4) 选择一个object中的一个残基和另一个object
```python
cmd.select("(model %s and resn %s) or (model %s)"%(i[1], i[2], i[0]))
```
(5) 3个object (object_a, object_b, object_c)，寻找 object_c 上与 object_a 和 object_b 残基距离在4埃内的残基
```python
cmd.select("(br. all within 4 of (mode object_a or mode object_b))")
```
(6) 选择某个object中的一段连续残基
```python
cmd.select("resi 1-7 and mode object_b")
```
(7) 选择某个object中的多段残基
```python
cmd.select("resi 3-7+23-34+45-49 and lig_1.pdb")
```
(8) 选择残基上某些特定原子
```python
cmd.select("resi 1 and (name OP1 or name P or name OP2)")
```
(9) 选择一个object附近一定距离内的所有其他object的所有原子
```python
cmd.select("polymer.nucleic within 4 of polymer.protein")   # 选择蛋白4埃范围内的所有RNA原子
```

---
### 11. cmd.identify()
(1) 获取原子序号
```python
cmd.identify("sele", 0)       # 先选中原子，输出结果是原子序号按从小到大顺序组成的列表
```

---
### 12. cmd.alter()
简介：改变，在pymol中进行各种修改。

(1) 更改残基编号，将对象object的残基编号5改成3
```python
cmd.alter("mode object and resi 5", "resi=3")
```
(2) 将对象object的所有残基编号+100
```python
cmd.alter("negative","resi = int(resi) + 100")
cmd.alter("negative","resv = resv + 100")
```
(3) 根据残基编号更改残基的名称
```python
cmd.alter("%s and resi %d and chain A"%(file_in.strip(".pdb"), i), "resn='%s'"%(dic_seq[i]))
```
(4) 给一个没有链编号的蛋白添加链号
```python
cmd.alter("all","chain='A'")
```
(5) 将一段残基添加链号
```python
cmd.alter("resi 333-527","chain='B'")
```
(6) 更改原子的类型于名称
```python
cmd.alter("resn MUT and name %s"%(i[1][0]), "elem = '%s'"%(i[0][1]))
cmd.alter("resn MUT and name %s"%(i[1][0]), "name = '%s'"%(i[1][1]))
```

---
### 13. cmd.iterate()
(1) 获取对象object中所有的残基编号和对应的残基名称
```python
lst = []
cmd.iterate("name ca and model object","lst.append((resi,resn))")     # 因为一个残基中只有一个α碳，所以以α碳为定位获取所有残基的编号和名称。
```
(2) 使用全局字典的形式定义变量
```python
myspace = {"lst":[]}
cmd.iterate("name ca and model 1a2b","lst.append((resi,resn))",space = myspace)
```

---
### 14. cmd.extend()

---

### 15. cmd.color()

简介：设置颜色

语法：

```python
color color-name
color color-name, object-name
color color-name, (selection)
```

颜色参考：https://pymolwiki.org/index.php/Color_Values

（1）设置sele的颜色

```python
cmd.color("brown","sele")
```

### 16. cmd.copy_to()
简介：将一个object复制进入另一个object中并重新进行编链。
示例：
```python
cmd.copy_to("test_0001", "lig_3") # 将lig_3复制进入test_0001。
```

### 17. cmd.set()
简介：更改Pymol的一些设置。

(1) 在保存pdb时去掉其中的TER标记。
```python
cmd.set('pdb_use_ter_records', 0)
```

### 18. cmd.load_traj()
简介：加载动力学轨迹文件。

(1) 加载.xtc轨迹文件。
```python
cmd.load(".pdb")
cmd.load_traj(".xtc")
```

### 19. cmd.count_states()
简介：计算所有states的数目。

(1) 计算pdb中所有states数目。
```python
cmd.count_states(".pdb")
```

### 20. cmd.set_color()
简介：使用任意RGB颜色。

（1）设置10个渐变的蓝色名称。
```python
RGB = [[186,208,248],[168,196,246],[150,184,244],[134,173,242],[115,160,241],[99,149,239],[81,137,237],[65,126,235],[47,113,233],[29,101,231]]

for i in range(len(RGB)):
    cmd.set_color("tmp_%d"%(i),RGB[i])
```

### 21. cmd.color()
简介：为pymol对象设置颜色。

（1）为特定残基设置颜色。
```python
cmd.color("tmp_%d"%(fre), "%s and resi %d"%(name_, int(i)))
```

### 22. cmd.zoom()
简介：根据窗口对视图进行缩放。

（1）以所有object为中心进行缩放。
```python
cmd.zoom("all")
```

### 23. cmd.ray()
简介：窗口打光。

（1）对整个窗口打光。
```python
cmd.ray()
```

### 24. cmd.png()
简介：保存图片。

（1）保存当前视图图片。
```python
cmd.png(os.path.join(".","Figure",cluster + ".png"))
```

### 25. cmd.distance()
简介：测量距离。

（1）测量两个已知ID号码的原子之间的距离。
```python
cmd.distance("id 233","id 3009", state = 1500)
```
（2）测量两个已知ID号码的原子之间的距离并命名生成距离object的名字。
```python
cmd.distance("dist", "%s_A and id 1639"%(name_aptamer), "%s_B and id 2"%(name_aptamer))
```

### 26. cmd.super()
简介：结构比对。
（1）根据蛋白结构特征叠合两个蛋白结构。
```python
cmd.super(pdb_file[a].strip(".pdb"), pdb_file[b].strip(".pdb"), cycles=5)
```

### 27. cmd.set_name()
简介：将对象重命名。
语法：
```python
cmd.set_name(string old_name, string new_name)
```

（1）将object重命名。
```python
cmd.set_name("prod2_12", "prod2_cluster12")
```

### 28. cmd.copy()
简介：复制对象。
语法：
```python
cmd.copy(string target, string source)
```

（1）将一个object进行复制。
```python

```

### 29. cmd.group()
简介：将object进行分组存放。
语法：
```python
cmd.group(name, members, action)
```

（1）将多个object放入一个group中保存。
```python
cmd.group("test","prod2_cluster30 prod2_cluster12")
```

### 30. cmd.ungroup()
简介：将object从group中释放。

（1）将group中的所有object进行释放。
```python
cmd.ungroup("test")
```

### 31. cmd.get_angle()
简介：计算3个原子间形成的角度。

（1）计算state=0状态下给定的3个原子形成的角度。
```python
cmd.get_angle(atom1="chain E and 405/ca",atom2="chain E and 622/ca",atom3="chain E and 991/ca",state=0)
```

### 32. cmd.get_coords()
简介：获取原子的坐标。

（1）获取一个蛋白所有原子的坐标，以一个二维数组的形式存储坐标信息。
```python
cmd.get_coords('polymer.protein', 1)
```

### 33. cmd.id_atom()
简介：获取某个原子的ID。

```python
cmd.id_atom("sele")
```

