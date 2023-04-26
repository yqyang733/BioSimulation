👏 工具|MultiDTI|建立分类模型预测药物靶标相互作用(小数据)

---

[封面PPT](../PPT/工具MultiDTI建立分类模型预测药物靶标相互作用(小数据).pptx)

---

[TOC]

---

## 生成输入文件

### SMILES处理

参考：[工具|DeepPurpose|MPNN-CNN建立分类/回归模型预测药物靶标相互作用(小数据)](./工具DeepPurposeMPNN-CNN建立分类回归模型预测药物靶标相互作用小数据.md)，只需进行正则化和去盐即可。

### 生成 drug_smiles, protein_fasta, drug_protein 文件

**注意**：顺序一定要对上。

（1）三个文件格式如PPT中图片所示

（2）脚本如下所示：

matrix.py

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
    lst_uniprot_unique = list(set(lst_uniprot))
    matrix_1 = np.zeros((len(lst_icv_unique),len(lst_uniprot_unique)))
    for i in range(len(lst_icv_unique)):
        for j in range(len(lst_uniprot_unique)):
            if [lst_icv_unique[i],lst_uniprot_unique[j]] in lst_total:
                matrix_1[i][j] += 1
    output = ""
    output += "network"
    output_icv = ""
    output_pro = ""
    for a in lst_uniprot_unique:
        output += ","
        output += a
        output_pro = output_pro + a + "\n"
    output += "\n"
    for b in range(len(lst_icv_unique)):
        output_icv = output_icv + lst_icv_unique[b] + "\n"
        output += lst_icv_unique[b]
        for c in matrix_1[b]:
            output += ","
            output += str(c)
        output += "\n"
    with open("result_matrix.txt","w") as rt, open("result_icv.txt","w") as rt_icv, open("result_pro.txt","w") as rt_pro:
        rt.write(output)
        rt_icv.write(output_icv)
        rt_pro.write(output_pro)

def main():
    inpt_1 = str(sys.argv[1])
    lst_total, lst_icv, lst_uniprot = pre_process(inpt_1)
    network(lst_total, lst_icv, lst_uniprot)

if __name__=="__main__":
    main()
```

pick.py

```python
import sys

def pre_process(inpt_1, inpt_2):
    dic = {}
    lst = []
    with open(inpt_1) as f:
        f1 = f.readlines()
    for line in f1:
        lst.append(line.strip())
    with open(inpt_2) as f2:
        f3 = f2.readlines()
    for line in f3:
        icv = line.strip().split(",")[0]
        smiles = line.strip().split(",")[1]
        dic[icv] = smiles
    output = ""
    for a in lst:
        output = output + a + "," + dic[a] + "\n"
    with open("protein_fasta_outtest.txt", "w") as rt:
        rt.write(output)

def main():
    inpt_1 = str(sys.argv[1])
    inpt_2 = str(sys.argv[2])
    pre_process(inpt_1, inpt_2)

if __name__=="__main__":
    main()
```

### 生成词典

```python
#对序列型符号进行分词，我们采用是n-gram的方法，将n个字符作为一个分词单位，用错开一个字符的方法构造出n个子序列
#而某个序列的高维向量表示就是这个子序列的对应向量的平均值。
#对于fasta序列，3个氨基酸可能有特殊的含义，所以一般使用3-gram
#对于smiles序列，这里将单个字符作为一个词
#在这里我们构造smiles序列和fasta序列的词汇表

import pandas as pd
import pickle

smile_gram = 1
fasta_gram = 3
smi_dict_set = set()
fas_dict_set = set()
one_fas_dict_set = set()

smi_max_len = 0
fas_max_len = 0

smile = pd.read_csv("drug_smiles.csv",header=None,index_col=None)
fasta = pd.read_csv("protein_fasta.csv",header=None,index_col=None)

for item in smile.values:
    if len(item[1]) < smile_gram:
        while len(item[1]) < smile_gram:
            item[1] = item[1] + "_"
        smi_dict_set.add(item[1])
    else:
        for i in range(len(item[1]) - smile_gram + 1):
            smi_dict_set.add(item[1][i:i+smile_gram])

    if len(item[1]) > smi_max_len:
        smi_max_len = len(item[1])

for item in fasta.values:
    if len(item[1]) < fasta_gram:
        while len(item[1]) < smile_gram:
            item[1] = item[1] + "_"
        fas_dict_set.add(item[1])
    else:
        for i in range(len(item[1]) - fasta_gram +1):
            fas_dict_set.add(item[1][i:i+fasta_gram])

    if len(item[1]) > fas_max_len:
        fas_max_len = len(item[1])

for item in fas_dict_set:
    one_fas_dict_set.add(item[0])
    one_fas_dict_set.add(item[1])
    one_fas_dict_set.add(item[2])

print(len(smi_dict_set))
print(smi_dict_set)
print(len(fas_dict_set))
print(fas_dict_set)
print(len(one_fas_dict_set))
print(one_fas_dict_set)
print("smiles最大序列长度：",smi_max_len)
print("fasta最大序列长度：",fas_max_len)

smi_dict = {}
fas_dict = {}
count = 1
for item in smi_dict_set:
    smi_dict[item] = count
    count = count + 1
count = 1
for item in fas_dict_set:
    fas_dict[item] = count
    count = count + 1
print(smi_dict)
print(fas_dict)
with open("smi_dict.pickle","wb") as f:
    pickle.dump(smi_dict,f)
with open("fas_dict.pickle","wb") as f:
    pickle.dump(fas_dict,f)
"""
读取smi_dict的方式
with open("smi_dict.pickle","rb") as f:
    test1 = pickle.load(f)
with open("fas_dict.pickle","rb") as f:
    test2 = pickle.load(f)
print(test1)
print(test2)
"""
```

---

## 十折交叉训练模型确定超参数

这里主要介绍相对于原始的包需要修改的部分。

由于原始的包只输出了AUC的值，没法作图，这里增加代码用于输出验证集的真实值和预测值以便作图。

```python
import time
import datetime
import os

import numpy as np
import torch.optim as optim
import torch
from torch import nn
from torch.autograd import Variable
from torch.nn import functional as F
from sklearn.metrics import roc_auc_score
from sklearn.metrics import average_precision_score

from config import Config
from utils.util import Helper
from model.CommonModel import Common_model
from model.PredictModel import Predict_model
from dataset import HetrDataset

def train_common_model(config,helper,model,hetrdataset,repeat_nums,flod_nums):

    optimizer = optim.Adam(model.parameters(),config.common_learn_rate)
    model.train()

    print("common model begin training----------",datetime.datetime.now())

    #common_loss
    for e in range(config.common_epochs):

        common_loss = 0
        begin_time = time.time()

        for i, (dg,pt,tag) in enumerate(hetrdataset.get_train_batch(repeat_nums,flod_nums,config.batch_size)):
            dg = helper.to_longtensor(dg,config.use_gpu)
            pt = helper.to_longtensor(pt,config.use_gpu)
            tag = helper.to_floattensor(tag,config.use_gpu)

            optimizer.zero_grad()

            smi_common, fas_common = model(dg,pt)

            distance_loss = helper.comput_distance_loss(smi_common,fas_common,tag)
            common_loss += distance_loss

            distance_loss.backward()
            optimizer.step()

        #end a epoch
        print("the loss of common model epoch[%d / %d]:is %4.f, time:%d s" % (e+1,config.common_epochs,common_loss,time.time()-begin_time))
    return common_loss.item()

def train_predict_model(config,helper,predict_model,common_model,hetrdataset,repeat_nums,flod_nums):

    optimizer1 = optim.Adam(predict_model.parameters(),config.pre_learn_rate)
    optimizer2 = optim.Adam(common_model.parameters(),config.common_learn_rate)

    predict_model.train()
    common_model.train()

    print("predict model begin training----------",datetime.datetime.now())

    #tag_loss
    for e in range(config.predict_epochs):

        epoch_loss = 0
        begin_time = time.time()

        for i, (dg,pt,tag) in enumerate(hetrdataset.get_train_batch(repeat_nums,flod_nums,config.batch_size)):
            dg = helper.to_longtensor(dg,config.use_gpu)
            pt = helper.to_longtensor(pt,config.use_gpu)
            tag = helper.to_floattensor(tag,config.use_gpu)

            smi_common,fas_common = common_model(dg,pt)

            optimizer1.zero_grad()
            optimizer2.zero_grad()

            predict, tag  = predict_model(smi_common,fas_common,tag)

            tag_loss = F.binary_cross_entropy(predict,tag)
            epoch_loss += tag_loss

            tag_loss.backward()

            optimizer1.step()
            optimizer2.step()

        # end a epech
        print("the loss of predict model epoch[%d / %d]:is %4.f, time:%d s" %  (e+1, config.predict_epochs, epoch_loss, time.time() - begin_time))

        #将每一折交叉验证的模型存储起来
        if not os.path.exists('./results'):
            os.mkdir('./results')
        if not os.path.exists('./results/com_model_parm'):
            os.mkdir('./results/com_model_parm')
        if not os.path.exists('./results/pre_model_parm'):
            os.mkdir('./results/pre_model_parm')

        #evaluation_model(config, helper, predict_model, common_model, hetrdataset, repeat_nums, flod_nums)
        if e == config.predict_epochs-1 and epoch == config.num_epochs-1:
            torch.save(common_model.state_dict(),
                       './results/com_model_parm/repeat_%d_corss_%d.parm' % (repeat_nums, flod_nums))
            torch.save(predict_model.state_dict(),
                       './results/pre_model_parm/repeat_%d_corss_%d.parm' % (repeat_nums, flod_nums))
            #评估模型
        loss, pre_all, lab_all = evaluation_model(config, helper, predict_model, common_model, hetrdataset, repeat_nums, flod_nums)

    return epoch_loss.item(), loss, pre_all, lab_all

