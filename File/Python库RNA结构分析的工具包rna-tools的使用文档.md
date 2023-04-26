👏 Python库|RNA结构分析的工具包rna-tools的使用文档

---
[TOC]

---
## 安装
```shell
pip install rna-tools
```

## 快速入门
（1）从pdb数据库中获取结构
```shell
rna_pdb_toolsx.py --fetch 1xjr
```
（2）从pdb数据库中获取集合结构
```
rna_pdb_toolsx.py --fetch-ba 1xjr
```
（3）从pdb文件中获取序列信息
```shell
rna_pdb_toolsx.py --get-seq 1XJR.pdb
# 1XJR
>A:1-47
GGAGUUCACCGAGGCCACGCGGAGUACGAUCGAGGGUACAGUGAAUU
rna_pdb_toolsx.py --get-seq --oneline 1XJR.pdb --color-seq --compact
GGGGGGUUCCCGGGGGGCCCGGCGGGGGGUCGGUCGGGGGGGGUCGGUGGUU # A:1-47 # 1XJR
```
（4）获取pdb文件的二级结构
```shell
rna_x3dna.py 1xjr.pdb
```
（5）删除结构的一部分
```shell
for i in *pdb; do rna_pdb_toolsx.py --delete A:46-56 $i > ../rpr_rm_loop/$i ; done
```
（6）重命名链编号
```shell
rna_pdb_toolsx.py --edit 'B:1-126>A:1-126' 4_das_1_rpr.pdb > 4_das_1_rpr2.pdb
```
（7）更改碱基编号
```shell
rna_pdb_toolsx.py --edit 'A:3-21>A:1-19' 1f27_clean.pdb > 1f27_clean_A1-19.pdb
rna_pdb_toolsx.py --edit 'A:3-21>A:1-19,B:22-32>B:20-30' 1f27_clean.pdb > 1f27_clean_renumb.pdb
```
（8）提取结构的一部分
```shell
rna_pdb_toolsx.py --extract A:1-4 13_Bujnicki_1_rpr.pdb
```
（9）找出结构中缺失的原子
```shell
rna_pdb_toolsx.py --get-rnapuzzle-ready test.pdb
```
（10）碱基突变
```shell
# 将A链1-4碱基突变为A，将B链13碱基突变为A。
rna_pdb_toolsx.py --mutate 'A:1A+2A+3A+4A,B:13A' --inplace 205d_rmH2o_mutA1234-B1_inplace.pdb
```
（11）增加缺失的原子（先简单记住函数名称，用的时候再详细剖析该函数）（在rna_tools_lib.py中）
>RNAStructure.get_rnapuzzle_ready(renumber_residues=True, fix_missing_atoms=True, rename_chains=True, ignore_op3=False, report_missing_atoms=True, keep_hetatm=False, backbone_only=False, no_backbone=False, bases_only=False, verbose=False)

## RNA PDB Tools
### rna_pdb_toolsx（工具接口脚本）
说明：该py文件就是将rna_tools_lib.py文件中的函数做成接口形式，方便命令行进行调用。
（1）用法
> usage: rna_pdb_toolsx.py [-h] [--version] [-r] [--no-progress-bar]
                         [--renum-atoms] [--renum-nmr]
                         [--renum-residues-dirty] [--undo] [--delete-anisou]
                         [--fix] [--to-mol2] [--split-alt-locations] [-c]
                         [--is-pdb] [--is-nmr] [--nmr-dir NMR_DIR] [--un-nmr]
                         [--orgmode] [--get-chain GET_CHAIN] [--fetch]
                         [--fetch-ba] [--fetch-chain] [--get-seq]
                         [--color-seq] [--ignore-files IGNORE_FILES]
                         [--compact] [--hide-warnings] [--get-ss]
                         [--rosetta2generic] [--no-hr] [--renumber-residues]
                         [--dont-rename-chains] [--dont-fix-missing-atoms]
                         [--inspect] [--collapsed-view] [--cv] [-v]
                         [--mutate MUTATE] [--edit EDIT]
                         [--rename-chain RENAME_CHAIN]
                         [--swap-chains SWAP_CHAINS] [--set-chain SET_CHAIN]
                         [--replace-chain REPLACE_CHAIN] [--delete DELETE]
                         [--extract EXTRACT] [--extract-chain EXTRACT_CHAIN]
                         [--uniq UNIQ] [--chain-first] [--oneline] [--fasta]
                         [--cif2pdb] [--pdb2cif] [--mdr]
                         [--get-rnapuzzle-ready] [--rpr] [--keep-hetatm]
                         [--inplace] [--suffix SUFFIX] [--replace-hetatm]
                         [--dont-report-missing-atoms] [--backbone-only]
                         [--no-backbone] [--bases-only]
                         file [file ...]

