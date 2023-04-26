👏 计算RNA的RMSD

---
[TOC]

---
## 计算两个单结构间RMSD
```python
# 将文件夹下所有pdb结构进行两两配对并计算所有对之间的RMSD。
import sys
import os
import numpy as np
import barnaba as bb

def rmsd_com(file_input):
    all_file = os.listdir(file_input)
    pdb_file = [i for i in all_file if i.split(".")[1] == "pdb"] 
    matrix_result = np.zeros((len(pdb_file), len(pdb_file)),dtype=np.float)
    for i in range(len(pdb_file)):
        matrix_result[i][i] = 0
    for a in range(len(pdb_file)):
        # cmd.load(pdb_file[a])
        for b in range(a+1, len(pdb_file)):
            # cmd.load(pdb_file[b])
            # tmp = cmd.super(pdb_file[a].strip(".pdb"), pdb_file[b].strip(".pdb"), cycles=5)
            tmp = bb.rmsd(pdb_file[a], pdb_file[b])
            matrix_result[a][b] = round(tmp[0], 3)
            matrix_result[b][a] = round(tmp[0], 3)

    f = open("result_barnaba.csv", "w")
    f.write("rmsd")
    for i in pdb_file:
        f.write("," + i.strip(".pdb"))
    f.write("\n")
    for a in range(len(pdb_file)):
        # cmd.load(pdb_file[a])
        f.write(pdb_file[a].strip(".pdb"))
        for b in range(len(pdb_file)):
            f.write("," + str(matrix_result[a][b]))
        f.write("\n")
    f.close

def main():
    file_input = sys.argv[1]
    rmsd_com(file_input)

if __name__ == "__main__":
    main()
```

```shell
python py ./
```

## 