def evaluation_model(config,helper,predict_model,common_model,hetrdataset,repeat_nums,flod_nums):
    predict_model.eval()
    common_model.eval()
    print("evaluate the model")

    begin_time = time.time()
    loss = 0
    avg_acc = []
    avg_aupr = []
    pre_all = []
    lab_all = []
    with torch.no_grad():
        auc_list = []
        auc_list_pre = []
        for i,(dg,pt,tag) in enumerate(hetrdataset.get_test_batch(repeat_nums,flod_nums,config.batch_size)):
            dg = helper.to_longtensor(dg,config.use_gpu)
            pt = helper.to_longtensor(pt,config.use_gpu)
            tag = helper.to_floattensor(tag,config.use_gpu)

            smi_common,fas_common = common_model(dg,pt)
            predict, tag = predict_model(smi_common, fas_common, tag)

            pre_all.append(predict)
            lab_all.append(tag)

            tag_loss = F.binary_cross_entropy(predict,tag)
            loss +=tag_loss

            #print(tag.cpu(),predict.cpu())
            auc_list.append(tag.cpu())
            auc_list_pre.append(predict.cpu())
        #print(auc_list,auc_list_pre)
        auc = roc_auc_score(torch.cat(auc_list,0),torch.cat(auc_list_pre,0))
        aupr = average_precision_score(torch.cat(auc_list,0),torch.cat(auc_list_pre,0))

            #avg_acc.append(auc)
            #avg_aupr.append(aupr)

    print("the total_loss of test model:is %4.f, time:%d s" % (loss, time.time() - begin_time))
    print("avg_acc:",auc,"avg_aupr:",aupr)

    return loss.item(), pre_all, lab_all

if __name__=='__main__':

    # initial parameters class
    config = Config()

    # initial utils class
    helper = Helper()

    #initial data
    hetrdataset = HetrDataset()

    #torch.backends.cudnn.enabled = False 把

    model_begin_time = time.time()
    for i in range(config.repeat_nums):
        print("repeat:",str(i),"+++++++++++++++++++++++++++++++++++")
        for j in range(config.fold_nums):
            print(" crossfold:", str(j), "----------------------------")
            if not os.path.exists('./results/result_' + str(j) + "/"):
                os.makedirs('./results/result_' + str(j) + "/")
            #initial presentation model
            c_model = Common_model(config)
            p_model = Predict_model()
            if config.use_gpu:
                c_model = c_model.cuda()
                p_model = p_model.cuda()

            loss_rt = "model_loss,predict_loss,val_loss\n"
            for epoch in range(config.num_epochs):
                print("         epoch:",str(epoch),"zzzzzzzzzzzzzzzz")
                common_loss = train_common_model(config,helper,c_model,hetrdataset,i,j)
                predict_loss, val_loss, pre_all, lab_all = train_predict_model(config,helper,p_model,c_model,hetrdataset,i,j)
                loss_rt = loss_rt + str(common_loss) + "," + str(predict_loss) + "," + str(val_loss) + "\n"
                print(str(common_loss) + "," + str(predict_loss) + "," + str(val_loss) + "\n")
            with open('./results/result_' + str(j) + "/" + "all_loss_" + str(j) + ".csv","w") as rt:
                rt.write(loss_rt)
            pre_lab = "y_pre,y_lab\n"
            # for i in range(len(pre_all)):
            #     pre_lab = pre_lab + str(pre_all[i]) + "," + str(lab_all[i]) + "\n"
            for qq in range(len(pre_all)):
                a = pre_all[qq].tolist()
                b = lab_all[qq].tolist()
                for m in range(len(a)):
                    pre_lab = pre_lab + str(a[m]) + "," + str(b[m]) + "\n"
            with open('./results/result_' + str(j) + "/" + "pre_lab_" + str(j) + ".csv","w") as rt:
                rt.write(pre_lab)

            # with open('./results/result_' + str(j) + "/" + "all_loss_" + str(j) + ".csv","w") as rt:
            #     rt.write("model_loss,predict_loss,val_loss\n")
            #     for epoch in range(config.num_epochs):
            #         print("         epoch:",str(epoch),"zzzzzzzzzzzzzzzz")
            #         common_loss = train_common_model(config,helper,c_model,hetrdataset,i,j)
            #         predict_loss, val_loss, pre_all, lab_all = train_predict_model(config,helper,p_model,c_model,hetrdataset,i,j)
            #         rt.write(str(common_loss) + "," + str(predict_loss) + "," + str(val_loss) + "\n")
            #         print(str(common_loss) + "," + str(predict_loss) + "," + str(val_loss) + "\n")
            #     pre_lab = "y_pre,y_lab\n"
            #     for i in range(len(pre_all)):
            #         a = pre_all[i].tolist()
            #         b = lab_all[i].tolist()
            #         for m in range(len(a)):
            #             pre_lab = pre_lab + str(a[m]) + "," + str(b[m]) + "\n"
            #     with open('./results/result_' + str(j) + "/" + "pre_lab_" + str(j) + ".csv","w") as rt:
            #         rt.write(pre_lab)

    print("Done!")
    print("All_training time:",time.time()-model_begin_time)
```

---

## 基于所有数据训练最终模型

修改主体main.py文件。

```python
import time
import datetime
import os

import numpy as np
import torch.optim as optim
import torch
from torch import nn
from torch.autograd import Variable
from torch.nn import functional as F
from sklearn.metrics import roc_auc_score
from sklearn.metrics import average_precision_score

from config import Config
from utils.util import Helper
from model.CommonModel import Common_model
from model.PredictModel import Predict_model
from dataset import HetrDataset

def train_common_model(config,helper,model,hetrdataset):

    optimizer = optim.Adam(model.parameters(),config.common_learn_rate)
    model.train()

    print("common model begin training----------",datetime.datetime.now())

    #common_loss
    for e in range(config.common_epochs):

        common_loss = 0
        begin_time = time.time()

        for i, (dg,pt,tag) in enumerate(hetrdataset.get_train_batch(config.batch_size)):
            dg = helper.to_longtensor(dg,config.use_gpu)
            pt = helper.to_longtensor(pt,config.use_gpu)
            tag = helper.to_floattensor(tag,config.use_gpu)

            optimizer.zero_grad()

            smi_common, fas_common = model(dg,pt)

            distance_loss = helper.comput_distance_loss(smi_common,fas_common,tag)
            common_loss += distance_loss

            distance_loss.backward()
            optimizer.step()

        #end a epech
        print("the loss of common model epoch[%d / %d]:is %4.f, time:%d s" % (e+1,config.common_epochs,common_loss,time.time()-begin_time))
    return common_loss.item()

def train_predict_model(config,helper,predict_model,common_model,hetrdataset):

    optimizer1 = optim.Adam(predict_model.parameters(),config.pre_learn_rate)
    optimizer2 = optim.Adam(common_model.parameters(),config.common_learn_rate)

    predict_model.train()
    common_model.train()

    print("predict model begin training----------",datetime.datetime.now())

    #tag_loss
    for e in range(config.predict_epochs):

        epoch_loss = 0
        begin_time = time.time()

        for i, (dg,pt,tag) in enumerate(hetrdataset.get_train_batch(config.batch_size)):
            dg = helper.to_longtensor(dg,config.use_gpu)
            pt = helper.to_longtensor(pt,config.use_gpu)
            tag = helper.to_floattensor(tag,config.use_gpu)

            smi_common,fas_common = common_model(dg,pt)

            optimizer1.zero_grad()
            optimizer2.zero_grad()

            predict, tag  = predict_model(smi_common,fas_common,tag)

            tag_loss = F.binary_cross_entropy(predict,tag)
            epoch_loss += tag_loss

            tag_loss.backward()

            optimizer1.step()
            optimizer2.step()

        # end a epech
        print("the loss of predict model epoch[%d / %d]:is %4.f, time:%d s" %  (e+1, config.predict_epochs, epoch_loss, time.time() - begin_time))

        #将每一折交叉验证的模型存储起来
        if not os.path.exists('./results'):
            os.mkdir('./results')
        if not os.path.exists('./results/com_model_parm'):
            os.mkdir('./results/com_model_parm')
        if not os.path.exists('./results/pre_model_parm'):
            os.mkdir('./results/pre_model_parm')

        #evaluation_model(config, helper, predict_model, common_model, hetrdataset, repeat_nums, flod_nums)
        if e == config.predict_epochs-1 and epoch == config.num_epochs-1:
            torch.save(common_model.state_dict(),
                       './results/com_model_parm/repeat_corss_all.parm')
            torch.save(predict_model.state_dict(),
                       './results/pre_model_parm/repeat_corss_all.parm')
            #评估模型
        #loss, pre_all, lab_all = evaluation_model(config, helper, predict_model, common_model, hetrdataset, repeat_nums, flod_nums)

    #return epoch_loss.item(), loss, pre_all, lab_all

def evaluation_model(config,helper,predict_model,common_model,hetrdataset,repeat_nums,flod_nums):
    predict_model.eval()
    common_model.eval()
    print("evaluate the model")

    begin_time = time.time()
    loss = 0
    avg_acc = []
    avg_aupr = []
    pre_all = []
    lab_all = []
    with torch.no_grad():
        for i,(dg,pt,tag) in enumerate(hetrdataset.get_test_batch(repeat_nums,flod_nums,config.batch_size)):
            dg = helper.to_longtensor(dg,config.use_gpu)
            pt = helper.to_longtensor(pt,config.use_gpu)
            tag = helper.to_floattensor(tag,config.use_gpu)

            smi_common,fas_common = common_model(dg,pt)
            predict, tag = predict_model(smi_common, fas_common, tag)

            pre_all.append(predict)
            lab_all.append(tag)

            tag_loss = F.binary_cross_entropy(predict,tag)
            loss +=tag_loss

            auc = roc_auc_score(tag.cpu(),predict.cpu())
            aupr = average_precision_score(tag.cpu(),predict.cpu())

            avg_acc.append(auc)
            avg_aupr.append(aupr)

    print("the total_loss of test model:is %4.f, time:%d s" % (loss, time.time() - begin_time))
    print("avg_acc:",np.mean(avg_acc),"avg_aupr:",np.mean(avg_aupr))

    return loss.item(), pre_all, lab_all