（2）参数说明：
|参数|说明|
|:----|:----|
|--version|-|
|-r=False, --report=False|-|
|--no-progress-bar=False|进度条|
|--renum-atoms=False|对原子重新编号|
|--renum-nmr=False|-|
|--renum-residues-dirty=False|-|
|--undo=False|恢复上一操作文件，将“备份文件”.pdb~重命名为pdb。|
|--delete-anisou=False|移除带有ANISOU记录的文件。|
|--fix=False|修复pdb文件，使用pdbfixer添加缺失的原子。|
|--to-mol2=False|-|
|--split-alt-locations=False|-|
|-c=False, --clean=False|得到干净的结构。|
|--is-pdb=False|检查文件是否是pdb格式文件。|
|--is-nmr=False|检查是否是NMR类型的多model文件。|
|--nmr-dir|-|
|--un-nmr=False|将NMR类型的多model的pdb文件分割称单models的多个pdb文件。|
|--orgmode=False|获取原始格式的结构文件。|
|--get-chain|需要获取结构的链编号。|
|--fetch=False|从网上自动获取pdb文件。|
|--fetch-ba=False|从网上自动获取pdb集合文件。|
|--fetch-chain=False|从网上自动获取某条链的pdb结构。|
|--get-seq=False|获取序列。|
|--color-seq=False|给序列加上颜色。|
|--ignore-files|需要忽略的文件。|
|--compact=False|和-–get-seq一起使用。|
|--hide-warnings=False|隐藏警告信息。|
|--get-ss=False|获取二级结构信息。|
|--rosetta2generic=False|将ROSETTA格式文件转换为普通pdb格式文件。|
|--no-hr=False|不在文件中插入头部信息。|
|--renumber-residues=False|对碱基进行重编号。|
|--dont-rename-chains=False|和–-get-rnapuzzle-ready一起使用，对链进行重新编号。|
|--dont-fix-missing-atoms=False|仅和–-get-rnapuzzle-ready一起使用。|
|--inspect=False|检查丢失的原子。|
|--collapsed-view=False|-|
|--cv=False|-|
|-v=False, --verbose=False|-|
|--mutate|对碱基进行突变。|
|--edit=|编辑碱基。|
|--rename-chain|对链进行重命名。|
|--swap-chains|交换链名。|
|--set-chain|-|
|--replace-chain=|一个文件的PDB名称与一条链将被用来替换原来的PDB文件中的链，该文件中的链id必须与原链的链id相同。|
|--delete=|删除碱基。|
|--extract=|提取RNA片段。|
|--extract-chain|提取特定的链。|
|--uniq|-|
|--chain-first=False|-|
|--oneline=False|-|
|--fasta=False|和-–get-seq一起使用，将序列以fasta格式形式展示。|
|--cif2pdb=False|pymol包。|
|--pdb2cif=False|pymol包。|
|--mdr=False|准备MD的准备文件。|
|--get-rnapuzzle-ready=False|-|
|--rpr=False|-|
|--keep-hetatm=False|保留杂原子。|
|--inplace=False|在原始文件中进行编辑。|
|--suffix|对文件进行流式重新命名。|
|--replace-hetatm=False|将‘HETATM’替换为‘ATOM’。|
|--dont-report-missing-atoms=False|仅和–-get-rnapuzzle-ready一起使用。|
|--backbone-only=False|仅保留骨架，移除碱基。|
|--no-backbone=False|移除骨架原子。|
|--bases-only=False|仅保留碱基原子。|

