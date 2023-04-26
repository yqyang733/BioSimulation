👏 工具|MultiDTI|建立分类模型预测药物靶标相互作用(大数据)

---

[封面PPT](../PPT/工具MultiDTI建立分类模型预测药物靶标相互作用(大数据).pptx)

---

[TOC]

---

## 大数据处理

和小数据基本一致，区别在于matrix.py脚本。

```python
import sys
import numpy as np

def pre_process(inpt_1):
    with open(inpt_1) as f:
        f1 = f.readlines()
    lst_total = []
    lst_icv = []
    lst_uniprot = []
    for line in f1:
        icv = line.strip().split(",")[0]
        uniprot = line.strip().split(",")[1]
        lst_total.append([icv,uniprot])
        lst_icv.append(icv)
        lst_uniprot.append(uniprot)
    return lst_total, lst_icv, lst_uniprot

def network(lst_total, lst_icv, lst_uniprot):
    lst_icv_unique = list(set(lst_icv))
    dic_icv_unique = dict(zip(lst_icv_unique,range(len(lst_icv_unique))))
    lst_uniprot_unique = list(set(lst_uniprot))
    dic_uniprot_unique = dict(zip(lst_uniprot_unique,range(len(lst_uniprot_unique))))
    matrix_1 = np.zeros((len(lst_icv_unique),len(lst_uniprot_unique)))
    # for i in range(len(lst_icv_unique)):
    #     for j in range(len(lst_uniprot_unique)):
    #         if [lst_icv_unique[i],lst_uniprot_unique[j]] in lst_total:
    #             matrix_1[i][j] += 1
    b = 0
    for aa in lst_total:
        print(aa)
        b += 1
        print(b)
        matrix_1[dic_icv_unique[aa[0]]][dic_uniprot_unique[aa[1]]] += 1

    f_icv=open("result_icv.txt","w")
    f_uniprot=open("result_pro.txt","w")
    f_matrix=open("result_matrix.txt","w")
    #output = ""
    #output += "network"
    f_matrix.write("network")
    #output_icv = ""
    #output_pro = ""
    for a in lst_uniprot_unique:
        #output += ","
        #f_matrix.write(",")
        #output += a
        f_matrix.write("," + a)
        #output_pro = output_pro + a + "\n"
        f_uniprot.write(a + "\n")
    #output += "\n"
    f_matrix.write("\n")
    for b in range(len(lst_icv_unique)):
        #output_icv = output_icv + lst_icv_unique[b] + "\n"
        f_icv.write(lst_icv_unique[b] + "\n")
        #output += lst_icv_unique[b]
        f_matrix.write(lst_icv_unique[b])
        for c in matrix_1[b]:
            #output += ","
            #output += str(c)
            f_matrix.write(","+str(c))
        #output += "\n"
        f_matrix.write("\n")
    #with open("result_matrix.txt","w") as rt, open("result_icv.txt","w") as rt_icv, open("result_pro.txt","w") as rt_pro:
        #rt.write(output)
        #rt_icv.write(output_icv)
        #rt_pro.write(output_pro)

def main():
    inpt_1 = str(sys.argv[1])
    lst_total, lst_icv, lst_uniprot = pre_process(inpt_1)
    network(lst_total, lst_icv, lst_uniprot)

if __name__=="__main__":
    main()
```

## 训练模型

只需修改dataset.py文件。

