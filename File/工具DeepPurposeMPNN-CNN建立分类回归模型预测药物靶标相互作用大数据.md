👏 工具|DeepPurpose|MPNN-CNN建立分类回归模型预测药物靶标相互作用大数据

---

[封面PPT](../PPT/)

---

[TOC]

---

## 简介

大数据模型和小数据基本一样，不过对于小数据的模型是先将所有数据编码后的表示存储起来。然后再进行用一点取一点的训练。但是数据太多的话内存不足以存储所有数据的编码表示，往往会导致内存爆掉。解决思路就是先将数据存储成文本，训练一点编码一点。

---

## 大数据处理

### 分类模型

（1）分训练集和外部测试集

（2）将训练集再次分成5折（训练+验证）

```python
import os
import sys
from collections import defaultdict
import numpy as np
from sklearn.model_selection import StratifiedKFold
from sklearn.preprocessing import LabelEncoder
import random

def muti_icv_protein():
    target_set = []
    with open("/home/databank/yls/DeepPurpose_DeepPurpose_outtest_2/dataset/drug_protein_rest.txt") as f:
        f1 = f.readlines()
    for line in f1:
        target_set.append(line.split(",")[1].strip())
    target_set = set(target_set)
    #print(target_set)
    dti_mutidict = defaultdict(list)
    [dti_mutidict[i.split(",")[0]].append(i.split(",")[1].strip()) for i in f1]
    #print(dti_mutidict)
    whole_positive = []
    whole_negetive = []
    for key in dti_mutidict:
        #print(key)
        num = 0
        for i in dti_mutidict[key]:
            num += 1
            # whole_positive.append([key,i,dict_icv_drug_encoding[key],dict_target_protein_encoding[i],1])
            whole_positive.append([key,i,1])
        target_no = list(target_set)[:]
        [target_no.remove(z) for z in dti_mutidict[key]]
        suiji = np.random.randint(1, len(target_no), size = num)
        for a in suiji:
            #print(a)
            # whole_negetive.append([key,a,dict_icv_drug_encoding[key],dict_target_protein_encoding[a],0])
            whole_negetive.append([key,target_no[a],0])
    whole_positive = np.array(whole_positive,dtype=object)
    whole_negetive = np.array(whole_negetive,dtype=object)
    data_set = np.vstack((whole_positive,whole_negetive))
    kf = StratifiedKFold(n_splits=100, shuffle=True, random_state=10)
    y = LabelEncoder().fit_transform(data_set[:, 2])
    flag = 0
    for train, test in kf.split(data_set[:, 0:2], y):
        train_data, test_data = data_set[train], data_set[test]
        if not os.path.exists('./fold_' + str(flag)):
            os.makedirs('./fold_' + str(flag))
        with open('./fold_' + str(flag) + "/train.txt", "w") as rt:
            li = list(range(len(train_data)))
            random.shuffle(li)
            for tmp in li:
                rt.writelines([str(line)+',' for line in train_data[tmp]])
                rt.write("\n")
        with open('./fold_' + str(flag) + "/val.txt", "w") as rt:
            li = list(range(len(test_data)))
            random.shuffle(li)
            for tmp in li:
                rt.writelines([str(line)+',' for line in test_data[tmp]])
                rt.write("\n")
        # one_fold_dataset = []
        # one_fold_dataset.append(train_data)
        # one_fold_dataset.append(test_data)
        # fold_dataset.append(one_fold_dataset)
        flag += 1
        if flag > 5:
            break

def main():
    muti_icv_protein()

if __name__ == '__main__':
    main()
```

### 回归模型

（1）将数据分成训练集和验证集

```python
import sys
import numpy as np
from numpy import random as nr

def pick_random(file):
    with open(file) as f:
        f1 = f.readlines()
    num = int(len(f1)/100)
    nr.seed(666)
    a = np.arange(0,len(f1),dtype=int)
    b1 = nr.choice(a,size=num,replace=False)
    c = np.array(list(set(a)-set(b1)))
    train = open("train.txt", "w")
    val = open("val.txt","w")
    for i in c:
        train.write(f1[i])
    for m in b1:
        val.write(f1[m])

def main():
    file = sys.argv[1]
    pick_random(file)

if __name__ == '__main__':
    main()
```

（2）随机打乱

```python
import sys
import numpy as np
from numpy import random as nr

def ran(file):
    with open(file) as f:
        f1 = f.readlines()
    num = len(f1)
    nr.seed(666)
    a = np.arange(0,num,dtype=int)
    b1 = nr.choice(a,size=num,replace=False)
    out = open("val.txt","w")
    for m in b1:
        out.write(f1[m])

def main():
    file = sys.argv[1]
    ran(file)

if __name__ == '__main__':
    main()
```

---

## 训练模型

分类模型和回归模型同样

（1）修改主体文件：