### 准备好RNAPuzzle
（1）函数所属类位置：
```python
class rna_tools.rna_tools_lib.RNAStructure(fn)
```
（2）函数：
```python
get_rnapuzzle_ready(renumber_residues=True, fix_missing_atoms=True, rename_chains=True, ignore_op3=False, report_missing_atoms=True, keep_hetatm=False, backbone_only=False, no_backbone=False, bases_only=False, verbose=False)
```
可以用于修补缺失的原子。  

### 获取序列
（1）函数所属类位置：
```python
class rna_tools.rna_tools_lib.RNAStructure(fn)
```
（2）函数：
```python
get_seq(compact=False, chainfirst=False, fasta=False, addfn='', color=False)
```

### 网上下载结构
（1）函数：
```python
rna_tools.rna_tools_lib.fetch(pdb_id, path='.')
```

### 网上下载晶体结构集合
（1）函数：
```python
rna_tools.rna_tools_lib.fetch_ba(pdb_id, path='.')
```

### 删除
```shell
for i in *pdb; do rna_pdb_toolsx.py --delete A:46-56 $i > ../rpr_rm_loop/$i ; done
```

### 编辑结构
```python
rna_tools.rna_tools_lib.edit_pdb(f, args)
```

### rna_tools_lib工具合集
**（1）RNAStructure类。**
函数位置：
```python
class rna_tools.rna_tools_lib.RNAStructure(fn)
```
* edit_occupancy_of_pdb(txt, pdb, pdb_out, v=False)
* fix_O_in_UC()
* fix_op_atoms()
* fix_with_qrnas(outfn='', verbose=False)
添加缺失的重原子
* get_all_chain_ids()
* get_atom_code(line)
* get_atom_coords(line)
* get_atom_num(line)
* get_info_chains()
* get_remarks_text()
* get_report()
* get_res_code(line)
* get_res_num(line)
* get_res_text(chain_id, resi)
* get_rnapuzzle_ready(renumber_residues=True, fix_missing_atoms=True, rename_chains=True, ignore_op3=False, report_missing_atoms=True, keep_hetatm=False, backbone_only=False, no_backbone=False, bases_only=False, verbose=False)
* get_seq(compact=False, chainfirst=False, fasta=False, addfn='', color=False)
* get_text(add_end=True)
* is_amber_like()
* is_mol2()
* is_nmr()
* is_pdb()
* remove(verbose)
* remove_ion()
* remove_water()
* rename_chain(chain_id_old, chain_id_new, debug=False)
* renum_atoms()
* set_atom_occupancy(line, occupancy)
* set_occupancy_atoms(occupancy)
* set_res_code(line, code)
* std_resn()
* un_nmr(startwith1=True, directory='', verbose=False)
* write(outfn='', verbose=False)

**（2）rna_tools.rna_tools_lib.aa3to1(aaa)。**
**（3）rna_tools.rna_tools_lib.collapsed_view(args)。**
**（4）rna_tools.rna_tools_lib.edit_pdb(f, args)。**
**（5）rna_tools.rna_tools_lib.fetch(pdb_id, path='.')。**
**（6）rna_tools.rna_tools_lib.fetch_ba(pdb_id, path='.')。**
**（7）rna_tools.rna_tools_lib.fetch_cif_ba(cif_id, path='.')。**
**（8）rna_tools.rna_tools_lib.load_rnas(path, verbose=True)。**
**（9）rna_tools.rna_tools_lib.replace_chain(struc_fn, insert_fn, chain_id)。**
**（10）rna_tools.rna_tools_lib.set_chain_for_struc(struc_fn, chain_id, save_file_inplace=False, skip_ter=True)。**
**（11）rna_tools.rna_tools_lib.sort_strings(l)。**

