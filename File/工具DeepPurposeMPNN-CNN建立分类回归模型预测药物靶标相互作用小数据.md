👏 工具|DeepPurpose|MPNN-CNN建立分类/回归模型预测药物靶标相互作用(小数据)

---

[封面PPT](../PPT/工具DeepPurposeMPNN-CNN建立分类回归模型预测药物靶标相互作用小数据.pptx)

---

[TOC]

---

## 生成输入文件

### Smiles处理

(1) Smiles正则化处理

参考 [使用molvs包进行Smiles标准化处理](./使用molvs包进行Smiles标准化处理.md)

(2) 对Smiles去盐

参考 [使用MayaChemTools对Smiles去盐](./使用MayaChemTools对Smiles去盐.md)

(3) 再次正则化

同 (1)

(4) 选出不能正常编码的SMILES的ID

```python
import sys
import torch
from rdkit import Chem, DataStructs
from rdkit.Chem import AllChem

ELEM_LIST = ['C', 'N', 'O', 'S', 'F', 'Si', 'P', 'Cl', 'Br', 'Mg', 'Na', 'Ca', 'Fe', 'Al', 'I', 'B', 'K', 'Se', 'Zn', 'H', 'Cu', 'Mn', 'unknown']
ATOM_FDIM = len(ELEM_LIST) + 6 + 5 + 4 + 1
BOND_FDIM = 5 + 6
MAX_NB = 6

def get_mol(smiles):
    mol = Chem.MolFromSmiles(smiles)
    if mol is None:
        return None
    Chem.Kekulize(mol)
    return mol

def onek_encoding_unk(x, allowable_set):
    if x not in allowable_set:
        x = allowable_set[-1]
    return list(map(lambda s: x == s, allowable_set))

def atom_features(atom):
    return torch.Tensor(onek_encoding_unk(atom.GetSymbol(), ELEM_LIST)
            + onek_encoding_unk(atom.GetDegree(), [0,1,2,3,4,5])
            + onek_encoding_unk(atom.GetFormalCharge(), [-1,-2,1,2,0])
            + onek_encoding_unk(int(atom.GetChiralTag()), [0,1,2,3])
            + [atom.GetIsAromatic()])

def bond_features(bond):
    bt = bond.GetBondType()
    stereo = int(bond.GetStereo())
    fbond = [bt == Chem.rdchem.BondType.SINGLE, bt == Chem.rdchem.BondType.DOUBLE, bt == Chem.rdchem.BondType.TRIPLE, bt == Chem.rdchem.BondType.AROMATIC, bond.IsInRing()]
    fstereo = onek_encoding_unk(stereo, [0,1,2,3,4,5])
    return torch.Tensor(fbond + fstereo)

def smiles2mpnnfeature(id, smiles):
    #flag = 0
    try:
        padding = torch.zeros(ATOM_FDIM + BOND_FDIM)
        fatoms, fbonds = [], [padding]
        in_bonds,all_bonds = [], [(-1,-1)]
        mol = get_mol(smiles)
        n_atoms = mol.GetNumAtoms()
        for atom in mol.GetAtoms():
            fatoms.append(atom_features(atom))
            in_bonds.append([])

        for bond in mol.GetBonds():
            a1 = bond.GetBeginAtom()
            a2 = bond.GetEndAtom()
            x = a1.GetIdx()
            y = a2.GetIdx()

            b = len(all_bonds)
            all_bonds.append((x,y))
            fbonds.append( torch.cat([fatoms[x], bond_features(bond)], 0) )
            in_bonds[y].append(b)

            b = len(all_bonds)
            all_bonds.append((y,x))
            fbonds.append( torch.cat([fatoms[y], bond_features(bond)], 0) )
            in_bonds[x].append(b)

        total_bonds = len(all_bonds)
        fatoms = torch.stack(fatoms, 0)
        fbonds = torch.stack(fbonds, 0)
        agraph = torch.zeros(n_atoms,MAX_NB).long()
        bgraph = torch.zeros(total_bonds,MAX_NB).long()
        for a in range(n_atoms):
            for i,b in enumerate(in_bonds[a]):
                agraph[a,i] = b

        for b1 in range(1, total_bonds):
            x,y = all_bonds[b1]
            for i,b2 in enumerate(in_bonds[x]):
                if all_bonds[b2][0] != y:
                    bgraph[b1,i] = b2

    except:
        #flag = 1
        return id

def check(file):
    with open(file) as f:
        f1 = f.readlines()
    result = open("err.txt","w")
    #result.write("fggggggg")
    for line in f1:
        id = smiles2mpnnfeature(line.split(",")[0], line.split(",")[1].strip())
        #print(id)
        if id is not None:
            #print(id)
            #result.write("yyyyyyy")
            result.write(id + "\n")
            result.flush()

def main():
    file = str(sys.argv[1])
    check(file)

if __name__ == '__main__':
    main()
```

(5) 去除不能正常编码的SMILES

```python
import sys

def pick(file):
    lst = []
    with open("err.txt") as f:
        f1 = f.readlines()
    for line in f1:
        lst.append(line.strip())
    with open(file) as f:
        f1 = f.readlines()
    result = open("drug_protein_1.txt","w")
    for line in f1:
        if line.split(",")[0] in lst:
            pass
        else:
            result.write(line)

def main():
    file = sys.argv[1]
    pick(file)

if __name__ == '__main__':
    main()
```

### 生成 drug_smiles, protein_fasta, drug_protein 文件

（1）drug_smiles和protein_fasta文件格式如PPT中所示。

（2）分类模型的drug_protein文件格式如PPT中所示。

（3）回归模型的drug_protein文件格式如PPT中所示。

---

## 十折交叉训练模型确定超参数

### 分类模型

（1）创建主体文件D-MPNN_T-CNN.py，用于将多个功能性py模块连接起来实现DTI预测。