```python
def fold_10_valid():
    for i in range(1):
        print("the "+str(i)+" cross fold beginning!")
        val = val_encode(i)
        #outtest = o_test(0)
        # train, tes = get_train_test(fold_datase[i])
        # ooout_test = out_test()
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
                         result_folder = "./rresult_" + str(i) + "/",
                         train_epoch = 250,
                         LR = 0.0001,
                         batch_size = 128,
                         hidden_dim_drug = 128,
                         mpnn_hidden_size = 128,
                         mpnn_depth = 3,
                         cnn_target_filters = [32,64,96],
                         cnn_target_kernels = [4,8,12]

        #model = model_initialize(**config)
        #model = model.cuda()
        model = models.model_initialize(**config)
        model.train(i, val)
        model.save_model("./rresult_" + str(i) + "/fold_" + str(i) + "_model")
                                       
def main():
    #smiles = str(sys.argv[1])
    #protein = str(sys.argv[2])
    #protein = str(sys.argv[1])
    #DTI = str(sys.argv[3])
    # print("drug start!")
    # dict_icv_drug_encoding = smiles_embed(smiles)
    # print("drug finshed!")
    # print("protein start!")
    # dict_target_protein_encoding = protein_embed(protein)
    # print("protein finshed!")
    # fold_dataset = sampling(DTI, dict_icv_drug_encoding, dict_target_protein_encoding)
    #fold_dataset = sampling(DTI)
    fold_10_valid()

if __name__=="__main__":
    main()                                       
```

（2）修改DTI.py文件

```python
drug_dict = {}
protein_dict = {}
with open("/home/yqyang/yqyang/DDB/MPNN-CNN_regression/dataset/drug_smiles.txt") as f:
    f1 = f.readlines()
for line in f1:
    drug_dict[line.split(",")[0]] = line.split(",")[1].strip()
del f1
with open("/home/yqyang/yqyang/DDB/MPNN-CNN_regression/dataset/protein_fasta.txt") as f:
    f1 = f.readlines()
for line in f1:
    protein_dict[line.split(",")[0]] = line.split(",")[1].strip()
del f1

def train(self, num, val = None, test = None, verbose = True):
    # training_generator = data.DataLoader(data_process_loader(train.index.values, train.Label.values, train, **self.config), **params)
    for epo in range(train_epoch):
        with open("/home/yqyang/yqyang/DDB/MPNN-CNN_regression/dataset/fold_" + str(num) + "/train.txt") as f:
            f1 = f.readlines()
                for zzz in range(len(f1)//128 + 1):
                    smiles = []
                    fasta = []
                    label = []
                    if zzz == len(f1)//128:
                        for aaa in f1[-128: -1]:
                            smiles.append(drug_dict[aaa.split(",")[0]])
                            fasta.append(protein_dict[aaa.split(",")[1]])
                            label.append(float(aaa.split(",")[2]))
                    else:
                        for aaa in f1[zzz*128: (zzz+1)*128]:
                            smiles.append(drug_dict[aaa.split(",")[0]])
                            fasta.append(protein_dict[aaa.split(",")[1]])
                            label.append(float(aaa.split(",")[2]))
                    df_data = pd.DataFrame(zip(smiles, fasta, label))
                    df_data.rename(columns={0:'SMILES',
                    1: 'Target Sequence',
                    2: 'Label'},
                    inplace=True)
                    df_data = encode_drug(df_data, "MPNN")
                    df_data = encode_protein(df_data, "CNN")
                    training_generator = data.DataLoader(data_process_loader(df_data.index.values, df_data.Label.values, df_data, **self.config), **params)
                    for i, (v_d, v_p, label) in enumerate(training_generator):
                    # (v_d, v_p, label) = enumerate(training_generator)
                    # v_d = training_generator[0][0]
                    # v_p = training_generator[0][1]
                    # label = training_generator[0][2]
                        #print(label)
                        if self.target_encoding == 'Transformer':
                            v_p = v_p
                        else:
                            v_p = v_p.float().to(self.device)
                        if self.drug_encoding == "MPNN" or self.drug_encoding == 'Transformer':
                            v_d = v_d
                        else:
                            v_d = v_d.float().to(self.device)
                            #score = self.model(v_d, v_p.float().to(self.device))

                        #print(v_d)
                        #print(v_p)
                        score = self.model(v_d, v_p)
                        #print(score)
                        label = Variable(torch.from_numpy(np.array(label)).float()).to(self.device)
                        #print(label)

                        if self.binary:
                            loss_fct = torch.nn.BCELoss()
                            m = torch.nn.Sigmoid()
                            n = torch.squeeze(m(score), 1)

                            #n.data = torch.where(torch.isnan(n), torch.full_like(n, 0.3), n)
                            #n.data = torch.where(torch.isinf(n), torch.full_like(n, 1), n)
                            #print("n:", n)
                            loss = loss_fct(n, label)
                        else:
                            loss_fct = torch.nn.MSELoss()
                            n = torch.squeeze(score, 1)
                            loss = loss_fct(n, label)
                        #print(loss)
                        loss_history.append(loss.item())
                        writer.add_scalar("Loss/train", loss.item(), iteration_loss)
                        iteration_loss += 1

                        opt.zero_grad()
                        #print("loss:", loss)
                        loss.backward()
                        #torch.nn.utils.clip_grad_norm(self.model.parameters(), 5)
                        #print(self.model)
                        opt.step()
```