### 编辑PDB的Bfactor和Occupancy的工具
（1）用法
> usage: rna_pdb_edit_occupancy_bfactor.py [-h] (--bfactor | --occupancy)
                                         [--select SELECT] [--set-to SET_TO]
                                         [--set-not-selected-to SET_NOT_SELECTED_TO]
                                         [-o OUTPUT] [--verbose]
                                         [--select-atoms SELECT_ATOMS]
                                         file

（2）参数
|参数|说明|
|:----|:----|
|--bfactor=False|设置bfactor。|
|--occupancy=False|设置occupancy。|
|--select|例如：A:1-10。|
|--set-to=1|-|
|--set-not-selected-to=0|-|
|-o, --output|输出文件。|
|--verbose=False|详细模式。|
|--select-atoms|只选择给定的原子。|

（3）函数所在位置及应用。
```python
rna_tools.tools.rna_pdb_edit_occupancy_bfactor.rna_pdb_edit_occupancy_bfactor.edit_occupancy_of_pdb(txt, pdb, pdb_out, bfactor, occupancy, set_to, set_not_selected_to, select_atoms, v=False)
```

### 给文件加链
（1）命令行
```shell
rna_add_chain.py -c X 1msy_rnakbmd_decoy999_clx_noChain.pdb > 1msy_rnakbmd_decoy999_clx_noChain_Xchain.pdb
```
（2）函数所在位置。
```python
rna_tools.tools.misc.rna_add_chain.get_parser()
```

### 测量原子间距离
（1）用法
> usage: pdbs_measure_atom_dists [-h] [-v] seqid1 seqid2 alignfn pdbfn1 pdbfn2

（2）参数
|参数|说明|
|:----|:----|
|seqid1|比对中的序列1编号。|
|seqid2|比对中的序列2编号。|
|alignfn|以fasta格式进行比对。|
|pdbfn1|第一个pdb文件。|
|pdbfn2|第二个pdb文件。|

（3）函数位置。
```python
rna_tools.tools.pdbs_measure_atom_dists.pdbs_measure_atom_dists.find_core(seq_with_gaps1, seq_with_gaps2)
```
```python
rna_tools.tools.pdbs_measure_atom_dists.pdbs_measure_atom_dists.get_parser()
```
```python
rna_tools.tools.pdbs_measure_atom_dists.pdbs_measure_atom_dists.get_seq(alignfn, seqid)
```
```python
rna_tools.tools.pdbs_measure_atom_dists.pdbs_measure_atom_dists.map_coords_atom(structure)
```
```python
rna_tools.tools.pdbs_measure_atom_dists.pdbs_measure_atom_dists.open_pdb(pdbfn)
```

## RNA Tools
### RNA序列
&emsp;&emsp;该工具中一共提供了5种预测RNA序列二级结构的方法，分别是：RNAfold、RNAsubopt、ipknot、contextfold和centroid_fold。但是使用之前需要安装这些软件。

**安装。**
|软件名称|安装|
|:----|:----|
|ContextFold|https://www.cs.bgu.ac.il/~negevcb/contextfold/|
|ViennaRNA|https://www.tbi.univie.ac.at/RNA/|
|ipknot OSX|https://github.com/satoken/homebrew-rnatools|
|RNA Structure|http://rna.urmc.rochester.edu/|

**使用小技巧。**
&emsp;&emsp;预测多条序列时可使用下述脚本。
```python
from rna_tools import Seq

f = open("listOfSequences.fasta")
for line in f:
    if line.startswith('>'):
        print line,
    else:
        print line,
        s = Seq.Seq(line.strip()) # module first Seq and class second Seq #without strip this has two lines
        print s.predict_ss(method="contextfold"),
        #print s.predict_ss(method="centroid_fold")
```

