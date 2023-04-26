# 👏 数据处理|BindingDB数据库药物-靶标-活性数据前处理

---
[封面PPT](../PPT/数据处理BindingDB数据库药物-靶标-活性数据前处理.pptx)

---
## Contents
- [BindingDB数据库简介]
- [BindingDB数据格式]
- [BindingDB数据库前处理脚本]

---
<a name="BindingDB数据库简介" />

## BindingDB数据库简介

---
<a name="BindingDB数据格式" />

## BindingDB数据格式
BindingDB数据存储的文件格式是tsv文件，可以用文本编辑器打开，但是文件太大可能导致文本编辑器运行终止，建议取前几百行打开看看格式即可，分隔符是tab键。文件的数据结构见PPT。

---
<a name="BindingDB数据库前处理脚本" />

## BindingDB数据库前处理脚本
```python
def process_BindingDB(path = None, df = None, y = 'Kd', binary = False, convert_to_log = True, threshold = 30):
    '''
    该函数提供了处理BindingDB数据库，从而获取数据库中所有配体化合物SMILES、蛋白靶标Uniprot编号和活性数据的方式。
    '''
    if not os.path.exists(path):
        os.makedirs(path)

    if df is not None:
        print('Loading Dataset from the pandas input...')
    else:
        print('Loading Dataset from path...')
        df = pd.read_csv(path, sep = '\t', error_bad_lines=False)
    print('Beginning Processing...')
    df = df[df['Number of Protein Chains in Target (>1 implies a multichain complex)'] == 1.0]   # 筛选出单链靶标蛋白的所有行
    df = df[df['Ligand SMILES'].notnull()]   # 去掉所有没有配体化合物SMILES的所有行

    if y == 'Kd':
        idx_str = 'Kd (nM)'
    elif y == 'IC50':
        idx_str = 'IC50 (nM)'
    elif y == 'Ki':
        idx_str = 'Ki (nM)'
    elif y == 'EC50':
        idx_str = 'EC50 (nM)'
    else:
        print('select Kd, Ki, IC50 or EC50')   # 选择一种活性表示形式

    df_want = df[df[idx_str].notnull()]   # 去掉没有对应活性的行
    df_want = df_want[['BindingDB Reactant_set_id', 'Ligand InChI', 'Ligand SMILES',\
                      'PubChem CID', 'UniProt (SwissProt) Primary ID of Target Chain',\
                      'BindingDB Target Chain  Sequence', idx_str]]    # 选出需要的列
    df_want.rename(columns={'BindingDB Reactant_set_id':'ID',
                            'Ligand SMILES':'SMILES',
                            'Ligand InChI':'InChI',
                            'PubChem CID':'PubChem_ID',
                            'UniProt (SwissProt) Primary ID of Target Chain':'UniProt_ID',
                            'BindingDB Target Chain  Sequence': 'Target Sequence',
                            idx_str: 'Label'},
                            inplace=True)   # 将列重命名

    df_want['Label'] = df_want['Label'].str.replace('>', '')
    df_want['Label'] = df_want['Label'].str.replace('<', '')
    df_want['Label'] = df_want['Label'].astype(float)   # 因为里面有很多没有确定的数据，给的是范围，这里将范围变成确定的数据值。

    # have at least uniprot or pubchem ID
    df_want = df_want[df_want.PubChem_ID.notnull() | df_want.UniProt_ID.notnull()]
    df_want = df_want[df_want.InChI.notnull()]   # 去掉没有uniprot编号和pubchem编号的行

    df_want = df_want[df_want.Label <= 10000000.0]   # 根据活性强弱取出一定活性阈值之上的行
    print('There are ' + str(len(df_want)) + ' drug target pairs.')

    if binary:
        print('Default binary threshold for the binding affinity scores are 30, you can adjust it by using the "threshold" parameter')
        y = [1 if i else 0 for i in df_want.Label.values < threshold]   # 如果是二分类模型则需要表示的是配体与蛋白是否有相互作用，这里以0和1的形式表示配体蛋白间的相互作用。这里可以自行设置活性阈值对是否有相互作用进行判断。阈值之上则有相互作用，以1表示；阈值之下则无相互作用，以0表示。
    else:
        y = df_want.Label.values

    return df_want.SMILES.values, df_want['Target Sequence'].values, np.array(y)
```