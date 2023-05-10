👏 FEP双拓扑准备相关

---
[TOC]

---
**文献：** [TIES 2.0: A Dual-Topology Open Source Relative Binding Free Energy Builder with Web Portal](FEP双拓扑准备相关/acs.jcim.2c01596.pdf)
**文献：** [JCTC | TIES 20: Relative Binding Free Energy with a Flexible Superimposition Algorithm and Partial Ring Morphing](FEP双拓扑准备相关/acs.jctc.0c01179.pdf)

**文献：** [Validation of the Alchemical Transfer Method for the Estimation of Relative Binding Affinities of Molecular Series](FEP双拓扑准备相关/acs.jcim.3c00178.pdf)


## Gromacs双拓扑准备
### pmx
Github地址：https://github.com/deGrootLab/pmx
Webserver: http://pmx.mpibpc.mpg.de/tutorial.html
Documentation: https://degrootlab.github.io/pmx/index.html

**pmx安装注意点：** pmx安装的时候新创环境之后，先将对应的python版本以及numpy，matplotlib以及scipy都安装上之后，再执行setup.py文件对pmx进行安装。

**pmx使用：**
#### 命令行使用：
**pmx mutate**
（1）运行的主体py名称是mutate.py文件
（2）参数
|参数|意义|
|:----|:----|
|-f|Input structure file in PDB or GRO format.|
|-fB|Input structure file of the B state in PDB. ???这个选项的意义在哪里？|
|-o|Output structure file in PDB or GRO format.|
|-ff|Force field to use. If none is provided, a list of available ff will be shown.|
|--script|Text file with list of mutations (optional).|
|--keep_resid|保留输入文件的残基编号信息，否则会将残基全部进行重新编号。|
|--ref|和--keep_resid不能同时使用。如果对输入进行了重新编号，可以同时提供一个没有重新编号的参考文件，然后根据参考文件的残基编号进行选择突变。|
|--resinfo|Show the list of 3-letter -> 1-letter residues.|
（3）m = Model(infile, renumber_residues=renumber, bPDBTER=True, rename_atoms=True, scale_coords='A')

#### Script
[mutate.py](FEP双拓扑准备相关/mutate.py)
[model.py](FEP双拓扑准备相关/model.py)
[alchemy.py](FEP双拓扑准备相关/alchemy.py)
[atomselection.py](FEP双拓扑准备相关/atomselection.py)
[atom.py](FEP双拓扑准备相关/atom.py)
[chain.py](FEP双拓扑准备相关/chain.py)
[utils.py](FEP双拓扑准备相关/utils.py)
[mutdb.py](FEP双拓扑准备相关/mutdb.py)
[parser.py](FEP双拓扑准备相关/parser.py)
[molecule.py](FEP双拓扑准备相关/molecule.py)
[library.py](FEP双拓扑准备相关/library.py)

#### Some Class
##### Model
**输入参数**
|名称(默认值)|作用|
|:----|:----|
|filename(None)|输入的pdb或者gro文件名称|
|pdbline(None)|和filename的内容是并列关系，相当于filename.read()|
|renumber_atoms(True)|是否对原子进行重新编号|
|renumber_residues(True)|是否对残基进行重新编号|
|rename_atoms(False)|是否对原子进行重新命名|
|scale_coords(False)|是否更换坐标的单位(Å/nm)|
|bPDBTER(True)||
|bNoNewID(True)||
|bPDBGAP(False)||
|bPDBMASS(False)||

**类属性**
|名称|作用|
|:----|:----|
|self.title('PMX MODEL')|pdb文件中的TITLE|
|self.filename|pdb文件名称|
|self.chains||
|self.chdic||
|self.residues||
|self.name||
|self.id||
|self.have_bonds||
|self.box||
|self.unity||
|||

**类方法**
|名称|作用|
|:----|:----|
|self.read()||
|self.make_chains()||
|self.make_residues()||
|self.renumber_residues()|对残基的id进行从1开始的重新编号。|
|self.assign_moltype()|判断分子是蛋白，DNA，RNA还是有机小分子。|
|self.writePIR()|将体系的序列信息打印出来。PIR格式。|
|self.writeFASTA()|将体系的序列信息打印出来。FASTA格式。|
|||

##### Atom
**输入参数**
|名称(默认值)|作用|
|:----|:----|
|line(None)|输入的pdb的文本内容|
|mol2line(None)|输入的mol2格式文件的文本内容|

**类属性**
|名称|作用|
|:----|:----|

**类方法**
|名称|作用|
|:----|:----|
|self.readPDBString()|输入是pdb文件的一个原子（一行），从这行内容中获取原子的坐标，链等信息并返回含有各种原子属性的原子类。|


##### Molecule
**输入参数**
|名称(默认值)|作用|
|:----|:----|
|||

##### Atomselection
**输入参数**
|名称(默认值)|作用|
|:----|:----|
|||

**类属性**
|名称|作用|
|:----|:----|

**类方法**
|名称|作用|
|:----|:----|
|self.renumber_atoms()|对原子的id进行从1开始的重新编号。|
|self.rename_atoms_to_gmx()|修改原子的名称以适配gmx格式，若原子名称以数字开头，则将数字移至末尾位置。|
|self.a2nm()|对输入文件的坐标进行单位换算，将Å换算成nm。|
|self.nm2a()|对输入文件的坐标进行单位换算，将nm换算成Å。|