if __name__=='__main__':

    # initial parameters class
    config = Config()

    # initial utils class
    helper = Helper()

    #initial data
    hetrdataset = HetrDataset()

    #torch.backends.cudnn.enabled = False 把

    model_begin_time = time.time()
    #for i in range(config.repeat_nums):
    #print("repeat:",str(i),"+++++++++++++++++++++++++++++++++++")
        #for j in range(config.fold_nums):
        #    print(" crossfold:", str(j), "----------------------------")
        #    if not os.path.exists('./results/result_' + str(j) + "/"):
        #        os.makedirs('./results/result_' + str(j) + "/")
            #initial presentation model
    c_model = Common_model(config)
    p_model = Predict_model()
    if config.use_gpu:
        c_model = c_model.cuda()
        p_model = p_model.cuda()

        #    loss_rt = "model_loss,predict_loss,val_loss\n"
    for epoch in range(config.num_epochs):
        #        print("         epoch:",str(epoch),"zzzzzzzzzzzzzzzz")
        train_common_model(config,helper,c_model,hetrdataset)
        train_predict_model(config,helper,p_model,c_model,hetrdataset)
        #        loss_rt = loss_rt + str(common_loss) + "," + str(predict_loss) + "," + str(val_loss) + "\n"
        #        print(str(common_loss) + "," + str(predict_loss) + "," + str(val_loss) + "\n")
        #    with open('./results/result_' + str(j) + "/" + "all_loss_" + str(j) + ".csv","w") as rt:
        #        rt.write(loss_rt)
        #    pre_lab = "y_pre,y_lab\n"
            # for i in range(len(pre_all)):
            #     pre_lab = pre_lab + str(pre_all[i]) + "," + str(lab_all[i]) + "\n"
        #    for qq in range(len(pre_all)):
        #        a = pre_all[qq].tolist()
        #        b = lab_all[qq].tolist()
        #        for m in range(len(a)):
        #            pre_lab = pre_lab + str(a[m]) + "," + str(b[m]) + "\n"
        #    with open('./results/result_' + str(j) + "/" + "pre_lab_" + str(j) + ".csv","w") as rt:
        #        rt.write(pre_lab)

            # with open('./results/result_' + str(j) + "/" + "all_loss_" + str(j) + ".csv","w") as rt:
            #     rt.write("model_loss,predict_loss,val_loss\n")
            #     for epoch in range(config.num_epochs):
            #         print("         epoch:",str(epoch),"zzzzzzzzzzzzzzzz")
            #         common_loss = train_common_model(config,helper,c_model,hetrdataset,i,j)
            #         predict_loss, val_loss, pre_all, lab_all = train_predict_model(config,helper,p_model,c_model,hetrdataset,i,j)
            #         rt.write(str(common_loss) + "," + str(predict_loss) + "," + str(val_loss) + "\n")
            #         print(str(common_loss) + "," + str(predict_loss) + "," + str(val_loss) + "\n")
            #     pre_lab = "y_pre,y_lab\n"
            #     for i in range(len(pre_all)):
            #         a = pre_all[i].tolist()
            #         b = lab_all[i].tolist()
            #         for m in range(len(a)):
            #             pre_lab = pre_lab + str(a[m]) + "," + str(b[m]) + "\n"
            #     with open('./results/result_' + str(j) + "/" + "pre_lab_" + str(j) + ".csv","w") as rt:
            #         rt.write(pre_lab)


    print("Done!")
    print("All_training time:",time.time()-model_begin_time)
```

修改数据准备dataset.py文件。

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

        self.dg_pt_path = config.dg_pt_path

        self.read_data()
        self.pre_process()

    def read_data(self):
        #这是序列数据，还没转变为数值特征
        self.drug_smi = pd.read_csv(self.dg_smi_path,header=None,index_col=None).values
        self.protein_fas = pd.read_csv(self.pt_fas_path,header=None,index_col=None).values
        #导入字符与数值之间映射的字典
        with open(self.smi_dict_path, "rb") as f:
            self.smi_dict = pickle.load(f)
        with open(self.fas_dict_path, "rb") as f:
            self.fas_dict = pickle.load(f)

        self.dg_pt = pd.read_csv(self.dg_pt_path, header=0, index_col=0).values


    def pre_process(self):
        """
        :return:all_data_set:list    repeat_nums*fold_nums*3
        """
        #self.all_data_set = []      #[]:repeat_nums*fold_nums*3
        whole_positive_index = []
        whole_negetive_index = []
        for i in range(self.dg_pt.shape[0]):
            for j in range(self.dg_pt.shape[1]):
                if int(self.dg_pt[i, j]) == 1:
                    whole_positive_index.append([i, j])
                elif int(self.dg_pt[i, j]) == 0:
                    whole_negetive_index.append([i, j])

        for x in range(self.repeat_nums):

            #对负2例进行self.neg_samp_ratio倍正例的下采样
            negative_sample_index = np.random.choice(np.arange(len(whole_negetive_index)),
                                                     size=self.neg_samp_ratio * len(whole_positive_index),replace=False)
            self.data_set = np.zeros((self.neg_samp_ratio*len(whole_positive_index) + len(negative_sample_index),3), dtype=int)

            count = 0
            for item in whole_positive_index:
                #对正例进行self.neg_samp_ratio倍过采样
                for i in range(self.neg_samp_ratio):
                    self.data_set[count][0] = item[0]
                    self.data_set[count][1] = item[1]
                    self.data_set[count][2] = 1
                    count = count + 1
            for i in negative_sample_index:
                self.data_set[count][0] = whole_negetive_index[i][0]
                self.data_set[count][1] = whole_negetive_index[i][1]
                self.data_set[count][2] = 0
                count = count + 1

            #all_fold_dataset = []
            #rs = np.random.randint(0,1000,1)[0]
            #kf = StratifiedKFold(n_splits=self.fold_nums, shuffle=True, random_state=rs)
            #for train_index, test_index in kf.split(data_set[:,0:2], data_set[:, 2]):
            #    train_data, test_data = data_set[train_index], data_set[test_index]
            #    one_fold_dataset = []
            #    one_fold_dataset.append(train_data)
            #    one_fold_dataset.append(test_data)
            #    all_fold_dataset.append(one_fold_dataset)

            #self.all_data_set.append(all_fold_dataset)

        #将序列转为数值特征表示
        self.smi_input = np.zeros((len(self.drug_smi),self.smi_max_len),dtype=int)
        self.fas_input = np.zeros((len(self.protein_fas),self.fas_max_len),dtype=int)

        for i in range(len(self.drug_smi)):
            for j in range(len(self.drug_smi[i,1]) - self.smi_ngram +1):
                key = self.drug_smi[i,1][j:j + self.smi_ngram]
                self.smi_input[i,j] = self.smi_dict[key]

        for i in range(len(self.protein_fas)):
            for j in range(len(self.protein_fas[i,1]) - self.fas_ngram +1):
                key = self.protein_fas[i,1][j:j + self.fas_ngram]
                self.fas_input[i,j] = self.fas_dict[key]

    def get_train_batch(self,batch_size):

        train_drugs = []
        train_proteins = []
        train_affinity = []
        train_data = self.data_set
        #print(type(train_data))     ndarray类型,没打乱，tag先1后0
        for index,(i,j,tag) in enumerate(train_data):
            train_drugs.append(self.smi_input[i])
            train_proteins.append(self.fas_input[j])
            train_affinity.append(tag)

        train_drugs = np.array(train_drugs)
        train_proteins = np.array(train_proteins)
        train_affinity = np.array(train_affinity)

        #打乱训练数据和标签，通过打乱索引从而打乱数据，数据量很大时能节约内存，并且每次生成的数据都不一样
        data_index = np.arange(len(train_drugs))        #生成下标
        np.random.shuffle(data_index)
        train_drugs = train_drugs[data_index]
        train_proteins = train_proteins[data_index]
        train_affinity = train_affinity[data_index]

        #迭代返回
        sindex = 0
        eindex = batch_size
        while eindex < len(train_drugs):
            tra_dg_batch = train_drugs[sindex:eindex,:]
            tra_pt_batch = train_proteins[sindex:eindex,:]
            tra_tag_batch = train_affinity[sindex:eindex]

            temp = eindex
            eindex = eindex + batch_size
            sindex = temp
            yield tra_dg_batch,tra_pt_batch,tra_tag_batch

        if eindex >= len(train_drugs):
            tra_dg_batch = train_drugs[sindex:,:]
            tra_pt_batch = train_proteins[sindex:,:]
            tra_tag_batch = train_affinity[sindex:]
            yield tra_dg_batch,tra_pt_batch,tra_tag_batch

    def get_test_batch(self,repeat_nums,flod_nums,batch_size):
        #测试可以一次性输入吧，如果内存充足的话
        train_drugs = []
        train_proteins = []
        train_affinity = []
        train_data = self.all_data_set[repeat_nums][flod_nums][1]
        #print(type(train_data))     ndarray类型,没打乱，tag先1后0
        for index,(i,j,tag) in enumerate(train_data):
            train_drugs.append(self.smi_input[i])
            train_proteins.append(self.fas_input[j])
            train_affinity.append(tag)

        train_drugs = np.array(train_drugs)
        train_proteins = np.array(train_proteins)
        train_affinity = np.array(train_affinity)

        #打乱训练数据和标签，通过打乱索引从而打乱数据，数据量很大时能节约内存，并且每次生成的数据都不一样
        data_index = np.arange(len(train_drugs))        #生成下标
        np.random.shuffle(data_index)
        train_drugs = train_drugs[data_index]
        train_proteins = train_proteins[data_index]
        train_affinity = train_affinity[data_index]

        #迭代返回
        sindex = 0
        eindex = batch_size
        while eindex < len(train_drugs):
            tra_dg_batch = train_drugs[sindex:eindex,:]
            tra_pt_batch = train_proteins[sindex:eindex,:]
            tra_tag_batch = train_affinity[sindex:eindex]

            temp = eindex
            eindex = eindex + batch_size
            sindex = temp
            yield tra_dg_batch,tra_pt_batch,tra_tag_batch

        if eindex >= len(train_drugs):
            tra_dg_batch = train_drugs[sindex:,:]
            tra_pt_batch = train_proteins[sindex:,:]
            tra_tag_batch = train_affinity[sindex:]
            yield tra_dg_batch,tra_pt_batch,tra_tag_batch

#ht = HetrDataset()
```