```python
import sys
from collections import defaultdict
import pandas as pd
import numpy as np
from sklearn.model_selection import StratifiedKFold
from sklearn.preprocessing import LabelEncoder
from DeepPurpose import utils
from DeepPurpose import DTI as models

def smiles_embed(smiles):
    with open(smiles) as f:
        f1 = f.readlines()
    dict_icv_drug_encoding = {}
    for line in f1:
        dict_icv_drug_encoding[line.split(",")[0]] = utils.smiles2mpnnfeature(line.split(",")[1].strip())
    return dict_icv_drug_encoding

def protein_embed(protein):
    with open(protein) as f:
        f1 = f.readlines()
    dict_target_protein_encoding = {}
    for line in f1:
        dict_target_protein_encoding[line.split(",")[0]] = utils.trans_protein(line.split(",")[1].strip())
    return dict_target_protein_encoding

def sampling(DTI, dict_icv_drug_encoding, dict_target_protein_encoding):
    target_set = list(dict_target_protein_encoding.keys())
    with open(DTI) as f:
        f1 = f.readlines()
    dti_mutidict = defaultdict(list)
    [dti_mutidict[i.split(",")[0]].append(i.split(",")[1].strip()) for i in f1]
    whole_positive = []
    whole_negetive = []
    for key in dti_mutidict:
        for i in dti_mutidict[key]:
            whole_positive.append([key,i,dict_icv_drug_encoding[key],dict_target_protein_encoding[i],1])
        target_no = target_set[:]
        [target_no.remove(i) for i in dti_mutidict[key]]
        for a in target_no:
            whole_negetive.append([key,a,dict_icv_drug_encoding[key],dict_target_protein_encoding[a],0])
    #print("whole_positive: ",whole_positive[0])
    #print("whole_negetive: ",whole_negetive[0])
    whole_positive = np.array(whole_positive,dtype=object)
    whole_negetive = np.array(whole_negetive,dtype=object)
    whole_positive_muti_10 = np.tile(whole_positive,(10,1))
    #print(len(whole_positive),len(whole_positive_muti_10))
    #[print(i) for i in whole_positive_muti_10[:,0]]
    np.random.seed(10)
    whole_negetive_index = np.random.choice(np.arange(len(whole_negetive)), size=10 * len(whole_positive),replace=False)
    whole_negetive_1 = np.array([whole_negetive[i] for i in whole_negetive_index])
    # print(len(whole_positive_muti_10),len(whole_negetive_1))
    # print(whole_negetive_1[:,0])
    data_set = np.vstack((whole_positive_muti_10,whole_negetive_1))
    #print(len(data_set),data_set[0])

    #all_fold_dataset = []
    fold_dataset = []
    #print(data_set[:,0:4][0:2],data_set[:,4][0:2])
    #print(type(data_set[:, 4]))
    kf = StratifiedKFold(n_splits=10, shuffle=True, random_state=10)
    print((len(data_set)))
    #temp = np.array([[i,1] for i in range(len(data_set))])
    #print(len(temp))
    #label_encoder = LabelEncoder()
    y = LabelEncoder().fit_transform(data_set[:, 4])
    for train, test in kf.split(data_set[:, 0:4], y):
        train_data, test_data = data_set[train], data_set[test]
        one_fold_dataset = []
        one_fold_dataset.append(train_data)
        one_fold_dataset.append(test_data)
        fold_dataset.append(one_fold_dataset)
    # print(len(fold_dataset[0][0]))
    # print(len(fold_dataset[0][1]))
    # print((fold_dataset[0][0][0]))
    # output = pd.DataFrame(fold_dataset[0][0])
    # print(output)
    return fold_dataset

def get_train_test(fold_1):
    train = pd.DataFrame(fold_1[0])
    train.rename(columns={0:'ICV',
                        1: 'Target',
                        2: 'drug_encoding',
                        3: 'target_encoding',
                        4: 'Label'},
                        inplace=True)
    test = pd.DataFrame(fold_1[1])
    test.rename(columns={0:'ICV',
                        1: 'Target',
                        2: 'drug_encoding',
                        3: 'target_encoding',
                        4: 'Label'},
                        inplace=True)
    return train, test

def fold_10_valid(fold_datase):
    for i in range(len(fold_datase)):
        print("the "+str(i)+" cross fold beginning!")
        train, tes = get_train_test(fold_datase[i])
        config = utils.generate_config(drug_encoding = "MPNN",
                         target_encoding = "CNN",
                         cls_hidden_dims = [1024,1024,512],
                         result_folder = "./rresult_" + str(i) + "/",
                         train_epoch = 60,
                         LR = 0.0001,
                         batch_size = 64,
                         hidden_dim_drug = 128,
                         mpnn_hidden_size = 128,
                         mpnn_depth = 3,
                         cnn_target_filters = [32,64,96],
                         cnn_target_kernels = [4,8,12]
                        )
        model = models.model_initialize(**config)
        model.train(train, tes)
        model.save_model("./rresult_" + str(i) + "/fold_" + str(i) + "_model")

def main():
    smiles = str(sys.argv[1])
    protein = str(sys.argv[2])
    DTI = str(sys.argv[3])
    dict_icv_drug_encoding = smiles_embed(smiles)
    dict_target_protein_encoding = protein_embed(protein)
    fold_dataset = sampling(DTI, dict_icv_drug_encoding, dict_target_protein_encoding)
    fold_10_valid(fold_dataset)

if __name__=="__main__":
    main()
```

（2）输出训练集损失和验证集损失进行绘图。

```python
# 修改DTI.py文件中DBTA类中的train函数。在合适位置增加：
output_loss = "epoch,train_loss,validate_loss,test_loss\n"
output_loss = output_loss + str(epo + 1) + "," + str(loss.cpu().detach().numpy())[:7] + ","
output_loss = output_loss + str(loss_val)[:7] + "\n"
pretty_file = os.path.join(self.result_folder, "all_loss.txt")
with open(pretty_file, 'w') as fp:
    fp.write(output_loss)
```

（3）输出验证集上每条数据的真实值和预测值，自行绘图作ROC、PR曲线，计算AUC等指标。

```python
# 修改DTI.py文件中DBTA类中的train函数。在合适位置增加：
self.model = model_max
if val is not None:
    output_label_pred_val = ""
    with torch.set_grad_enabled(False):
        if self.binary:
            auc, auprc, f1, loss_val, logits, y_la, y_pr = self.test_(validation_generator, self.model)
            for i in range(len(y_la)):
                output_label_pred_val = output_label_pred_val + str(y_la[i]) + "," + str(y_pr[i]) + "\n"
            pre_file = os.path.join(self.result_folder, "y_label_pred_val.txt")
            with open(pre_file, 'w') as fp:
                fp.write(output_label_pred_val)
```

（4）跑模型

```shell
python D-MPNN_T-CNN.py drug_smiles protein_fasta drug_protein
```

