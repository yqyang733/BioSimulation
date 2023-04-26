👏 Pymol脚本|将多个states的pdb文件分割为多个单pdb文件

---
[TOC]

---
```python
import sys
from pymol import cmd

def split_states(file_input):
    cmd.load(file_input)
    name = str(file_input).strip(".pdb")
    for i in range(10):
        cmd.save(name + "_" + str(i+1) + ".pdb", name, state=i+1)
    cmd.delete("all")

def main():
    file_input = sys.argv[1]
    split_states(file_input)

if __name__ == "__main__":
    main()
```

```shell
python py all_states.pdb
```