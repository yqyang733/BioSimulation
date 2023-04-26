👏 工具|药物设计预测相关的深度学习库-DeepPurpose

---

[TOC]

---

## 简介

文章：[DeepPurpose: a deep learning library for drug–target interaction prediction](https://doi.org/10.1093/bioinformatics/btaa1005)

[github](https://github.com/1512474508/DeepPurpose)

DeepPurpose是一个发表在 Bioinformatics 上的药物设计相关深度学习库。DeepPurpose对多种经典算法如分子指纹以及当下流行的多种深度学习算法，如CNN, RNN, Transformer等进行了包装，非常适合新手学习。可用于预测药物靶标相互作用、蛋白蛋白相互作用、药物药物相互作用、蛋白性质预测、药物性质预测等。

---

## 安装

一般采用佛系安装策略：将需要用的包直接下载下来并且在Anaconda中新建一个环境。把输入数据准备好之后，运行脚本，根据报错结果，少哪个包直接 conda install/pip install 安装即可。直到能正常运行。当然也可以按照官网给的步骤安装。

---

## 编码方式概览

| Drug Encodings      | Description                             |
| :------------------ | :-------------------------------------- |
| Morgan              | Extended-Connectivity Fingerprints      |
| Pubchem             | Pubchem Substructure-based Fingerprints |
| Daylight            | Daylight-type fingerprints              |
| rdkit_2d_normalized | Normalized Descriptastorus              |
| CNN                 | Convolutional Neural Network on SMILES  |
| CNN_RNN             | A GRU/LSTM on top of a CNN on SMILES    |
| Transformer         | Transformer Encoder on ESPF             |
| MPNN                | Message-passing neural network          |

| Target Encodings | Description                                |
| ---------------- | ------------------------------------------ |
| AAC              | Amino acid composition up to 3-mers        |
| PseudoAAC        | Pseudo amino acid composition              |
| Conjoint_triad   | Conjoint triad features                    |
| Quasi-seq        | Quasi-sequence order descriptor            |
| CNN              | Convolutional Neural Network on target seq |
| CNN_RNN          | A GRU/LSTM on top of a CNN on target seq   |
| Transformer      | Transformer Encoder on ESPF                |

---

## DTI预测

(1) [工具|DeepPurpose|MPNN-CNN建立分类/回归模型预测药物靶标相互作用(小数据)](./工具DeepPurposeMPNN-CNN建立分类回归模型预测药物靶标相互作用小数据.md)

(2) [工具|DeepPurpose|MPNN-CNN建立分类/回归模型预测药物靶标相互作用(大数据)](./工具DeepPurposeMPNN-CNN建立分类回归模型预测药物靶标相互作用大数据.md)