### 回归模型

（1）只需修改主体py文件，其他文件不需修改。创建主体文件D-MPNN_T-CNN-regression.py，用于将多个功能性py模块连接起来实现DTI活性预测。

```python
import sys
from collections import defaultdict
import pandas as pd
import numpy as np
from sklearn.model_selection import StratifiedKFold
from sklearn.preprocessing import LabelEncoder
from DeepPurpose import utils
from DeepPurpose import DTI as models

def smiles_embed(smiles):
    with open(smiles) as f:
        f1 = f.readlines()
    dict_icv_drug_encoding = {}
    for line in f1:
        dict_icv_drug_encoding[line.split(",")[0]] = utils.smiles2mpnnfeature(line.split(",")[1].strip())
    return dict_icv_drug_encoding

def protein_embed(protein):
    with open(protein) as f:
        f1 = f.readlines()
    dict_target_protein_encoding = {}
    for line in f1:
        dict_target_protein_encoding[line.split(",")[0]] = utils.trans_protein(line.split(",")[1].strip())
    return dict_target_protein_encoding

def sampling(DTI, dict_icv_drug_encoding, dict_target_protein_encoding):
    target_set = list(dict_target_protein_encoding.keys())
    with open(DTI) as f:
        f1 = f.readlines()
    dti_mutidict = defaultdict(list)
    [dti_mutidict[i.split(",")[0]].append(i.split(",",1)[1].strip()) for i in f1]
    whole_positive = []
    whole_negetive = []
    for key in dti_mutidict:
        for i in dti_mutidict[key]:
            whole_positive.append([key,i.split(",")[0],dict_icv_drug_encoding[key],dict_target_protein_encoding[i.split(",")[0]],float(2-np.log10(float(i.split(",")[1])))])
        target_no = target_set[:]
        [target_no.remove(i.split(",")[0]) for i in dti_mutidict[key]]
        for a in target_no:
            whole_negetive.append([key,a,dict_icv_drug_encoding[key],dict_target_protein_encoding[a],float(0)])
    #print("whole_positive: ",whole_positive[0])
    #print("whole_negetive: ",whole_negetive[0])
    whole_positive = np.array(whole_positive,dtype=object)
    whole_negetive = np.array(whole_negetive,dtype=object)
    whole_positive_muti_10 = np.tile(whole_positive,(10,1))
    #print(len(whole_positive),len(whole_positive_muti_10))
    #[print(i) for i in whole_positive_muti_10[:,0]]
    np.random.seed(10)
    whole_negetive_index = np.random.choice(np.arange(len(whole_negetive)), size=10 * len(whole_positive),replace=False)
    whole_negetive_1 = np.array([whole_negetive[i] for i in whole_negetive_index])
    # print(len(whole_positive_muti_10),len(whole_negetive_1))
    # print(whole_negetive_1[:,0])
    data_set = np.vstack((whole_positive_muti_10,whole_negetive_1))
    #print(len(data_set),data_set[0])

    #all_fold_dataset = []
    fold_dataset = []
    #print(data_set[:,0:4][0:2],data_set[:,4][0:2])
    #print(type(data_set[:, 4]))
    kf = StratifiedKFold(n_splits=10, shuffle=True, random_state=10)
    print((len(data_set)))
    #temp = np.array([[i,1] for i in range(len(data_set))])
    #print(len(temp))
    #label_encoder = LabelEncoder()
    y = LabelEncoder().fit_transform(data_set[:, 4])
    for train, test in kf.split(data_set[:, 0:4], y):
        train_data, test_data = data_set[train], data_set[test]
        one_fold_dataset = []
        one_fold_dataset.append(train_data)
        one_fold_dataset.append(test_data)
        fold_dataset.append(one_fold_dataset)
    # print(len(fold_dataset[0][0]))
    # print(len(fold_dataset[0][1]))
    # print((fold_dataset[0][0][0]))
    # output = pd.DataFrame(fold_dataset[0][0])
    # print(output)
    return fold_dataset

def get_train_test(fold_1):
    train = pd.DataFrame(fold_1[0])
    train.rename(columns={0:'ICV',
                        1: 'Target',
                        2: 'drug_encoding',
                        3: 'target_encoding',
                        4: 'Label'},
                        inplace=True)
    test = pd.DataFrame(fold_1[1])
    test.rename(columns={0:'ICV',
                        1: 'Target',
                        2: 'drug_encoding',
                        3: 'target_encoding',
                        4: 'Label'},
                        inplace=True)
    return train, test

def fold_10_valid(fold_datase):
    for i in range(len(fold_datase)):
        print("the "+str(i)+" cross fold beginning!")
        train, tes = get_train_test(fold_datase[i])
        config = utils.generate_config(drug_encoding = "MPNN",
                         target_encoding = "CNN",
                         cls_hidden_dims = [1024,1024,512],
                         result_folder = "./rresult_" + str(i) + "/",
                         train_epoch = 60,
                         LR = 0.0001,
                         batch_size = 64,
                         hidden_dim_drug = 128,
                         mpnn_hidden_size = 128,
                         mpnn_depth = 3,
                         cnn_target_filters = [32,64,96],
                         cnn_target_kernels = [4,8,12]
                        )
        model = models.model_initialize(**config)
        model.train(train, tes)
        model.save_model("./rresult_" + str(i) + "/fold_" + str(i) + "_model")

def main():
    smiles = str(sys.argv[1])
    protein = str(sys.argv[2])
    DTI = str(sys.argv[3])
    dict_icv_drug_encoding = smiles_embed(smiles)
    dict_target_protein_encoding = protein_embed(protein)
    fold_dataset = sampling(DTI, dict_icv_drug_encoding, dict_target_protein_encoding)
    fold_10_valid(fold_dataset)

if __name__=="__main__":
    main()    
```

（2）跑模型

```shell
python D-MPNN_T-CNN-regression.py drug_smiles protein_fasta drug_protein
```

---

## 基于所有数据训练最终模型

采用十折交叉验证法训练出来的所有模型都只用了十分之九的数据进行训练。这里最后基于所有数据再训练一个模型。

### 分类模型

（1）主要需要修改的是主题文件。

