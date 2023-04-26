👏 Python脚本|pdb结构加链或者改链名称

---
[TOC]

---
## 脚本
```python
import sys,re

def chain_add(pdb):
    with open(pdb) as pdb_1, open("chain.pdb","w") as rt:
        pdb_lines = pdb_1.readlines()
        for line in pdb_lines:
            lst = re.split(r'[\s]\s*', line.strip())
            if len(lst) >= 10:
                atom = float(lst[1])
                if atom <= 3015:
                    rt.write(line[:21]+"B"+line[22:])
                else:
                    rt.write(line[:21]+"A"+line[22:])
            else:
                rt.write(line)

def main():
    pdb = str(sys.argv[1])
    chain_add(pdb)
    
if __name__=="__main__":
    main()  
```

## 使用
**使用时需要适当的修改**
```shell
python py .pdb
```