---

## 测试模型

### 获取小分子和靶标蛋白在空间中的位置

```python
import torch
import numpy as np
import pandas as pd

from config import Config
from utils.util import Helper
from model.CommonModel import Common_model
from dataset import HetrDataset
import sys

def get_commm(num):
    #导入模型参数
    com_parm = torch.load('./results/com_model_parm/repeat_corss_' + num + '.parm')
    #print(com_parm)

    config = Config()
    helper = Helper()
    com_model = Common_model(config)
    dataset = HetrDataset()

    this_use_gpu = False
    smi_input = dataset.smi_input
    fas_input = dataset.fas_input
    smi_input = helper.to_longtensor(smi_input,this_use_gpu)
    fas_input = helper.to_longtensor(fas_input,this_use_gpu)

    com_model.load_state_dict(com_parm)
    com_model.eval()

    #将异构数据集的公共空间保存起来
    #模型训练完成后，他们的表示都是固定的

    #获取drug在公共空间的表示
    batch = 14
    drug_com = None
    sindex = 0
    eindex = batch
    with torch.no_grad():
        while eindex < len(smi_input):
            smi_common, _= com_model(smi_input[sindex:eindex], fas_input[0:batch])
            smi_common = smi_common.numpy()
            if sindex == 0:
                drug_com = smi_common
            else:
                drug_com = np.concatenate((drug_com, smi_common), axis=0)

            temp = eindex
            eindex = eindex + batch
            sindex = temp

        if eindex >= len(smi_input):
            new_batch = len(smi_input) - sindex
            smi_common, _ = com_model(smi_input[sindex:], fas_input[0:new_batch])
            smi_common = smi_common.numpy()
            drug_com = np.concatenate((drug_com, smi_common), axis=0)
    print(len(drug_com))

    #获取target在公共空间的表示
    batch = 14
    target_com = None
    sindex = 0
    eindex = batch
    with torch.no_grad():
        while eindex < len(fas_input):
            _, fas_common = com_model(smi_input[0:batch], fas_input[sindex:eindex])
            fas_common = fas_common.numpy()
            if sindex == 0:
                target_com = fas_common
            else:
                target_com = np.concatenate((target_com, fas_common), axis=0)

            temp = eindex
            eindex = eindex + batch
            sindex = temp

        if eindex >= len(fas_input):
            new_batch = len(fas_input) - sindex
            _, fas_common= com_model(smi_input[0:new_batch], fas_input[sindex:])
            fas_common = fas_common.numpy()
            target_com = np.concatenate((target_com, fas_common), axis=0)
    print(len(target_com))

    #因为公共空间的获取需要一定时间，所有这里各个节点在公共空间的表示存储起来
    dg_pt = pd.read_csv('data/drug_protein.csv',header=0,index_col=0)

    dg_file = pd.DataFrame(drug_com,index=dg_pt._stat_axis.values)
    dg_file.to_csv('results/result_' + num + '/drug_com_' + num + '.csv')

    pt_file = pd.DataFrame(target_com,index=dg_pt.columns.values)
    pt_file.to_csv('results/result_' + num + '/target_com_' + num + '.csv')


def main():
    num = str(sys.argv[1])
    get_commm(num)


if __name__=="__main__":
    main()
```

### 使用外部测试集建立交叉网络进行预测

```python
import os, sys
import torch
import numpy as np
import pandas as pd
import pickle
from collections import defaultdict

from config import Config
from utils.util import Helper
from model.CommonModel import Common_model
from model.PredictModel import Predict_model
from dataset import HetrDataset


"""
这个文件的功能：
靶标预测及虚拟筛选
"""

def smiles_read(smiles):
    with open(smiles) as f:
        f1 = f.readlines()
    dict_icv_drug = {}
    for line in f1:
        dict_icv_drug[line.split(",")[0]] = line.split(",")[1].strip()
        #break
    #print(dict_icv_drug_encoding)
    return dict_icv_drug

def protein_read(protein):
    with open(protein) as f:
        f1 = f.readlines()
    dict_target_protein = {}
    for line in f1:
        dict_target_protein[line.split(",")[0]] = line.split(",")[1].strip()
    # print(len(dict_target_protein_encoding["P0C6X7_3C"]))
    # print(len(f1[0]))
    return dict_target_protein

def data_process():
    out_dict_drug = smiles_read("./outtest/drug_smiles.txt")
    out_dict_protein = protein_read("./outtest/protein_fasta.txt")
    target_set = list(out_dict_protein.keys())
    with open("./outtest/drug_protein.txt") as f:
        f1 = f.readlines()
    dti_mutidict = defaultdict(list)
    [dti_mutidict[i.split(",")[0]].append(i.split(",")[1].strip()) for i in f1]
    whole_positive = []
    whole_negetive = []
    for key in dti_mutidict:
        for i in dti_mutidict[key]:
            whole_positive.append([key,i,out_dict_drug[key],out_dict_protein[i],1])
        target_no = target_set[:]
        [target_no.remove(i) for i in dti_mutidict[key]]
        for a in target_no:
            whole_negetive.append([key,a,out_dict_drug[key],out_dict_protein[a],0])
    whole_positive = np.array(whole_positive,dtype=object)
    whole_negetive = np.array(whole_negetive,dtype=object)
    data_set = np.vstack((whole_positive,whole_negetive))
    return data_set

with open('./data/smi_dict.pickle',"rb") as f:
    smi_dict = pickle.load(f)
with open('data/fas_dict.pickle',"rb") as f:
    fas_dict = pickle.load(f)

def load_mod(num):
    #导入字典

    #导入异构数据集在公共空间的表示,并转换为tensor格式
    target_com = pd.read_csv('./results/result_' + num + '/target_com_' + num + '.csv',index_col=0,header=0)
    #print(target_com)
    target_index = target_com._stat_axis.values
    target_com = torch.FloatTensor(target_com.values)
    #print(target_com)

    #导入模型参数
    com_parm = torch.load('./results/com_model_parm/repeat_0_corss_' + num + '.parm')
    pre_parm = torch.load('./results/pre_model_parm/repeat_0_corss_' + num + '.parm')
    #print(com_parm)
    #print(pre_parm)
    return com_parm, pre_parm

# com_parm, pre_parm = load_mod(num)
config = Config()
helper = Helper()
com_model = Common_model(config)
pre_model = Predict_model()
dataset = HetrDataset()

this_use_gpu = False
smi_input = dataset.smi_input
fas_input = dataset.fas_input
smi_input = helper.to_longtensor(smi_input,this_use_gpu)
fas_input = helper.to_longtensor(fas_input,this_use_gpu)

# com_model.load_state_dict(com_parm)
# pre_model.load_state_dict(pre_parm)
# com_model.eval()
# pre_model.eval()

def transmol_linux(in_name,out_name):
    in_suffix=os.path.splitext(in_name)[1][1:]
    out_suffix=os.path.splitext(out_name)[1][1:]
    if out_suffix == "smi":
        command="obabel -i%s %s -ocan -O %s"%(in_suffix,in_name,out_name)
    else:
        command="obabel -i%s %s -o%s -O %s"%(in_suffix,in_name,out_suffix,out_name)
    os.system(command)

def dg_seq2num(smi):
    #将smiles序列通过字典的映射，转为数值表示
    output = np.zeros(config.smiles_max_len, dtype=int)

    for i in range(len(smi) - config.smi_n_gram + 1):
        key = smi[i:i + config.smi_n_gram]
        output[i] = smi_dict[key]
    return output

def tg_seq2sum(fas):
    # 将smiles序列通过字典的映射，转为数值表示
    output = np.zeros(config.fasta_max_len, dtype=int)

    for j in range(len(fas) - config.fas_n_gram + 1):
        key = fas[j:j + config.fas_n_gram]
        output[j] = fas_dict[key]
    return output

def predict_(smiless):
    temp = seq2num(smiless)
    input = []
    input.append(temp)
    input.append(temp)
    input = np.array(input)
    input = torch.LongTensor(input)
    with torch.no_grad():
        input_common, _= com_model(input,fas_input[0:2])
    tag = 1       #这个可以随便设定
    target_predict = dict()
    with torch.no_grad():
        for i in range(len(target_com)):
            predict_rate,tag = pre_model(input_common,target_com[i],tag)  #predict_rate就表示他们之间有相互作用的可能性大小
            target_predict[target_index[i]] = predict_rate[0].item()      #针对单个数据，将tensor转为python数据类型，即把tensor去掉
    return target_predict

# #异构网络的节点
# temp_name = "DB00385"
# temp = "c1cc(c2c(c1)C(=O)c1c(C2=O)c(O)c2c(c1O)C[C@](C[C@@H]2O[C@H]1C[C@@H]([C@@H]([C@@H](O1)C)O)NC(=O)C(F)(F)F)(C(=O)COC(=O)CCCC)O)OC"

# temp = seq2num(temp)
# input = []
# input.append(temp)
# input.append(temp)
# input = np.array(input)
# input = torch.LongTensor(input)
# #print(input)
# #得到这个药物在公共空间的表示，不管是原本就在公共空间内的，还是不在公共空间内的drug
# with torch.no_grad():
#     input_common, _= com_model(input,fas_input[0:2])
#     #print(input_common)

# #进行靶标预测,因为输入需要2个样本，所以这里就直接将input_common直接输入，得到2个相同的结果
# tag = 1       #这个可以随便设定
# target_predict = dict()
# with torch.no_grad():

#     for i in range(len(target_com)):
#         predict_rate,tag = pre_model(input_common,target_com[i],tag)  #predict_rate就表示他们之间有相互作用的可能性大小
#         target_predict[target_index[i]] = predict_rate[0].item()      #针对单个数据，将tensor转为python数据类型，即把tensor去掉

#     # print(target_predict)
#     # sort_predict = sorted(target_predict.items(),key= lambda x:x[1],reverse=True)   #reverse默认为False，按从小到大排序
#     # print(sort_predict)

def read_all_target():
    with open("proname_uniprot.txt") as f:
        f1 = f.readlines()
    uni_pro = {}
    for line in f1:
        uni_pro[line.split(",")[0]] = line.strip().split(",")[1]
    return uni_pro

def result_process(mol_name, uni_pro, target_predict_dict):
    output = "drug,target,possibility\n"
    for key in target_predict_dict:
        output = output + mol_name + "," + uni_pro[key] + "," + str(target_predict_dict[key]) + "\n"
    if not os.path.exists('./result_output'):
        os.mkdir('./result_output')
    with open('./result_output' + "/target_predict.txt","w") as rt:
        rt.write(output)

def pair_predict(drug_name,drug_smi,target_name,target_fas,tag):
    drug_smi = dg_seq2num(drug_smi)
    dg_input = []
    dg_input.append(drug_smi)
    dg_input.append(drug_smi)
    print("dg_input", dg_input)
    dg_input = np.array(dg_input)
    dg_input = torch.LongTensor(dg_input)
    print("dg_input", dg_input)

    target_fas = tg_seq2sum(target_fas)
    tg_input = []
    tg_input.append(target_fas)
    tg_input.append(target_fas)
    tg_input = np.array(tg_input)
    tg_input = torch.LongTensor(tg_input)

    with torch.no_grad():
        dg_common, tg_common= com_model(dg_input,tg_input)
        predict_rate,tag_p = pre_model(dg_common,tg_common,tag)
        # print(drug_name,target_name,tag,predict_rate[0].item())
    return drug_name, target_name, predict_rate[0].item(), tag

def final_process(num):
    out_put = "drug_name,uniprot,y_pre,y_label\n"
    out_set = data_process()
    for i in out_set:
        # print(i[0],i[2],i[1],i[3],i[4])
        drug_name, target_name, predic, label = pair_predict(i[0],i[2],i[1],i[3],i[4])
        out_put = out_put + drug_name + "," + target_name + "," + str(predic) + "," + str(label) + "\n"
    with open("./results/result_" + num + "/outtest_pre_lab.csv", "w") as f:
        f.write(out_put)

def main():
    num = str(sys.argv[1])
    # out_set = data_process()
    com_parm, pre_parm = load_mod(num)
    com_model.load_state_dict(com_parm)
    pre_model.load_state_dict(pre_parm)
    com_model.eval()
    pre_model.eval()
    final_process(num)


if __name__=="__main__":
    main()
```