```python
import sys
from collections import defaultdict
import pandas as pd
import numpy as np
from sklearn.model_selection import StratifiedKFold
from sklearn.preprocessing import LabelEncoder
from DeepPurpose import utils
from DeepPurpose import DTI as models

def smiles_embed(smiles):
    with open(smiles) as f:
        f1 = f.readlines()
    dict_icv_drug_encoding = {}
    for line in f1:
        dict_icv_drug_encoding[line.split(",")[0]] = utils.smiles2mpnnfeature(line.split(",")[1].strip())
    return dict_icv_drug_encoding

def protein_embed(protein):
    with open(protein) as f:
        f1 = f.readlines()
    dict_target_protein_encoding = {}
    for line in f1:
        dict_target_protein_encoding[line.split(",")[0]] = utils.trans_protein(line.split(",")[1].strip())
    return dict_target_protein_encoding

def sampling(DTI, dict_icv_drug_encoding, dict_target_protein_encoding):
    target_set = list(dict_target_protein_encoding.keys())
    with open(DTI) as f:
        f1 = f.readlines()
    dti_mutidict = defaultdict(list)
    [dti_mutidict[i.split(",")[0]].append(i.split(",")[1].strip()) for i in f1]
    whole_positive = []
    whole_negetive = []
    for key in dti_mutidict:
        for i in dti_mutidict[key]:
            whole_positive.append([key,i,dict_icv_drug_encoding[key],dict_target_protein_encoding[i],1])
        target_no = target_set[:]
        [target_no.remove(i) for i in dti_mutidict[key]]
        for a in target_no:
            whole_negetive.append([key,a,dict_icv_drug_encoding[key],dict_target_protein_encoding[a],0])
    #print("whole_positive: ",whole_positive[0])
    #print("whole_negetive: ",whole_negetive[0])
    whole_positive = np.array(whole_positive,dtype=object)
    whole_negetive = np.array(whole_negetive,dtype=object)
    whole_positive_muti_10 = np.tile(whole_positive,(10,1))
    #print(len(whole_positive),len(whole_positive_muti_10))
    #[print(i) for i in whole_positive_muti_10[:,0]]
    np.random.seed(10)
    whole_negetive_index = np.random.choice(np.arange(len(whole_negetive)), size=10 * len(whole_positive),replace=False)
    whole_negetive_1 = np.array([whole_negetive[i] for i in whole_negetive_index])
    # print(len(whole_positive_muti_10),len(whole_negetive_1))
    # print(whole_negetive_1[:,0])
    data_set = np.vstack((whole_positive_muti_10,whole_negetive_1))
    random.shuffle(data_set)
    #print(len(data_set),data_set[0])

    #all_fold_dataset = []
    #fold_dataset = []
    #print(data_set[:,0:4][0:2],data_set[:,4][0:2])
    #print(type(data_set[:, 4]))
    #kf = StratifiedKFold(n_splits=10, shuffle=True, random_state=10)
    #print((len(data_set)))
    #temp = np.array([[i,1] for i in range(len(data_set))])
    #print(len(temp))
    #label_encoder = LabelEncoder()
    #y = LabelEncoder().fit_transform(data_set[:, 4])
    #for train, test in kf.split(data_set[:, 0:4], y):
    #    train_data, test_data = data_set[train], data_set[test]
    #    one_fold_dataset = []
    #    one_fold_dataset.append(train_data)
    #    one_fold_dataset.append(test_data)
    #    fold_dataset.append(one_fold_dataset)
    # print(len(fold_dataset[0][0]))
    # print(len(fold_dataset[0][1]))
    # print((fold_dataset[0][0][0]))
    # output = pd.DataFrame(fold_dataset[0][0])
    # print(output)
    return data_set

def get_train_test(fold_1):
    train = pd.DataFrame(fold_1)
    train.rename(columns={0:'ICV',
                        1: 'Target',
                        2: 'drug_encoding',
                        3: 'target_encoding',
                        4: 'Label'},
                        inplace=True)
    #test = pd.DataFrame(fold_1[1])
    #test.rename(columns={0:'ICV',
    #                    1: 'Target',
    #                    2: 'drug_encoding',
    #                    3: 'target_encoding',
    #                    4: 'Label'},
    #                    inplace=True)
    return train

def fold_10_valid(fold_datase):
    #for i in range(len(fold_datase)):
    #    print("the "+str(i)+" cross fold beginning!")
    train = get_train_test(fold_datase)
    #    ooout_test = out_test()
        # config = generate_config(cls_hidden_dims = [1024,1024,512],
        #                  result_folder = "./result_" + str(i) + "/",
        #                  train_epoch = 30,
        #                  LR = 0.0001,
        #                  batch_size = 128,
        #                  hidden_dim_drug = 128,
        #                  mpnn_hidden_size = 128,
        #                  mpnn_depth = 3,
        #                  cnn_target_filters = [32,64,96],
        #                  cnn_target_kernels = [4,8,12]
        #                 )
    config = utils.generate_config(drug_encoding = "MPNN",
                         target_encoding = "CNN",
                         cls_hidden_dims = [1024,1024,512],
                         result_folder = "./rresult_/",
                         train_epoch = 60,
                         LR = 0.0001,
                         batch_size = 128,
                         hidden_dim_drug = 128,
                         mpnn_hidden_size = 128,
                         mpnn_depth = 3,
                         cnn_target_filters = [32,64,96],
                         cnn_target_kernels = [4,8,12]
                        )
        #model = model_initialize(**config)
        #model = model.cuda()
    model = models.model_initialize(**config)
    #print(train)
    model.train(train)
    model.save_model("./rresult/fold_model")

def main():
    smiles = str(sys.argv[1])
    protein = str(sys.argv[2])
    DTI = str(sys.argv[3])
    dict_icv_drug_encoding = smiles_embed(smiles)
    dict_target_protein_encoding = protein_embed(protein)
    fold_dataset = sampling(DTI, dict_icv_drug_encoding, dict_target_protein_encoding)
    fold_10_valid(fold_dataset)

if __name__=="__main__":
    main()
```

### 回归模型

（1）主要需要修改的是主体文件。

