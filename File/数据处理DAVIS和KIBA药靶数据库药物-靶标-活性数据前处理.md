# 👏 数据处理|DAVIS和KIBA药靶数据库药物-靶标-活性数据前处理

---
[封面PPT](../PPT/数据处理DAVIS和KIBA药靶数据库药物-靶标-活性数据前处理.pptx)

---
## Contents
- [DAVIS数据库简介](#DAVIS数据库简介)
- [KIBA数据库简介](#KIBA数据库简介)
- [DAVIS和KIBA数据格式](#DAVIS和KIBA数据格式)
- [前处理脚本](#前处理脚本)

---
<a name="DAVIS数据库简介" />

## 1. DAVIS数据库简介

---
<a name="KIBA数据库简介" />

## 2. KIBA数据库简介

---
<a name="DAVIS和KIBA数据格式" />

## 3. DAVIS和KIBA数据格式
DAVIS和KIBA两个药靶数据库的格式非常相似，都是由affinity.txt、SMILES.txt和target_seq.txt三个文件组成的。以DAVIS数据库为例，SMILES.txt文件中记录了所有配体SMILES信息，target_seq.txt文件中记录了所有蛋白靶标FASTA信息。affinity.txt中以矩阵的形式记录了所有配体蛋白间的相互作用值。如PPT中所示。其中affinity.txt中的行顺序和列顺序与target_seq.txt和SMILES.txt的顺序是对应起来的。

---
<a name="前处理脚本" />

## 4. 前处理脚本
```python
def load_process_DAVIS(path = './data', binary = False, convert_to_log = True, threshold = 30):
    print('Beginning Processing...')

    if not os.path.exists(path):
        os.makedirs(path)

    url = 'https://drive.google.com/uc?export=download&id=14h-0YyHN8lxuc0KV3whsaSaA-4KSmiVN'
    saved_path = wget.download(url, path)

    print('Beginning to extract zip file...')
    with ZipFile(saved_path, 'r') as zip:
        zip.extractall(path = path)

    affinity = pd.read_csv(path + '/DAVIS/affinity.txt', header=None, sep = ' ')

    with open(path + '/DAVIS/target_seq.txt') as f:
        target = json.load(f)

    with open(path + '/DAVIS/SMILES.txt') as f:
        drug = json.load(f)

    target = list(target.values())
    drug = list(drug.values())

    SMILES = []
    Target_seq = []
    y = []

    for i in range(len(drug)):
        for j in range(len(target)):
            SMILES.append(drug[i])
            Target_seq.append(target[j])
            y.append(affinity.values[i, j])    # 将矩阵的形式转换成配体-靶标-活性一一对应的形式。

    if binary:
        print('Default binary threshold for the binding affinity scores are 30, you can adjust it by using the "threshold" parameter')
        y = [1 if i else 0 for i in np.array(y) < threshold]
    else:
        y = y
    print('Done!')
    return np.array(SMILES), np.array(Target_seq), np.array(y)

def load_process_KIBA(path = './data', binary = False, threshold = 9):
    print('Beginning Processing...')


    if not os.path.exists(path):
        os.makedirs(path)

    url = 'https://drive.google.com/uc?export=download&id=1fb3ZI-3_865OuRMWNMzLPnbLm9CktM44'
    saved_path = wget.download(url, path)

    print('Beginning to extract zip file...')
    with ZipFile(saved_path, 'r') as zip:
        zip.extractall(path = path)

    affinity = pd.read_csv(path + '/KIBA/affinity.txt', header=None, sep = '\t')
    affinity = affinity.fillna(-1)

    with open(path + '/KIBA/target_seq.txt') as f:
        target = json.load(f)

    with open(path + '/KIBA/SMILES.txt') as f:
        drug = json.load(f)

    target = list(target.values())
    drug = list(drug.values())

    SMILES = []
    Target_seq = []
    y = []

    for i in range(len(drug)):
        for j in range(len(target)):
            if affinity.values[i, j] != -1:
                SMILES.append(drug[i])
                Target_seq.append(target[j])
                y.append(affinity.values[i, j])

    if binary:
        print('Note that KIBA is not suitable for binary classification as it is a modified score. \
               Default binary threshold for the binding affinity scores are 9, \
               you should adjust it by using the "threshold" parameter')
        y = [1 if i else 0 for i in np.array(y) < threshold]
    else:
        y = y

    print('Done!')
    return np.array(SMILES), np.array(Target_seq), np.array(y)
```