### 使用外部测试集进行预测

```python
import os, sys
import torch
import numpy as np
import pandas as pd
import pickle
from collections import defaultdict

from config import Config
from utils.util import Helper
from model.CommonModel import Common_model
from model.PredictModel import Predict_model
from dataset import HetrDataset


"""
这个文件的功能：
靶标预测及虚拟筛选
"""

def smiles_read(smiles):
    with open(smiles) as f:
        f1 = f.readlines()
    dict_icv_drug = {}
    for line in f1:
        dict_icv_drug[line.split(",")[0]] = line.split(",")[1].strip()
        #break
    #print(dict_icv_drug_encoding)
    return dict_icv_drug

def protein_read(protein):
    with open(protein) as f:
        f1 = f.readlines()
    dict_target_protein = {}
    for line in f1:
        dict_target_protein[line.split(",")[0]] = line.split(",")[1].strip()
    # print(len(dict_target_protein_encoding["P0C6X7_3C"]))
    # print(len(f1[0]))
    return dict_target_protein

def data_process():
    out_dict_drug = smiles_read("./outtest/drug_smiles.txt")
    out_dict_protein = protein_read("./outtest/protein_fasta.txt")
    target_set = list(out_dict_protein.keys())
    with open("./outtest/drug_protein.txt") as f:
        f1 = f.readlines()
    dataset = []
    for i in range(39):
        dataset.append([f1[i].split(",")[0],f1[i].split(",")[1].strip(),out_dict_drug[f1[i].split(",")[0]],out_dict_protein[f1[i].split(",")[1].strip()],1])
    for i in range(39,78):
        dataset.append([f1[i].split(",")[0],f1[i].split(",")[1].strip(),out_dict_drug[f1[i].split(",")[0]],out_dict_protein[f1[i].split(",")[1].strip()],0])
    #dti_mutidict = defaultdict(list)
    #[dti_mutidict[i.split(",")[0]].append(i.split(",")[1].strip()) for i in f1]
    #whole_positive = []
    #whole_negetive = []
    #for key in dti_mutidict:
    #    for i in dti_mutidict[key]:
    #        whole_positive.append([key,i,out_dict_drug[key],out_dict_protein[i],1])
    #    target_no = target_set[:]
    #    [target_no.remove(i) for i in dti_mutidict[key]]
    #    for a in target_no:
    #        whole_negetive.append([key,a,out_dict_drug[key],out_dict_protein[a],0])
    #whole_positive = np.array(whole_positive,dtype=object)
    #whole_negetive = np.array(whole_negetive,dtype=object)
    #data_set = np.vstack((whole_positive,whole_negetive))
    return dataset

with open('./data/smi_dict.pickle',"rb") as f:
    smi_dict = pickle.load(f)
with open('data/fas_dict.pickle',"rb") as f:
    fas_dict = pickle.load(f)

def load_mod(num):
    #导入字典

    #导入异构数据集在公共空间的表示,并转换为tensor格式
    target_com = pd.read_csv('./results/result_' + num + '/target_com_' + num + '.csv',index_col=0,header=0)
    #print(target_com)
    target_index = target_com._stat_axis.values
    target_com = torch.FloatTensor(target_com.values)
    #print(target_com)

    #导入模型参数
    com_parm = torch.load('./results/com_model_parm/repeat_corss_' + num + '.parm')
    pre_parm = torch.load('./results/pre_model_parm/repeat_corss_' + num + '.parm')
    #print(com_parm)
    #print(pre_parm)
    return com_parm, pre_parm

# com_parm, pre_parm = load_mod(num)
config = Config()
helper = Helper()
com_model = Common_model(config)
pre_model = Predict_model()
dataset = HetrDataset()

this_use_gpu = False
smi_input = dataset.smi_input
fas_input = dataset.fas_input
smi_input = helper.to_longtensor(smi_input,this_use_gpu)
fas_input = helper.to_longtensor(fas_input,this_use_gpu)

# com_model.load_state_dict(com_parm)
# pre_model.load_state_dict(pre_parm)
# com_model.eval()
# pre_model.eval()

def transmol_linux(in_name,out_name):
    in_suffix=os.path.splitext(in_name)[1][1:]
    out_suffix=os.path.splitext(out_name)[1][1:]
    if out_suffix == "smi":
        command="obabel -i%s %s -ocan -O %s"%(in_suffix,in_name,out_name)
    else:
        command="obabel -i%s %s -o%s -O %s"%(in_suffix,in_name,out_suffix,out_name)
    os.system(command)

def dg_seq2num(smi):
    #将smiles序列通过字典的映射，转为数值表示
    output = np.zeros(config.smiles_max_len, dtype=int)

    for i in range(len(smi) - config.smi_n_gram + 1):
        key = smi[i:i + config.smi_n_gram]
        output[i] = smi_dict[key]
    return output

def tg_seq2sum(fas):
    # 将smiles序列通过字典的映射，转为数值表示
    output = np.zeros(config.fasta_max_len, dtype=int)

    for j in range(len(fas) - config.fas_n_gram + 1):
        key = fas[j:j + config.fas_n_gram]
        output[j] = fas_dict[key]
    return output

def predict_(smiless):
    temp = seq2num(smiless)
    input = []
    input.append(temp)
    input.append(temp)
    input = np.array(input)
    input = torch.LongTensor(input)
    with torch.no_grad():
        input_common, _= com_model(input,fas_input[0:2])
    tag = 1       #这个可以随便设定
    target_predict = dict()
    with torch.no_grad():
        for i in range(len(target_com)):
            predict_rate,tag = pre_model(input_common,target_com[i],tag)  #predict_rate就表示他们之间有相互作用的可能性大小
            target_predict[target_index[i]] = predict_rate[0].item()      #针对单个数据，将tensor转为python数据类型，即把tensor去掉
    return target_predict

# #异构网络的节点
# temp_name = "DB00385"
# temp = "c1cc(c2c(c1)C(=O)c1c(C2=O)c(O)c2c(c1O)C[C@](C[C@@H]2O[C@H]1C[C@@H]([C@@H]([C@@H](O1)C)O)NC(=O)C(F)(F)F)(C(=O)COC(=O)CCCC)O)OC"

# temp = seq2num(temp)
# input = []
# input.append(temp)
# input.append(temp)
# input = np.array(input)
# input = torch.LongTensor(input)
# #print(input)
# #得到这个药物在公共空间的表示，不管是原本就在公共空间内的，还是不在公共空间内的drug
# with torch.no_grad():
#     input_common, _= com_model(input,fas_input[0:2])
#     #print(input_common)

# #进行靶标预测,因为输入需要2个样本，所以这里就直接将input_common直接输入，得到2个相同的结果
# tag = 1       #这个可以随便设定
# target_predict = dict()
# with torch.no_grad():

#     for i in range(len(target_com)):
#         predict_rate,tag = pre_model(input_common,target_com[i],tag)  #predict_rate就表示他们之间有相互作用的可能性大小
#         target_predict[target_index[i]] = predict_rate[0].item()      #针对单个数据，将tensor转为python数据类型，即把tensor去掉

#     # print(target_predict)
#     # sort_predict = sorted(target_predict.items(),key= lambda x:x[1],reverse=True)   #reverse默认为False，按从小到大排序
#     # print(sort_predict)

def read_all_target():
    with open("proname_uniprot.txt") as f:
        f1 = f.readlines()
    uni_pro = {}
    for line in f1:
        uni_pro[line.split(",")[0]] = line.strip().split(",")[1]
    return uni_pro

def result_process(mol_name, uni_pro, target_predict_dict):
    output = "drug,target,possibility\n"
    for key in target_predict_dict:
        output = output + mol_name + "," + uni_pro[key] + "," + str(target_predict_dict[key]) + "\n"
    if not os.path.exists('./result_output'):
        os.mkdir('./result_output')
    with open('./result_output' + "/target_predict.txt","w") as rt:
        rt.write(output)

def pair_predict(drug_name,drug_smi,target_name,target_fas,tag):
    drug_smi = dg_seq2num(drug_smi)
    dg_input = []
    dg_input.append(drug_smi)
    dg_input.append(drug_smi)
    dg_input = np.array(dg_input)
    dg_input = torch.LongTensor(dg_input)

    target_fas = tg_seq2sum(target_fas)
    tg_input = []
    tg_input.append(target_fas)
    tg_input.append(target_fas)
    tg_input = np.array(tg_input)
    tg_input = torch.LongTensor(tg_input)

    with torch.no_grad():
        dg_common, tg_common= com_model(dg_input,tg_input)
        predict_rate,tag_p = pre_model(dg_common,tg_common,tag)
        # print(drug_name,target_name,tag,predict_rate[0].item())
    return drug_name, target_name, predict_rate[0].item(), tag

def final_process(num):
    out_put = "drug_name,uniprot,y_pre,y_label\n"
    out_set = data_process()
    for i in out_set:
        # print(i[0],i[2],i[1],i[3],i[4])
        drug_name, target_name, predic, label = pair_predict(i[0],i[2],i[1],i[3],i[4])
        out_put = out_put + drug_name + "," + target_name + "," + str(predic) + "," + str(label) + "\n"
    with open("./results/result_" + num + "/outtest_pre_lab.csv", "w") as f:
        f.write(out_put)

def main():
    num = str(sys.argv[1])
    # out_set = data_process()
    com_parm, pre_parm = load_mod(num)
    com_model.load_state_dict(com_parm)
    pre_model.load_state_dict(pre_parm)
    com_model.eval()
    pre_model.eval()
    final_process(num)


if __name__=="__main__":
    main()
```