```python
import sys
from collections import defaultdict
import pandas as pd
import numpy as np
from sklearn.model_selection import StratifiedKFold
from sklearn.preprocessing import LabelEncoder
from DeepPurpose import utils
from DeepPurpose import DTI as models

def smiles_embed(smiles):
    with open(smiles) as f:
        f1 = f.readlines()
    dict_icv_drug_encoding = {}
    for line in f1:
        dict_icv_drug_encoding[line.split(",")[0]] = utils.smiles2mpnnfeature(line.split(",")[1].strip())
    return dict_icv_drug_encoding

def protein_embed(protein):
    with open(protein) as f:
        f1 = f.readlines()
    dict_target_protein_encoding = {}
    for line in f1:
        dict_target_protein_encoding[line.split(",")[0]] = utils.trans_protein(line.split(",")[1].strip())
    return dict_target_protein_encoding

def sampling(DTI, dict_icv_drug_encoding, dict_target_protein_encoding):
    target_set = list(dict_target_protein_encoding.keys())
    with open(DTI) as f:
        f1 = f.readlines()
    dti_mutidict = defaultdict(list)
    [dti_mutidict[i.split(",")[0]].append(i.split(",",1)[1].strip()) for i in f1]
    whole_positive = []
    whole_negetive = []
    for key in dti_mutidict:
        for i in dti_mutidict[key]:
            whole_positive.append([key,i.split(",")[0],dict_icv_drug_encoding[key],dict_target_protein_encoding[i.split(",")[0]],float(2-np.log10(float(i.split(",")[1])))])
        target_no = target_set[:]
        [target_no.remove(i.split(",")[0]) for i in dti_mutidict[key]]
        for a in target_no:
            whole_negetive.append([key,a,dict_icv_drug_encoding[key],dict_target_protein_encoding[a],float(0)])
    #print("whole_positive: ",whole_positive[0])
    #print("whole_negetive: ",whole_negetive[0])
    whole_positive = np.array(whole_positive,dtype=object)
    whole_negetive = np.array(whole_negetive,dtype=object)
    whole_positive_muti_10 = np.tile(whole_positive,(10,1))
    #print(len(whole_positive),len(whole_positive_muti_10))
    #[print(i) for i in whole_positive_muti_10[:,0]]
    np.random.seed(10)
    whole_negetive_index = np.random.choice(np.arange(len(whole_negetive)), size=10 * len(whole_positive),replace=False)
    whole_negetive_1 = np.array([whole_negetive[i] for i in whole_negetive_index])
    # print(len(whole_positive_muti_10),len(whole_negetive_1))
    # print(whole_negetive_1[:,0])
    data_set = np.vstack((whole_positive_muti_10,whole_negetive_1))
    #print(len(data_set),data_set[0])

    #all_fold_dataset = []
    #fold_dataset = []
    #print(data_set[:,0:4][0:2],data_set[:,4][0:2])
    #print(type(data_set[:, 4]))
    #kf = StratifiedKFold(n_splits=10, shuffle=True, random_state=10)
    #print((len(data_set)))
    #temp = np.array([[i,1] for i in range(len(data_set))])
    #print(len(temp))
    #label_encoder = LabelEncoder()
    #y = LabelEncoder().fit_transform(data_set[:, 4])
    #for train, test in kf.split(data_set[:, 0:4], y):
    #    train_data, test_data = data_set[train], data_set[test]
    #    one_fold_dataset = []
    #    one_fold_dataset.append(train_data)
    #    one_fold_dataset.append(test_data)
    #    fold_dataset.append(one_fold_dataset)
    # print(len(fold_dataset[0][0]))
    # print(len(fold_dataset[0][1]))
    # print((fold_dataset[0][0][0]))
    # output = pd.DataFrame(fold_dataset[0][0])
    # print(output)
    return data_set

def get_train_test(fold_1):
    train = pd.DataFrame(fold_1)
    train.rename(columns={0:'ICV',
                        1: 'Target',
                        2: 'drug_encoding',
                        3: 'target_encoding',
                        4: 'Label'},
                        inplace=True)
    #test = pd.DataFrame(fold_1[1])
    #test.rename(columns={0:'ICV',
    #                    1: 'Target',
    #                    2: 'drug_encoding',
    #                    3: 'target_encoding',
    #                    4: 'Label'},
    #                    inplace=True)
    return train

def fold_10_valid(fold_datase):
    #for i in range(len(fold_datase)):
    #    print("the "+str(i)+" cross fold beginning!")
    train = get_train_test(fold_datase)
    #    ooout_test = out_test()
        # config = generate_config(cls_hidden_dims = [1024,1024,512],
        #                  result_folder = "./result_" + str(i) + "/",
        #                  train_epoch = 30,
        #                  LR = 0.0001,
        #                  batch_size = 128,
        #                  hidden_dim_drug = 128,
        #                  mpnn_hidden_size = 128,
        #                  mpnn_depth = 3,
        #                  cnn_target_filters = [32,64,96],
        #                  cnn_target_kernels = [4,8,12]
        #                 )
    config = utils.generate_config(drug_encoding = "MPNN",
                         target_encoding = "CNN",
                         cls_hidden_dims = [1024,1024,512],
                         result_folder = "./rresult_/",
                         train_epoch = 60,
                         LR = 0.0001,
                         batch_size = 128,
                         hidden_dim_drug = 128,
                         mpnn_hidden_size = 128,
                         mpnn_depth = 3,
                         cnn_target_filters = [32,64,96],
                         cnn_target_kernels = [4,8,12]
                        )
        #model = model_initialize(**config)
        #model = model.cuda()
    model = models.model_initialize(**config)
    #print(train)
    model.train(train)
    model.save_model("./rresult/fold_model")

def main():
    smiles = str(sys.argv[1])
    protein = str(sys.argv[2])
    DTI = str(sys.argv[3])
    dict_icv_drug_encoding = smiles_embed(smiles)
    dict_target_protein_encoding = protein_embed(protein)
    fold_dataset = sampling(DTI, dict_icv_drug_encoding, dict_target_protein_encoding)
    fold_10_valid(fold_dataset)

if __name__=="__main__":
    main()
```

---

## 测试模型

测试模型分为两种方式。一种是在训练模型的时候直接将测试集加到model函数中进行计算。另一种是在模型训练完成之后，使用训练好的模型对测试集进行测试。建议使用第二种。遵循测试集只能使用一次的原则。

### 方式1

修改主体文件如下：