**RNASequence类。**
（1）函数位置。
```python
class rna_tools.Seq.RNASequence(seq, ss='', name='rna_seq')
```
（2）用法。
```python
>>> seq = RNASequence("CCCCUUUUGGGG")
>>> seq.name = 'RNA03'
>>> print(seq.predict_ss("RNAfold", constraints="((((....))))"))
>RNA03
CCCCUUUUGGGG
((((....)))) ( -6.40)
```
（3）该类中的常用函数。
* eval(ss='', no_dangling_end_energies=False, verbose=False)
评估RNA序列的能量。
* get_foldability(ss='', verbose=False)
基于EntRNA计算可折叠性。
* predict_ss(method='RNAfold', constraints='', enforce_constraint=False, shapefn='', explore='', verbose=0, path='')
预测序列的二级结构。
* load_fasta_ss_into_RNAseqs(fn, debug=True)

### RNA二级结构
**SecondaryStructure常用函数。**
* rna_tools.SecondaryStructure.draw_ss(title, seq, ss, img_out, resolution=4, verbose=False)
使用VARNA绘制二级结构。
```python
>>> seq = 'GGAAACC'
>>> ss =  '((...))'
>>> img_out = 'output/demo.png'
>>> draw_ss('rna', seq, ss, img_out)
>>> print('Made %s' % img_out)
Made output/demo.png
```
* rna_tools.SecondaryStructure.parse_vienna_to_pairs(ss, remove_gaps_in_ss=False)
使用Parse Vienna方法获取成对碱基。
```python
>>> parse_vienna_to_pairs('((..))')
([[1, 6], [2, 5]], [])

>>> parse_vienna_to_pairs('(([[))]]')
([[1, 6], [2, 5]], [[3, 8], [4, 7]])

>>> parse_vienna_to_pairs('((--))')
([[1, 6], [2, 5]], [])

>>> parse_vienna_to_pairs('((--))', remove_gaps_in_ss=True)
([[1, 4], [2, 3]], [])

>>> parse_vienna_to_pairs('((((......')
Traceback (most recent call last):
  File "/usr/lib/python2.7/doctest.py", line 1315, in __run
    compileflags, 1) in test.globs
  File "<doctest __main__.parse_vienna_to_pairs[4]>", line 1, in <module>
    parse_vienna_to_pairs('((((......')
  File "./SecondaryStructure.py", line 106, in parse_vienna_to_pairs
    raise ExceptionOpenPairsProblem('Too many open pairs (()) in structure')
ExceptionOpenPairsProblem: Too many open pairs (()) in structure
```
**rna_dot2ct.py**
&emsp;&emsp;将输入序列文件转换为.ct格式文件。

**二级结构格式转换**
（1）常用函数。
* rna_tools.tools.rna_convert_pseudoknot_formats.rna_ss_pk_to_simrna.get_multiple_lines(ss)
* rna_tools.tools.rna_convert_pseudoknot_formats.rna_ss_pk_to_simrna.get_parser()
* rna_tools.tools.rna_convert_pseudoknot_formats.rna_ss_pk_to_simrna.is_pk(ss)
（2）示例
```python
## from
> a
....((.(..(((....)))..((((.(.........).)))....).)).......((((......))))
..............................((((...................))))..............

## to
> a
....((.(..(((....)))..((((.(..[[[[...).)))....).))...]]]]((((......))))
```

### 搜索
**Blast PDB**
（1）类位置
```python
classrna_tools.BlastPDB.BlastPDB(seq)
```
（2）用法
```python
>>> p = BlastPDB('GGGUCAGGCCGGCGAAAGUCGCCACAGUUUGGGGAAAGCUGUGCAGCCUGUAACCCCCCCACGAAAGUGGG')
>>> p.search()
>>> p.result  
u'<HTML>\n<TITLE>BLAST Search Results</TITLE>...
```

**Rfam Search**


## 参考资料
1. https://rna-tools.readthedocs.io/en/latest/index.html