## 网站接口

说明：multiDTI没有包装的很好，需要的文件比较繁杂，要确保文件配置好方能正常运行。

首先需要获取药物和靶标在空间中的表示并存储下来，留作后用。其次则是预测。

```python
import os, sys
import torch
import numpy as np
import pandas as pd
import pickle

from utils.config import Config
from utils.util import Helper
from utils.CommonModel import Common_model
from utils.PredictModel import Predict_model
from utils.dataset import HetrDataset

"""
这个文件的功能：
靶标预测及虚拟筛选
"""
os.environ["CUDA_VISIBLE_DEVICES"] = '1'
path = "/home/yqyang/D3Deep/target_predict_and_vs/target_predict/common_space/main_file/"

#导入字典
with open(path + 'smi_dict.pickle',"rb") as f:
    smi_dict = pickle.load(f)

#导入异构数据集在公共空间的表示,并转换为tensor格式
target_com = pd.read_csv(path + 'target_com_all.csv',index_col=0,header=0)
#print(target_com)
target_index = target_com._stat_axis.values
target_com = torch.FloatTensor(target_com.values)
#print(target_com)

#导入模型参数
com_parm = torch.load(path + 'repeat_0_corss_2_com.parm')
pre_parm = torch.load(path + 'repeat_0_corss_2_pre.parm')
#print(com_parm)
#print(pre_parm)

config = Config()
helper = Helper()
com_model = Common_model(config)
pre_model = Predict_model()
dataset = HetrDataset()

this_use_gpu = False
smi_input = dataset.smi_input
fas_input = dataset.fas_input
smi_input = helper.to_longtensor(smi_input,this_use_gpu)
fas_input = helper.to_longtensor(fas_input,this_use_gpu)

com_model.load_state_dict(com_parm)
pre_model.load_state_dict(pre_parm)
com_model.eval()
pre_model.eval()

def transmol_linux(in_name,out_name):
    in_suffix=os.path.splitext(in_name)[1][1:]
    out_suffix=os.path.splitext(out_name)[1][1:]
    if out_suffix == "smi":
        command="obabel -i%s %s -ocan -O %s"%(in_suffix,in_name,out_name)
    else:
        command="obabel -i%s %s -o%s -O %s"%(in_suffix,in_name,out_suffix,out_name)
    os.system(command)

def seq2num(smi):
    #将smiles序列通过字典的映射，转为数值表示
    output = np.zeros(config.smiles_max_len, dtype=int)

    for i in range(len(smi) - config.smi_n_gram + 1):
        key = smi[i:i + config.smi_n_gram]
        output[i] = smi_dict[key]
    return output

def predict_(smiless):
    temp = seq2num(smiless)
    input = []
    input.append(temp)
    input.append(temp)
    input = np.array(input)
    input = torch.LongTensor(input)
    with torch.no_grad():
        input_common, _= com_model(input,fas_input[0:2])
    tag = 1       #这个可以随便设定
    target_predict = dict()
    with torch.no_grad():
        for i in range(len(target_com)):
            predict_rate,tag = pre_model(input_common,target_com[i],tag)  #predict_rate就表示他们之间有相互作用的可能性大小
            target_predict[target_index[i]] = predict_rate[0].item()      #针对单个数据，将tensor转为python数据类型，即把tensor去掉
    return target_predict

# #异构网络的节点
# temp_name = "DB00385"
# temp = "c1cc(c2c(c1)C(=O)c1c(C2=O)c(O)c2c(c1O)C[C@](C[C@@H]2O[C@H]1C[C@@H]([C@@H]([C@@H](O1)C)O)NC(=O)C(F)(F)F)(C(=O)COC(=O)CCCC)O)OC"

# temp = seq2num(temp)
# input = []
# input.append(temp)
# input.append(temp)
# input = np.array(input)
# input = torch.LongTensor(input)
# #print(input)
# #得到这个药物在公共空间的表示，不管是原本就在公共空间内的，还是不在公共空间内的drug
# with torch.no_grad():
#     input_common, _= com_model(input,fas_input[0:2])
#     #print(input_common)

# #进行靶标预测,因为输入需要2个样本，所以这里就直接将input_common直接输入，得到2个相同的结果
# tag = 1       #这个可以随便设定
# target_predict = dict()
# with torch.no_grad():

#     for i in range(len(target_com)):
#         predict_rate,tag = pre_model(input_common,target_com[i],tag)  #predict_rate就表示他们之间有相互作用的可能性大小
#         target_predict[target_index[i]] = predict_rate[0].item()      #针对单个数据，将tensor转为python数据类型，即把tensor去掉

#     # print(target_predict)
#     # sort_predict = sorted(target_predict.items(),key= lambda x:x[1],reverse=True)   #reverse默认为False，按从小到大排序
#     # print(sort_predict)

def read_all_target():
    with open(path + "proname_uniprot.txt") as f:
        f1 = f.readlines()
    uni_pro = {}
    for line in f1:
        uni_pro[line.split(",")[0]] = line.strip().split(",")[1]
    return uni_pro

def result_process(mol_name, uni_pro, target_predict_dict):
    output = "drug,target,possibility\n"
    for key in target_predict_dict:
        output = output + mol_name + "," + uni_pro[key] + "," + str(target_predict_dict[key]) + "\n"
    if not os.path.exists('./result_output'):
        os.mkdir('./result_output')
    with open('./result_output' + "/target_predict.txt","w") as rt:
        rt.write(output)

def main():
    mol_file = str(sys.argv[1])
    mol_name = os.path.splitext(mol_file)[0]
    transmol_linux(mol_file, mol_name + ".smi")
    with open(mol_name + ".smi") as f:
        smiles = f.readline().strip().split("\t")[0]
    # target_name, target_fasta = read_target()
    # target_predict(smiles, mol_name, target_name, target_fasta)
    print(smiles)
    target_predict_dict = predict_(smiles)
    uni_pro = read_all_target()
    result_process(mol_name, uni_pro, target_predict_dict)

if __name__=="__main__":
    main()
```

## 数据分析

（1）绘制loss曲线

参考：[Python绘图|Python绘制多曲线图](./Python绘图Python绘制多曲线图.md)

（2）绘制ROC和PR曲线

参考：[Python绘图|Python绘制ROC曲线和PR曲线](./Python绘图Python绘制ROC曲线和PR曲线.md)

（3）计算AUC、AUPRC、Acc、Pre、F1等指标

参考：

## 其他

### 续跑

### 多GPU训练、预测

#### 训练