```python
def val_encode(num):
    smi_val = []
    fas_val = []
    la_val = []
    with open("/home/databank/yls/DeepPurpose_DeepPurpose_outtest_2/dataset/fold/fold_" + str(num) + "/val.txt") as f:
        f1 = f.readlines()
    for line in f1:
        smi_val.append(models.drug_dict[line.split(",")[0]])
        fas_val.append(models.protein_dict[line.split(",")[1]])
        la_val.append(float(line.split(",")[2]))
    df_data = pd.DataFrame(zip(smi_val, fas_val, la_val))
    df_data.rename(columns={0:'SMILES',
                                1: 'Target Sequence',
                                2: 'Label'},
                                inplace=True)
    df_data = encode_drug(df_data, "MPNN")
    df_data = encode_protein(df_data, "CNN")
    return df_data

def o_test(num):
    smi_val = []
    fas_val = []
    la_val = []
    with open("/home/databank/yls/DeepPurpose_DeepPurpose_outtest_2/out_test/test_" + str(num) + "/test.txt") as f:
        f1 = f.readlines()
    for line in f1:
        smi_val.append(models.drug_dict[line.split(",")[0]])
        fas_val.append(models.protein_dict[line.split(",")[1]])
        la_val.append(float(line.split(",")[2]))
    df_data = pd.DataFrame(zip(smi_val, fas_val, la_val))
    df_data.rename(columns={0:'SMILES',
                                1: 'Target Sequence',
                                2: 'Label'},
                                inplace=True)
    df_data = encode_drug(df_data, "MPNN")
    df_data = encode_protein(df_data, "CNN")
    return df_data

model = models.model_initialize(**config)
model.train(i, val, outtest)
```

### 方式2

```python
import sys, os
import numpy as np
import DeepPurpose.DTI as models

def read_target(d_t, smiles, fasta):
    path = ""
    icv_smiles = {}
    tar_fasta = {}
    drug_name = []
    tar_name = []
    smi = []
    fas = []
    with open(smiles) as f:
        f1 = f.readlines()
    for i in f1:
        icv_smiles[i.strip().split(",")[0]] = i.strip().split(",")[1]
    with open(fasta) as f:
        f1 = f.readlines()
    for i in f1:
        tar_fasta[i.strip().split(",")[0]] = i.strip().split(",")[1]
    with open(d_t) as f:
        f1 = f.readlines()
    for i in f1:
        drug_name.append(i.split(",")[0])
        tar_name.append(i.split(",")[1].strip())
        smi.append(icv_smiles[i.split(",")[0]])
        fas.append(tar_fasta[i.split(",")[1].strip()])
    # for line in f1:
    #     line = line.strip().split(",")
    #     tar_name.append(line[0])
    #     tar_fasta.append(line[1])
    return drug_name, tar_name, smi, fas

def target_predict(i, drug_name, target_name, smiles, fasta):
    # drug_name = ["drug"]
    # target_name = ["Target"]
    # drug_name = np.tile(drug_name, (len(target_name), ))
    #command = "obabel -:'%s' -ocan -O mol.smi"%(smiles)
    #os.system(command)
    #with open("mol.smi") as f:
    #    smiles = f.readline().strip()
    # smiles = [smiles]
    # smiles = np.tile(smiles, (len(target_name), ))
    model = models.model_pretrained(path_dir = '/home/yqyang/yqyang/D3AI-CoV/Deep/logistic/DeepPurpose_DeepPurpose_outtest_4_xu/rresult_' + str(i) + '/fold_'+str(i)+'_model')
    models.virtual_screening(smiles, fasta,
                     model, drug_names = drug_name, target_names = target_name, result_folder = "./rresult_"+str(i)+"/",
                     convert_y = False)

def main():
    smiles = str(sys.argv[1])
    fasta = str(sys.argv[2])
    d_t = str(sys.argv[3])
    nu = str(sys.argv[4])
    drug_name, target_name, smiles, fasta = read_target(d_t, smiles, fasta)
    target_predict(nu, drug_name, target_name, smiles, fasta)

if __name__=="__main__":
    main()
```

---

## 网站接口

### 靶标预测

#### 用户上传一个小分子进行靶标预测

```python
import sys, os
import numpy as np
import DeepPurpose.DTI_1 as models

def read_target():
    path = ""
    tar_name = []
    tar_fasta = []
    with open("/home/yqyang/D3Deep/target_predict_and_vs/target_predict/in/protein_name-fasta.txt") as f:
        f1 = f.readlines()
    for line in f1:
        line = line.strip().split(",")
        tar_name.append(line[1])
        tar_fasta.append(line[2])
    return tar_name, tar_fasta

def transmol_linux(in_name,out_name):
    in_suffix=os.path.splitext(in_name)[1][1:]
    out_suffix=os.path.splitext(out_name)[1][1:]
    if out_suffix == "smi":
        command="obabel -i%s %s -ocan -O %s"%(in_suffix,in_name,out_name)
    else:
        command="obabel -i%s %s -o%s -O %s"%(in_suffix,in_name,out_suffix,out_name)
    os.system(command)

def target_predict(smiles, mol_name, target_name, target_fasta):
    drug_name = [mol_name]
    drug_name = np.tile(drug_name, (len(target_name), ))
    #command = "obabel -:'%s' -ocan -O mol.smi"%(smiles)
    #os.system(command)
    #with open("mol.smi") as f:
    #    smiles = f.readline().strip()
    smiles = [smiles]
    smiles = np.tile(smiles, (len(target_name), ))
    model = models.model_pretrained(path_dir = '/home/yqyang/D3Deep/model/fold_1_model_log_final')
    os.system("mkdir result_7")
    models.virtual_screening(smiles, target_fasta,
                     model, drug_names = drug_name, target_names = target_name, result_folder = "./result_7/",
                     convert_y = False)

def main():
    mol_file = str(sys.argv[1])
    mol_name = os.path.splitext(mol_file)[0]
    transmol_linux(mol_file, mol_name + ".smi")
    with open(mol_name + ".smi") as f:
        smiles = f.readline().strip().split("\t")[0]
    target_name, target_fasta = read_target()
    target_predict(smiles, mol_name, target_name, target_fasta)

if __name__=="__main__":
    main()
```

#### 用户上传一个小分子和一条序列进行靶标预测