```python
import pandas as pd
import numpy as np
import pickle
from sklearn.model_selection import train_test_split,StratifiedKFold
import scipy.io as scio

from config import Config

class HetrDataset():
    def __init__(self):
        config = Config()
        self.repeat_nums = config.repeat_nums
        self.fold_nums = config.fold_nums
        self.neg_samp_ratio = config.neg_samp_ratio

        self.dg_smi_path = config.dg_smiles_path
        self.pt_fas_path = config.pt_fasta_path
        self.smi_dict_path = config.smi_dict_path
        self.fas_dict_path = config.fas_dict_path
        self.smi_ngram = config.smi_n_gram
        self.fas_ngram = config.fas_n_gram
        self.smi_max_len = config.smiles_max_len
        self.fas_max_len = config.fasta_max_len

        self.train = config.train_pair_path
        self.val = config.val_pair_path

        #self.dg_pt_path = config.dg_pt_path

        self.read_data()
        self.pre_process()

    def read_data(self):
        #这是序列数据，还没转变为数值特征
        #self.drug_smi = pd.read_csv(self.dg_smi_path,header=None,index_col=None).values
        self.smiles_dict = {}
        with open(self.dg_smi_path) as f:
            f1 = f.readlines()
        for line in f1:
            self.smiles_dict[line.split(",")[0]]=line.split(",")[1].strip()

        # print(self.smi_dict["312540"])
        self.fasta_dict = {}
        with open(self.pt_fas_path) as f:
            f1 = f.readlines()
        for line in f1:
            self.fasta_dict[line.split(",")[0]]=line.split(",")[1].strip()
        #self.protein_fas = pd.read_csv(self.pt_fas_path,header=None,index_col=None).values
        #导入字符与数值之间映射的字典
        with open(self.smi_dict_path, "rb") as f:
            self.smi_dict = pickle.load(f)
        with open(self.fas_dict_path, "rb") as f:
            self.fas_dict = pickle.load(f)

        #self.dg_pt = pd.read_csv(self.dg_pt_path, header=0, index_col=0).values
        print("read_data finshed!!!")


    def pre_process(self):
        """
        :return:all_data_set:list    repeat_nums*fold_nums*3
        """
        #self.all_data_set = []      #[]:repeat_nums*fold_nums*3
        #whole_positive_index = []
        #whole_negetive_index = []
        #for i in range(self.dg_pt.shape[0]):
        #    for j in range(self.dg_pt.shape[1]):
        #        if int(self.dg_pt[i, j]) == 1:
        #            whole_positive_index.append([i, j])
        #        elif int(self.dg_pt[i, j]) == 0:
        #            whole_negetive_index.append([i, j])
        #print("positive and negative finshed!!!")

        #for x in range(self.repeat_nums):

            #对负2例进行self.neg_samp_ratio倍正例的下采样
        #    negative_sample_index = np.random.choice(np.arange(len(whole_negetive_index)),
        #                                             size=self.neg_samp_ratio * len(whole_positive_index),replace=False)
        #    data_set = np.zeros((self.neg_samp_ratio*len(whole_positive_index) + len(negative_sample_index),3), dtype=int)

        #    count = 0
        #    for item in whole_positive_index:
                #对正例进行self.neg_samp_ratio倍过采样
                # for i in range(self.neg_samp_ratio):
        #        data_set[count][0] = item[0]
        #        data_set[count][1] = item[1]
        #        data_set[count][2] = 1
        #        count = count + 1
        #    for i in negative_sample_index:
        #        data_set[count][0] = whole_negetive_index[i][0]
        #        data_set[count][1] = whole_negetive_index[i][1]
        #        data_set[count][2] = 0
        #        count = count + 1

        #    all_fold_dataset = []
        #    rs = np.random.randint(0,1000,1)[0]
        #    kf = StratifiedKFold(n_splits=self.fold_nums, shuffle=True, random_state=rs)
        #    for train_index, test_index in kf.split(data_set[:,0:2], data_set[:, 2]):
        #        train_data, test_data = data_set[train_index], data_set[test_index]
        #        one_fold_dataset = []
        #        one_fold_dataset.append(train_data)
        #        one_fold_dataset.append(test_data)
        #        all_fold_dataset.append(one_fold_dataset)
        #        break
            # for aa in range(10):
            #     print("fold" + str(aa) + ":",list(all_fold_dataset[aa][1][:,2]).count(1),len(all_fold_dataset[0][1]))
            #print("fold1:",list(all_fold_dataset[0][1][:,2]).count(1),len(all_fold_dataset[0][1]))
            #print("fold1:",list(all_fold_dataset[1][1][:,2]).count(1),len(all_fold_dataset[0][1]))
            #print("fold1:",list(all_fold_dataset[2][1][:,2]).count(1),len(all_fold_dataset[0][1]))
            #print("fold1:",list(all_fold_dataset[3][1][:,2]).count(1),len(all_fold_dataset[0][1]))
        #    self.all_data_set.append(all_fold_dataset)
        #print("fold finshed!!!")

        #将序列转为数值特征表示
        #self.smi_input = np.zeros((len(self.drug_smi),self.smi_max_len),dtype=int)
        #self.fas_input = np.zeros((len(self.protein_fas),self.fas_max_len),dtype=int)

        #for i in range(len(self.drug_smi)):
        #    for j in range(len(self.drug_smi[i,1]) - self.smi_ngram +1):
        #        key = self.drug_smi[i,1][j:j + self.smi_ngram]
        #        self.smi_input[i,j] = self.smi_dict[key]

        #for i in range(len(self.protein_fas)):
        #    for j in range(len(self.protein_fas[i,1]) - self.fas_ngram +1):
        #        key = self.protein_fas[i,1][j:j + self.fas_ngram]
        #        self.fas_input[i,j] = self.fas_dict[key]
        #print("Embedding finshed!!!")
        self.train_lst = []
        with open(self.train) as f:
            f1 = f.readlines()
        [self.train_lst.append(i.strip().split(",")) for i in f1]
        self.val_lst = []
        with open(self.val) as f:
            f1 = f.readlines()
        [self.val_lst.append(i.strip().split(",")) for i in f1]

    def get_train_batch(self,repeat_nums,flod_nums,batch_size):

        #train_drugs = []
        #train_proteins = []
        #train_affinity = []
        #train_data = self.all_data_set[repeat_nums][flod_nums][0]
        #print(type(train_data))     ndarray类型,没打乱，tag先1后0
        #for index,(i,j,tag) in enumerate(train_data):
        #    train_drugs.append(self.smi_input[i])
        #    train_proteins.append(self.fas_input[j])
        #    train_affinity.append(tag)

        #train_drugs = np.array(train_drugs)
        #train_proteins = np.array(train_proteins)
        #train_affinity = np.array(train_affinity)

        #打乱训练数据和标签，通过打乱索引从而打乱数据，数据量很大时能节约内存，并且每次生成的数据都不一样
        #data_index = np.arange(len(train_drugs))        #生成下标
        #np.random.shuffle(data_index)
        #train_drugs = train_drugs[data_index]
        #train_proteins = train_proteins[data_index]
        #train_affinity = train_affinity[data_index]

        #迭代返回
        sindex = 0
        eindex = batch_size
        while eindex < len(self.train_lst):
            #tra_dg_batch = train_drugs[sindex:eindex,:]
            #tra_pt_batch = train_proteins[sindex:eindex,:]
            #tra_tag_batch = train_affinity[sindex:eindex]
            tra_dg_batch = []
            tra_pt_batch = []
            tra_tag_batch = []
            for num in range(sindex, eindex):
                # print(self.smiles_dict)
                # print(self.train_lst[num][0])
                tra_dg_batch.append(self.smiles_dict[self.train_lst[num][0]])
                tra_pt_batch.append(self.fasta_dict[self.train_lst[num][1]])
                tra_tag_batch.append(int(self.train_lst[num][2]))
            smi_input = np.zeros((len(tra_dg_batch),self.smi_max_len),dtype=int)
            fas_input = np.zeros((len(tra_pt_batch),self.fas_max_len),dtype=int)

            for i in range(len(tra_dg_batch)):
                for j in range(len(tra_dg_batch[i]) - self.smi_ngram +1):
                    key = tra_dg_batch[i][j:j + self.smi_ngram]
                    smi_input[i,j] = self.smi_dict[key]

            for i in range(len(tra_pt_batch)):
                for j in range(len(tra_pt_batch[i]) - self.fas_ngram +1):
                    key = tra_pt_batch[i][j:j + self.fas_ngram]
                    fas_input[i,j] = self.fas_dict[key]

            tra_dg_batch = np.array(smi_input)
            tra_pt_batch = np.array(fas_input)
            tra_tag_batch = np.array(tra_tag_batch)

            temp = eindex
            eindex = eindex + batch_size
            sindex = temp
            yield tra_dg_batch,tra_pt_batch,tra_tag_batch

        if eindex >= len(self.train_lst):
            #tra_dg_batch = train_drugs[sindex:,:]
            #tra_pt_batch = train_proteins[sindex:,:]
            #tra_tag_batch = train_affinity[sindex:]
            #yield tra_dg_batch,tra_pt_batch,tra_tag_batch
            tra_dg_batch = []
            tra_pt_batch = []
            tra_tag_batch = []
            for num in range(sindex, len(self.train_lst)):
                tra_dg_batch.append(self.smiles_dict[self.train_lst[num][0]])
                tra_pt_batch.append(self.fasta_dict[self.train_lst[num][1]])
                tra_tag_batch.append(int(self.train_lst[num][2]))
            smi_input = np.zeros((len(tra_dg_batch),self.smi_max_len),dtype=int)
            fas_input = np.zeros((len(tra_pt_batch),self.fas_max_len),dtype=int)

            for i in range(len(tra_dg_batch)):
                for j in range(len(tra_dg_batch[i]) - self.smi_ngram +1):
                    key = tra_dg_batch[i][j:j + self.smi_ngram]
                    smi_input[i,j] = self.smi_dict[key]

            for i in range(len(tra_pt_batch)):
                for j in range(len(tra_pt_batch[i]) - self.fas_ngram +1):
                    key = tra_pt_batch[i][j:j + self.fas_ngram]
                    fas_input[i,j] = self.fas_dict[key]

            tra_dg_batch = np.array(smi_input)
            tra_pt_batch = np.array(fas_input)
            tra_tag_batch = np.array(tra_tag_batch)

            temp = eindex
            eindex = eindex + batch_size
            sindex = temp
            yield tra_dg_batch,tra_pt_batch,tra_tag_batch

    def get_test_batch(self,repeat_nums,flod_nums,batch_size):
        #测试可以一次性输入吧，如果内存充足的话
        #train_drugs = []
        #train_proteins = []
        #train_affinity = []
        #train_data = self.all_data_set[repeat_nums][flod_nums][1]
        #print(type(train_data))     ndarray类型,没打乱，tag先1后0
        #for index,(i,j,tag) in enumerate(train_data):
        #    train_drugs.append(self.smi_input[i])
        #    train_proteins.append(self.fas_input[j])
        #    train_affinity.append(tag)

        # train_drugs = np.array(train_drugs)
        # train_proteins = np.array(train_proteins)
        # train_affinity = np.array(train_affinity)

        # #打乱训练数据和标签，通过打乱索引从而打乱数据，数据量很大时能节约内存，并且每次生成的数据都不一样
        # data_index = np.arange(len(train_drugs))        #生成下标
        # np.random.shuffle(data_index)
        # train_drugs = train_drugs[data_index]
        # train_proteins = train_proteins[data_index]
        # train_affinity = train_affinity[data_index]

        # #迭代返回
        # sindex = 0
        # eindex = batch_size
        # while eindex < len(train_drugs):
        #     tra_dg_batch = train_drugs[sindex:eindex,:]
        #     tra_pt_batch = train_proteins[sindex:eindex,:]
        #     tra_tag_batch = train_affinity[sindex:eindex]

        #     temp = eindex
        #     eindex = eindex + batch_size
        #     sindex = temp
        #     yield tra_dg_batch,tra_pt_batch,tra_tag_batch

        # if eindex >= len(train_drugs):
        #     tra_dg_batch = train_drugs[sindex:,:]
        #     tra_pt_batch = train_proteins[sindex:,:]
        #     tra_tag_batch = train_affinity[sindex:]
        #     yield tra_dg_batch,tra_pt_batch,tra_tag_batch
        sindex = 0
        eindex = batch_size
        while eindex < len(self.val_lst):
            #tra_dg_batch = train_drugs[sindex:eindex,:]
            #tra_pt_batch = train_proteins[sindex:eindex,:]
            #tra_tag_batch = train_affinity[sindex:eindex]
            tra_dg_batch = []
            tra_pt_batch = []
            tra_tag_batch = []
            for num in range(sindex, eindex):
                tra_dg_batch.append(self.smiles_dict[self.val_lst[num][0]])
                tra_pt_batch.append(self.fasta_dict[self.val_lst[num][1]])
                tra_tag_batch.append(int(self.val_lst[num][2]))
            smi_input = np.zeros((len(tra_dg_batch),self.smi_max_len),dtype=int)
            fas_input = np.zeros((len(tra_pt_batch),self.fas_max_len),dtype=int)

            for i in range(len(tra_dg_batch)):
                for j in range(len(tra_dg_batch[i]) - self.smi_ngram +1):
                    key = tra_dg_batch[i][j:j + self.smi_ngram]
                    smi_input[i,j] = self.smi_dict[key]

            for i in range(len(tra_pt_batch)):
                for j in range(len(tra_pt_batch[i]) - self.fas_ngram +1):
                    key = tra_pt_batch[i][j:j + self.fas_ngram]
                    fas_input[i,j] = self.fas_dict[key]

            tra_dg_batch = np.array(smi_input)
            tra_pt_batch = np.array(fas_input)
            tra_tag_batch = np.array(tra_tag_batch)

            temp = eindex
            eindex = eindex + batch_size
            sindex = temp
            yield tra_dg_batch,tra_pt_batch,tra_tag_batch

        if eindex >= len(self.val_lst):
            #tra_dg_batch = train_drugs[sindex:,:]
            #tra_pt_batch = train_proteins[sindex:,:]
            #tra_tag_batch = train_affinity[sindex:]
            #yield tra_dg_batch,tra_pt_batch,tra_tag_batch
            tra_dg_batch = []
            tra_pt_batch = []
            tra_tag_batch = []
            for num in range(sindex, len(self.val_lst)):
                tra_dg_batch.append(self.smiles_dict[self.val_lst[num][0]])
                tra_pt_batch.append(self.fasta_dict[self.val_lst[num][1]])
                tra_tag_batch.append(int(self.val_lst[num][2]))
            smi_input = np.zeros((len(tra_dg_batch),self.smi_max_len),dtype=int)
            fas_input = np.zeros((len(tra_pt_batch),self.fas_max_len),dtype=int)

            for i in range(len(tra_dg_batch)):
                for j in range(len(tra_dg_batch[i]) - self.smi_ngram +1):
                    key = tra_dg_batch[i][j:j + self.smi_ngram]
                    smi_input[i,j] = self.smi_dict[key]

            for i in range(len(tra_pt_batch)):
                for j in range(len(tra_pt_batch[i]) - self.fas_ngram +1):
                    key = tra_pt_batch[i][j:j + self.fas_ngram]
                    fas_input[i,j] = self.fas_dict[key]

            tra_dg_batch = np.array(smi_input)
            tra_pt_batch = np.array(fas_input)
            tra_tag_batch = np.array(tra_tag_batch)

            temp = eindex
            eindex = eindex + batch_size
            sindex = temp
            yield tra_dg_batch,tra_pt_batch,tra_tag_batch

#ht = HetrDataset()
```