```python
import time
import datetime
import os
import copy

import numpy as np
import torch.optim as optim
import torch
from torch import nn
from torch.autograd import Variable
from torch.nn import functional as F
from sklearn.metrics import roc_auc_score
from sklearn.metrics import average_precision_score

from config import Config
from utils.util import Helper
from MPNN.CommonModel import Common_model
from MPNN.PredictModel import Predict_model
from dataset import HetrDataset

device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')

def train_common_model(config,helper,model,hetrdataset,repeat_nums,flod_nums):

    optimizer = optim.Adam(model.parameters(),config.common_learn_rate)
    model.train()

    print("common model begin training----------",datetime.datetime.now())

    #common_loss
    for e in range(config.common_epochs):

        common_loss = 0
        begin_time = time.time()

        for i, (dg,pt,tag) in enumerate(hetrdataset.get_train_batch(repeat_nums,flod_nums,config.batch_size)):
            #dg = helper.to_longtensor(dg,config.use_gpu)
            pt = helper.to_longtensor(pt,config.use_gpu)
            tag = helper.to_floattensor(tag,config.use_gpu)

            optimizer.zero_grad()

            smi_common, fas_common = model(dg,pt)

            distance_loss = helper.comput_distance_loss(smi_common,fas_common,tag)
            common_loss += distance_loss

            distance_loss.backward()
            optimizer.step()

        #end a epech
        print("the loss of common model epoch[%d / %d]:is %4.f, time:%d s" % (e+1,config.common_epochs,common_loss,time.time()-begin_time))
    return common_loss.item()

def train_predict_model(config,helper,predict_model,common_model,hetrdataset,repeat_nums,flod_nums):

    optimizer1 = optim.Adam(predict_model.parameters(),config.pre_learn_rate)
    optimizer2 = optim.Adam(common_model.parameters(),config.common_learn_rate)

    predict_model.train()
    common_model.train()

    print("predict model begin training----------",datetime.datetime.now())

    #tag_loss
    for e in range(config.predict_epochs):

        max_auc = 0
        epoch_loss = 0
        begin_time = time.time()

        for i, (dg,pt,tag) in enumerate(hetrdataset.get_train_batch(repeat_nums,flod_nums,config.batch_size)):
            #dg = helper.to_longtensor(dg,config.use_gpu)
            pt = helper.to_longtensor(pt,config.use_gpu)
            tag = helper.to_floattensor(tag,config.use_gpu)

            smi_common,fas_common = common_model(dg,pt)

            optimizer1.zero_grad()
            optimizer2.zero_grad()

            predict, tag  = predict_model(smi_common,fas_common,tag)

            tag_loss = F.binary_cross_entropy(predict,tag)
            epoch_loss += tag_loss

            tag_loss.backward()

            optimizer1.step()
            optimizer2.step()

        _, _, _, auc = evaluation_model(config, helper, predict_model, common_model, hetrdataset, repeat_nums, flod_nums)

        if auc > max_auc:
            model_max_p = copy.deepcopy(predict_model)
            model_max_c = copy.deepcopy(common_model)
            max_auc = auc

        # end a epech
        print("the loss of predict model epoch[%d / %d]:is %4.f, time:%d s" %  (e+1, config.predict_epochs, epoch_loss, time.time() - begin_time))

    # #将每一折交叉验证的模型存储起来
    # if not os.path.exists('./results'):
    #     os.mkdir('./results')
    # if not os.path.exists('./results/com_model_parm'):
    #     os.mkdir('./results/com_model_parm')
    # if not os.path.exists('./results/pre_model_parm'):
    #     os.mkdir('./results/pre_model_parm')

    #evaluation_model(config, helper, predict_model, common_model, hetrdataset, repeat_nums, flod_nums)
    #if e == config.predict_epochs-1 and epoch == config.num_epochs-1:
    # torch.save(common_model.state_dict(),
    #            './results/com_model_parm/repeat_%d_corss_%d.parm' % (repeat_nums, flod_nums))
    # torch.save(model_max.state_dict(),
    #            './results/pre_model_parm/repeat_%d_corss_%d.parm' % (repeat_nums, flod_nums))
    #评估模型

    loss, pre_all, lab_all, _ = evaluation_model(config, helper, model_max_p, model_max_c, hetrdataset, repeat_nums, flod_nums)

    return epoch_loss.item(), loss, pre_all, lab_all, model_max_p, model_max_c

def evaluation_model(config,helper,predict_model,common_model,hetrdataset,repeat_nums,flod_nums):
    predict_model.eval()
    common_model.eval()
    print("evaluate the model")

    begin_time = time.time()
    loss = 0
    avg_acc = []
    avg_aupr = []
    pre_all = []
    lab_all = []
    with torch.no_grad():
        for i,(dg,pt,tag) in enumerate(hetrdataset.get_test_batch(repeat_nums,flod_nums,config.batch_size)):
            #dg = helper.to_longtensor(dg,config.use_gpu)
            pt = helper.to_longtensor(pt,config.use_gpu)
            tag = helper.to_floattensor(tag,config.use_gpu)

            smi_common,fas_common = common_model(dg,pt)
            predict, tag = predict_model(smi_common, fas_common, tag)

            pre_all.append(predict)
            lab_all.append(tag)

            tag_loss = F.binary_cross_entropy(predict,tag)
            loss +=tag_loss

            auc = roc_auc_score(tag.cpu(),predict.cpu())
            aupr = average_precision_score(tag.cpu(),predict.cpu())

            avg_acc.append(auc)
            avg_aupr.append(aupr)

    print("the total_loss of test model:is %4.f, time:%d s" % (loss, time.time() - begin_time))
    print("avg_acc:",np.mean(avg_acc),"avg_aupr:",np.mean(avg_aupr))

    return loss.item(), pre_all, lab_all, np.mean(avg_acc)

if __name__=='__main__':

    # initial parameters class
    config = Config()

    # initial utils class
    helper = Helper()

    #initial data
    hetrdataset = HetrDataset()

    #torch.backends.cudnn.enabled = False 把

    model_begin_time = time.time()
    for i in range(config.repeat_nums):
        print("repeat:",str(i),"+++++++++++++++++++++++++++++++++++")
        for j in range(config.fold_nums):
            print(" crossfold:", str(j), "----------------------------")
            if not os.path.exists('./results/result_' + str(j) + "/"):
                os.makedirs('./results/result_' + str(j) + "/")
            #initial presentation model
            c_model = Common_model(config)
            p_model = Predict_model()
            if config.use_gpu:
                if torch.cuda.device_count() > 1:
                    #c_model = c_model.cuda()
                    #p_model = p_model.cuda()
                    c_model = c_model.to(device)
                    p_model = p_model.to(device)
                    c_model = nn.DataParallel(c_model, dim = 0)
                    p_model = nn.DataParallel(p_model, dim = 0)

            loss_rt = "model_loss,predict_loss,val_loss\n"
            maxx_auc = 0
            for epoch in range(config.num_epochs):
                print("         epoch:",str(epoch),"zzzzzzzzzzzzzzzz")
                common_loss = train_common_model(config,helper,c_model,hetrdataset,i,j)
                predict_loss, val_loss, pre_all, lab_all, pred_model, com_model = train_predict_model(config,helper,p_model,c_model,hetrdataset,i,j)
                if not os.path.exists('./results'):
                    os.mkdir('./results')
                if not os.path.exists('./results/com_model_parm'):
                    os.mkdir('./results/com_model_parm')
                if not os.path.exists('./results/pre_model_parm'):
                    os.mkdir('./results/pre_model_parm')
                _, _, _, auc = evaluation_model(config, helper, pred_model, com_model, hetrdataset, i, j)
                if auc > maxx_auc:
                    model_train_max = copy.deepcopy(com_model)
                    model_pre_max = copy.deepcopy(pred_model)
                    max_auc = auc
                loss_rt = loss_rt + str(common_loss) + "," + str(predict_loss) + "," + str(val_loss) + "\n"
                print(str(common_loss) + "," + str(predict_loss) + "," + str(val_loss) + "\n")
            torch.save(model_train_max.state_dict(),
                       './results/com_model_parm/repeat_%d_corss_%d.parm' % (i, j))
            torch.save(model_pre_max.state_dict(),
                       './results/pre_model_parm/repeat_%d_corss_%d.parm' % (i, j))
            with open('./results/result_' + str(j) + "/" + "all_loss_" + str(j) + ".csv","w") as rt:
                rt.write(loss_rt)
            predict_loss, val_loss, pre_all, lab_all, _, _ = train_predict_model(config,helper,model_pre_max,model_train_max,hetrdataset,i,j)
            pre_lab = "y_pre,y_lab\n"
            # for i in range(len(pre_all)):
            #     pre_lab = pre_lab + str(pre_all[i]) + "," + str(lab_all[i]) + "\n"
            for qq in range(len(pre_all)):
                a = pre_all[qq].tolist()
                b = lab_all[qq].tolist()
                for m in range(len(a)):
                    pre_lab = pre_lab + str(a[m]) + "," + str(b[m]) + "\n"
            with open('./results/result_' + str(j) + "/" + "pre_lab_" + str(j) + ".csv","w") as rt:
                rt.write(pre_lab)

            # with open('./results/result_' + str(j) + "/" + "all_loss_" + str(j) + ".csv","w") as rt:
            #     rt.write("model_loss,predict_loss,val_loss\n")
            #     for epoch in range(config.num_epochs):
            #         print("         epoch:",str(epoch),"zzzzzzzzzzzzzzzz")
            #         common_loss = train_common_model(config,helper,c_model,hetrdataset,i,j)
            #         predict_loss, val_loss, pre_all, lab_all = train_predict_model(config,helper,p_model,c_model,hetrdataset,i,j)
            #         rt.write(str(common_loss) + "," + str(predict_loss) + "," + str(val_loss) + "\n")
            #         print(str(common_loss) + "," + str(predict_loss) + "," + str(val_loss) + "\n")
            #     pre_lab = "y_pre,y_lab\n"
            #     for i in range(len(pre_all)):
            #         a = pre_all[i].tolist()
            #         b = lab_all[i].tolist()
            #         for m in range(len(a)):
            #             pre_lab = pre_lab + str(a[m]) + "," + str(b[m]) + "\n"
            #     with open('./results/result_' + str(j) + "/" + "pre_lab_" + str(j) + ".csv","w") as rt:
            #         rt.write(pre_lab)


    print("Done!")
    print("All_training time:",time.time()-model_begin_time)
```

#### 预测