```python
import sys, os
import numpy as np
import DeepPurpose.DTI as models

# def read_target():
#     path = ""
#     tar_name = []
#     tar_fasta = []
#     with open("protein_name-fasta.txt") as f:
#         f1 = f.readlines()
#     for line in f1:
#         line = line.strip().split(",")
#         tar_name.append(line[0])
#         tar_fasta.append(line[1])
#     return tar_name, tar_fasta

def target_predict(smiles, fasta):
    drug_name = ["drug"]
    target_name = ["Target"]
    # drug_name = np.tile(drug_name, (len(target_name), ))
    #command = "obabel -:'%s' -ocan -O mol.smi"%(smiles)
    #os.system(command)
    #with open("mol.smi") as f:
    #    smiles = f.readline().strip()
    smiles = [smiles]
    # smiles = np.tile(smiles, (len(target_name), ))
    model = models.model_pretrained(path_dir = '/home/yqyang/D3Deep/model/fold_7_model_logistic')
    models.virtual_screening(smiles, fasta,
                     model, drug_names = drug_name, target_names = target_name, result_folder = "./result_7/",
                     convert_y = False)

def main():
    smiles = sys.argv[1]
    fasta = sys.argv[2]
    # target_name, target_fasta = read_target()
    target_predict(smiles, fasta)

if __name__=="__main__":
    main()
```

### 虚拟筛选

#### 用户上传配体库，对数据库中的所有靶标预测一遍

```python
import sys, os, re
import numpy as np
import DeepPurpose.DTI_1 as models

#os.environ["CUDA_VISIBLE_DEVICES"] = '1'

def read_target():
    path = ""
    tar_name = []
    tar_fasta = []
    with open("/home/yqyang/D3Deep/target_predict_and_vs/virtual_screening/regression/virtual_screening_in/protein_name-fasta.txt") as f:
        f1 = f.readlines()
    for line in f1:
        line = line.strip().split(",")
        tar_name.append(line[1])
        tar_fasta.append(line[2])
    return tar_name, tar_fasta

def read_drug(file):
    smiles = []
    names = []
    with open(file) as f:
        f1 = f.readlines()
    for line in f1:
        names.append(line.strip().split("\t")[1])
        smiles.append(line.strip().split("\t")[0])
    return smiles, names

def transmol_linux(in_name,out_name):
    in_suffix=os.path.splitext(in_name)[1][1:]
    out_suffix=os.path.splitext(out_name)[1][1:]
    if out_suffix == "smi":
        command="obabel -i%s %s -ocan -O %s"%(in_suffix,in_name,out_name)
    else:
        command="obabel -i%s %s -o%s -O %s"%(in_suffix,in_name,out_suffix,out_name)
    os.system(command)

def virtual_screen_1_target(smiles, drug_names, target_name, target_fasta, result_folder_ = "./result_test/"):
    #smis = []
    # drug_name = ["drug"]
    # drug_name = np.tile(drug_name, (len(target_name), ))
    # for i in range(len(smiles)):
        # command = "obabel -:'%s' -ocan -O mol_%d.smi"%(smiles[i], i)
    # command = "obabel -:%s -ocan -O mol.smi"%(smiles)
        # os.system(command)
        # with open("mol_%d.smi"%(i)) as f:
            # smi = f.readline().strip()
            # smis.append(smi)
    # print(smiles)
    # smiles = np.tile(smiles, (len(target_name), ))
    target_name = np.tile(target_name, (len(smiles), ))
    target_fasta = np.tile(target_fasta, (len(smiles), ))
    model = models.model_pretrained(path_dir = '/home/yqyang/D3Deep/model/fold_all_model_all')
    models.virtual_screening(smiles, target_fasta,
                     model, drug_names = drug_names, target_names = target_name, result_folder = result_folder_,
                     convert_y = False)
    # for i in range(len(smiles)):
        # os.move("./mol_%d.smi"%(i))

def virtual_screen_all_target(smiles, drug_names, target_names, target_fastas):
    for pro in range(len(target_names)):
        os.system("mkdir \"./result_"+ target_names[pro]+"\"");
        virtual_screen_1_target(smiles, drug_names, target_names[pro], target_fastas[pro], result_folder_ = "./result_" + target_names[pro] + "/")

def main():
    molfile_all=str(sys.argv[1])
    mol_name = os.path.splitext(molfile_all)[0]
    transmol_linux(molfile_all, mol_name + ".smi")
    smiles, drug_names = read_drug(mol_name + ".smi")
    # print(mols)
    target_names, target_fastas = read_target()
    # virtual_screen_1_target(mols, target_name, target_fasta)
    virtual_screen_all_target(smiles, drug_names, target_names, target_fastas)

if __name__=="__main__":
    main()
```

#### 用户上传配体库，选择数据库中的一个靶标进行虚拟筛选

```python
import sys, os, re
import numpy as np
import DeepPurpose.DTI as models

def read_target():
    path = ""
    tar_name = []
    tar_fasta = []
    with open("protein_name-fasta.txt") as f:
        f1 = f.readlines()
    nu = 1
    dic = {}
    for line in f1:
        #line = line.strip().split(",")
        #tar_name.append(line[0])
        #tar_fasta.append(line[1])
        dic[str(nu)] = line.strip()
        nu += 1
    return dic

def read_drug(file):
    smiles = []
    names = []
    with open(file) as f:
        f1 = f.readlines()
    for line in f1:
        names.append(line.strip().split("\t")[1])
        smiles.append(line.strip().split("\t")[0])
    return smiles, names

def transmol_linux(in_name,out_name):
    in_suffix=os.path.splitext(in_name)[1][1:]
    out_suffix=os.path.splitext(out_name)[1][1:]
    if out_suffix == "smi":
        command="obabel -i%s %s -ocan -O %s"%(in_suffix,in_name,out_name)
    else:
        command="obabel -i%s %s -o%s -O %s"%(in_suffix,in_name,out_suffix,out_name)
    os.system(command)

def virtual_screen_1_target(smiles, drug_names, target_name, target_fasta, result_folder_ = "./result_test/"):
    #smis = []
    # drug_name = ["drug"]
    # drug_name = np.tile(drug_name, (len(target_name), ))
    # for i in range(len(smiles)):
        # command = "obabel -:'%s' -ocan -O mol_%d.smi"%(smiles[i], i)
    # command = "obabel -:%s -ocan -O mol.smi"%(smiles)
        # os.system(command)
        # with open("mol_%d.smi"%(i)) as f:
            # smi = f.readline().strip()
            # smis.append(smi)
    # print(smiles)
    # smiles = np.tile(smiles, (len(target_name), ))
    target_name = np.tile(target_name, (len(smiles), ))
    target_fasta = np.tile(target_fasta, (len(smiles), ))
    model = models.model_pretrained(path_dir = '/home/yqyang/D3Deep/model/fold_0_model_regression_out')
    models.virtual_screening(smiles, target_fasta,
                     model, drug_names = drug_names, target_names = target_name, result_folder = result_folder_,
                     convert_y = False)
    # for i in range(len(smiles)):
        # os.move("./mol_%d.smi"%(i))

def virtual_screen_all_target(smiles, drug_names, target_names, target_fastas):
    #for pro in range(len(target_names)):
    virtual_screen_1_target(smiles, drug_names, target_names, target_fastas, result_folder_ = "./result_" + target_names + "/")

def main():
    molfile_all=str(sys.argv[1])
    nu = str(sys.argv[2])
    mol_name = os.path.splitext(molfile_all)[0]
    transmol_linux(molfile_all, mol_name + ".smi")
    smiles, drug_names = read_drug(mol_name + ".smi")
    # print(mols)
    dic = read_target()
    # virtual_screen_1_target(mols, target_name, target_fasta)
    virtual_screen_all_target(smiles, drug_names, dic[nu].split(",")[0], dic[nu].split(",")[1])

if __name__=="__main__":
    main()
```

#### 用户上传配体库并且上传一个靶标序列进行虚拟筛选

```python
import sys, os
import numpy as np
import DeepPurpose.DTI as models

# def read_target():
#     path = ""
#     tar_name = []
#     tar_fasta = []
#     with open("protein_name-fasta.txt") as f:
#         f1 = f.readlines()
#     for line in f1:
#         line = line.strip().split(",")
#         tar_name.append(line[0])
#         tar_fasta.append(line[1])
#     return tar_name, tar_fasta

def read_drug(file):
    smiles = []
    with open(file) as f:
        f1 = f.readlines()
    for line in f1:
        smiles.append(line.strip())
    return smiles

# def virtual_screen_1_target(smiles, target_name, target_fasta, result_folder_ = "./result_test/"):
def virtual_screen_1_target(smiles, target):
    # smis = []
    # drug_name = ["drug"]
    # drug_name = np.tile(drug_name, (len(target_name), ))
    # for i in range(len(smiles)):
        # command = "obabel -:'%s' -ocan -O mol_%d.smi"%(smiles[i], i)
    # command = "obabel -:%s -ocan -O mol.smi"%(smiles)
        # os.system(command)
        # with open("mol_%d.smi"%(i)) as f:
            # smi = f.readline().strip()
            # smis.append(smi)
    # print(smiles)
    # smiles = np.tile(smiles, (len(target_name), ))
    target_name = ["Target"]
    target_name = np.tile(target_name, (len(smiles), ))
    target_fasta = np.tile(target, (len(smiles), ))
    model = models.model_pretrained(path_dir = '/home/databank/D3Deep/model/fold_4_model_regression')
    models.virtual_screening(smiles, target_fasta,
                     model, drug_names = smiles, target_names = target_name, result_folder = "./result_4/",
                     convert_y = False)
    # for i in range(len(smiles)):
        # os.move("./mol_%d.smi"%(i))

# def virtual_screen_all_target(mols, target_names, target_fastas):
#     for pro in range(len(target_names)):
#         virtual_screen_1_target(mols, target_names[pro], target_fastas[pro], result_folder_ = "./result_" + target_names[pro] + "/")

def main():
    file = sys.argv[1]
    target = sys.argv[2]
    mols = read_drug(file)
    # print(mols)
    # target_names, target_fastas = read_target()
    # virtual_screen_1_target(mols, target_name, target_fasta)
    virtual_screen_1_target(mols, target)

if __name__=="__main__":
    main()
```

---

## 数据分析

### 靶标预测和虚拟筛选

（1）获取每折中验证集上最好的epoch模型的shell脚本（靶标预测和虚拟筛选差不多，稍作修改即可）

```shell
echo "fold,max(auc),aupr" >> fold_final_.txt
for i in `seq 0 9`
do
  cd rresult_${i}
  grep epoch valid_markdowntable.txt|grep -v "#" > temp
  auc_max=`awk -F "|" '{print $3}' temp |sort -g|tail -1`
  grep ${auc_max} temp > temp1
  num=`wc -l temp1|awk '{print $1}'`
  if [ `echo "$num > 1.0" |bc` -eq 1 ];then aupr=`awk -F "|" '{print $4}' temp1 |sort -g|tail -1`;else aupr=`awk -F "|" '{print $4}' temp1`;fi
  echo -e "fold_${i},\c" >> ../fold_final_.txt
  echo -e "${auc_max},\c" >> ../fold_final_.txt
  echo "${aupr}" >> ../fold_final_.txt
  cd ..
done
```

（2）绘制loss曲线

参考：[Python绘图|Python绘制多曲线图](./Python绘图Python绘制多曲线图.md)

（3）绘制ROC和PR曲线

参考：[Python绘图|Python绘制ROC曲线和PR曲线](./Python绘图Python绘制ROC曲线和PR曲线.md)

（4）计算AUC、AUPRC、Acc、Pre、F1等指标

参考：

## 其他

### 续跑

续跑很简单，先初始化模型，再加载模型参数，再跑即可。

```python
model = models.model_initialize(**config)
model.load_pretrained('/home/yqyang/yqyang/DDB/MPNN-CNN_regression_3/rresult_0/fold_0_model/model.pt')
model.train(i, val)
```

### 多GPU训练、预测

DeepPurpose默认用机器上的所有GPU。

### CPU预测

在预测时重新定义一个device，将预测的数据和模型都放到该device上即可。

```python
self.device_1 = torch.device('cpu')
v_d = v_d.float().to(self.device_1)
v_p = v_p.to(self.device_1)
self.model = self.model.to(self.device_1)
score = self.model(v_d, v_p)
```

### 保存所有epoch模型

修改DTI.py文件，增加：

```python
self.save_model(self.result_folder + "epoch_" + str(epo+1))
```

### 最终预测模型选择

（1）挑取十折交叉验证中AUC最高的作为最终预测模型。

（2）基于所有数据训练一个最终模型作为最后预测模型。

（3）用多个模型进行预测，取预测值的平均值。

---

## 一些bug

### 后台提交，关闭窗口死掉问题

该问题主要是因为DeepPurpose在训练的时候会自动绘图，窗口关闭，绘图界面弹不出来，就会死掉。将绘图的部分注释掉即可。