```python
import os, sys
import torch
import numpy as np
import pandas as pd
import pickle
from collections import defaultdict

from config import Config
from utils.util import Helper
from MPNN.CommonModel import Common_model
from MPNN.PredictModel import Predict_model
from dataset import HetrDataset

from MPNN.utils import *

"""
这个文件的功能：
靶标预测及虚拟筛选
"""

config = Config()
helper = Helper()
com_model = Common_model(config)
pre_model = Predict_model()
dataset = HetrDataset()

def smiles_read(smiles):
    with open(smiles) as f:
        f1 = f.readlines()
    dict_icv_drug = {}
    for line in f1:
        dict_icv_drug[line.split(",")[0]] = line.split(",")[1].strip()
        #break
    #print(dict_icv_drug_encoding)
    return dict_icv_drug

def protein_read(protein):
    with open(protein) as f:
        f1 = f.readlines()
    dict_target_protein = {}
    for line in f1:
        dict_target_protein[line.split(",")[0]] = line.split(",")[1].strip()
    # print(len(dict_target_protein_encoding["P0C6X7_3C"]))
    # print(len(f1[0]))
    return dict_target_protein

def data_process():
    out_dict_drug = smiles_read("./outtest/drug_smiles.txt")
    out_dict_protein = protein_read("./outtest/protein_fasta.txt")
    target_set = list(out_dict_protein.keys())
    with open("./outtest/drug_protein.txt") as f:
        f1 = f.readlines()
    dti_mutidict = defaultdict(list)
    [dti_mutidict[i.split(",")[0]].append(i.split(",")[1].strip()) for i in f1]
    whole_positive = []
    whole_negetive = []
    for key in dti_mutidict:
        for i in dti_mutidict[key]:
            whole_positive.append([key,i,out_dict_drug[key],out_dict_protein[i],1])
        target_no = target_set[:]
        [target_no.remove(i) for i in dti_mutidict[key]]
        for a in target_no:
            whole_negetive.append([key,a,out_dict_drug[key],out_dict_protein[a],0])
    whole_positive = np.array(whole_positive,dtype=object)
    whole_negetive = np.array(whole_negetive,dtype=object)
    data_set = np.vstack((whole_positive,whole_negetive))
    return data_set

# with open('./data/smi_dict.pickle',"rb") as f:
#     smi_dict = pickle.load(f)
with open('data/fas_dict.pickle',"rb") as f:
    fas_dict = pickle.load(f)

def load_mod(num):
    #导入字典

    #导入异构数据集在公共空间的表示,并转换为tensor格式
    target_com = pd.read_csv('./results/result_' + num + '/target_com_' + num + '.csv',index_col=0,header=0)
    #print(target_com)
    target_index = target_com._stat_axis.values
    target_com = torch.FloatTensor(target_com.values)
    #print(target_com)

    #导入模型参数
    com_parm = torch.load('./results/com_model_parm/repeat_0_corss_' + num + '.parm')

    if next(iter(com_parm))[:7] == 'module.':
        # the pretrained model is from data-parallel module
        from collections import OrderedDict
        new_state_dict = OrderedDict()
        for k, v in com_parm.items():
            name = k[7:] # remove `module.`
            new_state_dict[name] = v
        com_parm = new_state_dict

    pre_parm = torch.load('./results/pre_model_parm/repeat_0_corss_' + num + '.parm')

    if next(iter(pre_parm))[:7] == 'module.':
        # the pretrained model is from data-parallel module
        from collections import OrderedDict
        new_state_dict = OrderedDict()
        for k, v in pre_parm.items():
            name = k[7:] # remove `module.`
            new_state_dict[name] = v
        pre_parm = new_state_dict

    #print(com_parm)
    #print(pre_parm)
    return com_parm, pre_parm

# com_parm, pre_parm = load_mod(num)
# config = Config()
# helper = Helper()
# com_model = Common_model(config)
# pre_model = Predict_model()
# dataset = HetrDataset()

this_use_gpu = True
# smi_input = dataset.smi_input
fas_input = dataset.fas_input
# smi_input = helper.to_longtensor(smi_input,this_use_gpu)
fas_input = helper.to_longtensor(fas_input,this_use_gpu)

# com_model.load_state_dict(com_parm)
# pre_model.load_state_dict(pre_parm)
# com_model.eval()
# pre_model.eval()

def transmol_linux(in_name,out_name):
    in_suffix=os.path.splitext(in_name)[1][1:]
    out_suffix=os.path.splitext(out_name)[1][1:]
    if out_suffix == "smi":
        command="obabel -i%s %s -ocan -O %s"%(in_suffix,in_name,out_name)
    else:
        command="obabel -i%s %s -o%s -O %s"%(in_suffix,in_name,out_suffix,out_name)
    os.system(command)

def dg_seq2num(smi):
    #将smiles序列通过字典的映射，转为数值表示
    # output = np.zeros(config.smiles_max_len, dtype=int)

    # for i in range(len(smi) - config.smi_n_gram + 1):
    #     key = smi[i:i + config.smi_n_gram]
    #     output[i] = smi_dict[key]
    fatoms = []
    fbonds = []
    agraph = []
    bgraph = []
    shape_tensor = []

    [a, b, c, d, e] = smiles2mpnnfeature(smi)
    fatoms.append(a)
    fbonds.append(b)
    agraph.append(c)
    bgraph.append(d)
    shape_tensor.append(e)

    v_d = []
    v_d.append(torch.stack(fatoms))
    v_d.append(torch.stack(fbonds))
    v_d.append(torch.stack(agraph))
    v_d.append(torch.stack(bgraph))
    v_d.append(torch.stack(shape_tensor).squeeze(1))
    return v_d

def tg_seq2sum(fas):
    # 将smiles序列通过字典的映射，转为数值表示
    output = np.zeros(config.fasta_max_len, dtype=int)

    for j in range(len(fas) - config.fas_n_gram + 1):
        key = fas[j:j + config.fas_n_gram]
        output[j] = fas_dict[key]
    return output

def predict_(smiless):
    temp = seq2num(smiless)
    input = []
    input.append(temp)
    input.append(temp)
    input = np.array(input)
    input = torch.LongTensor(input)
    with torch.no_grad():
        input_common, _= com_model(input,fas_input[0:2])
    tag = 1       #这个可以随便设定
    target_predict = dict()
    with torch.no_grad():
        for i in range(len(target_com)):
            predict_rate,tag = pre_model(input_common,target_com[i],tag)  #predict_rate就表示他们之间有相互作用的可能性大小
            target_predict[target_index[i]] = predict_rate[0].item()      #针对单个数据，将tensor转为python数据类型，即把tensor去掉
    return target_predict

# #异构网络的节点
# temp_name = "DB00385"
# temp = "c1cc(c2c(c1)C(=O)c1c(C2=O)c(O)c2c(c1O)C[C@](C[C@@H]2O[C@H]1C[C@@H]([C@@H]([C@@H](O1)C)O)NC(=O)C(F)(F)F)(C(=O)COC(=O)CCCC)O)OC"

# temp = seq2num(temp)
# input = []
# input.append(temp)
# input.append(temp)
# input = np.array(input)
# input = torch.LongTensor(input)
# #print(input)
# #得到这个药物在公共空间的表示，不管是原本就在公共空间内的，还是不在公共空间内的drug
# with torch.no_grad():
#     input_common, _= com_model(input,fas_input[0:2])
#     #print(input_common)

# #进行靶标预测,因为输入需要2个样本，所以这里就直接将input_common直接输入，得到2个相同的结果
# tag = 1       #这个可以随便设定
# target_predict = dict()
# with torch.no_grad():

#     for i in range(len(target_com)):
#         predict_rate,tag = pre_model(input_common,target_com[i],tag)  #predict_rate就表示他们之间有相互作用的可能性大小
#         target_predict[target_index[i]] = predict_rate[0].item()      #针对单个数据，将tensor转为python数据类型，即把tensor去掉

#     # print(target_predict)
#     # sort_predict = sorted(target_predict.items(),key= lambda x:x[1],reverse=True)   #reverse默认为False，按从小到大排序
#     # print(sort_predict)

def read_all_target():
    with open("proname_uniprot.txt") as f:
        f1 = f.readlines()
    uni_pro = {}
    for line in f1:
        uni_pro[line.split(",")[0]] = line.strip().split(",")[1]
    return uni_pro

def result_process(mol_name, uni_pro, target_predict_dict):
    output = "drug,target,possibility\n"
    for key in target_predict_dict:
        output = output + mol_name + "," + uni_pro[key] + "," + str(target_predict_dict[key]) + "\n"
    if not os.path.exists('./result_output'):
        os.mkdir('./result_output')
    with open('./result_output' + "/target_predict.txt","w") as rt:
        rt.write(output)

def pair_predict(drug_name,drug_smi,target_name,target_fas,tag):
    # drug_smi = dg_seq2num(drug_smi)
    # dg_input = []
    # dg_input.append(drug_smi)
    # dg_input.append(drug_smi)
    # dg_input = np.array(dg_input)
    # dg_input = torch.LongTensor(dg_input)
    fatoms = []
    fbonds = []
    agraph = []
    bgraph = []
    shape_tensor = []
    for i in range(2):
        [a, b, c, d, e] = smiles2mpnnfeature(drug_smi)
        fatoms.append(a)
        fbonds.append(b)
        agraph.append(c)
        bgraph.append(d)
        shape_tensor.append(e)

    v_d = []
    v_d.append(torch.stack(fatoms))
    v_d.append(torch.stack(fbonds))
    v_d.append(torch.stack(agraph))
    v_d.append(torch.stack(bgraph))
    v_d.append(torch.stack(shape_tensor).squeeze(1))
    dg_input = v_d

    target_fas = tg_seq2sum(target_fas)
    tg_input = []
    tg_input.append(target_fas)
    tg_input.append(target_fas)
    tg_input = np.array(tg_input)
    tg_input = torch.LongTensor(tg_input).to("cuda")

    with torch.no_grad():
        dg_common, tg_common= com_model(dg_input,tg_input)
        predict_rate,tag_p = pre_model(dg_common,tg_common,tag)
        # print(drug_name,target_name,tag,predict_rate[0].item())
    return drug_name, target_name, predict_rate[0].item(), tag

def final_process(num):
    out_put = "drug_name,uniprot,y_pre,y_label\n"
    out_set = data_process()
    for i in out_set:
        # print(i[0],i[2],i[1],i[3],i[4])
        drug_name, target_name, predic, label = pair_predict(i[0],i[2],i[1],i[3],i[4])
        out_put = out_put + drug_name + "," + target_name + "," + str(predic) + "," + str(label) + "\n"
    with open("./results/result_" + num + "/outtest_pre_lab.csv", "w") as f:
        f.write(out_put)

def main():
    num = str(sys.argv[1])
    global com_model, pre_model
    # out_set = data_process()
    com_parm, pre_parm = load_mod(num)
    com_model.load_state_dict(com_parm)
    pre_model.load_state_dict(pre_parm)
    com_model = com_model.to("cuda")
    pre_model = pre_model.to("cuda")
    com_model.eval()
    pre_model.eval()
    final_process(num)


if __name__=="__main__":
    main()
```

---

## 一些bug

