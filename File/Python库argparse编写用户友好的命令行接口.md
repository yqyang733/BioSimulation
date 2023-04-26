👏 Python库|argparse|编写用户友好的命令行接口

---
[TOC]

---
## 简介
argparse 模块可以让人轻松编写用户友好的命令行接口。程序定义它需要的参数，然后 argparse 将弄清如何从 sys.argv 解析出那些参数。 argparse 模块还会自动生成帮助和使用手册，并在用户给程序传入无效参数时报出错误信息。

## 使用流程示例
```python
import argparse

parser = argparse.ArgumentParser(description='DEEPScreen arguments')  # ArgumentParser 对象包含将命令行解析成 Python 数据类型所需的全部信息。# 使用argparse 的第一步是创建一个ArgumentParser 对象。
parser.add_argument(
    '--targetid',
    type=str,
    default="CHEMBL286",
    metavar='TID',
    help='Target ChEMBL ID')   # 添加参数
parser.add_argument(
    '--model',
    type=str,
    default="CNNModel1",
    metavar='MN',
    help='model name (default: CNNModel1)')
parser.add_argument(
    '--fc1',
    type=int,
    default=512,
    metavar='FC1',
    help='number of neurons in the first fully-connected layer (default:512)')
parser.add_argument(
    '--fc2',
    type=int,
    default=256,
    metavar='FC2',
    help='number of neurons in the second fully-connected layer (default:256)')
parser.add_argument(
    '--lr',
    type=float,
    default=0.001,
    metavar='LR',
    help='learning rate (default: 0.001)')
parser.add_argument(
    '--bs',
    type=int,
    default=32,
    metavar='BS',
    help='batch size (default: 32)')
parser.add_argument(
    '--dropout',
    type=float,
    default=0.25,
    metavar='DO',
    help='dropout rate (default: 0.25)')
parser.add_argument(
    '--epoch',
    type=int,
    default=100,
    metavar='EPC',
    help='Number of epochs (default: 100)')
parser.add_argument(
    '--en',
    type=str,
    default="my_experiment",
    metavar='EN',
    help='the name of the experiment (default: my_experiment)')  # 增加需要的所有的参数并存储在parser中。

args = parser.parse_args()
print(args)
train_validation_test_training(args.targetid, args.model, args.fc1, args.fc2, args.lr, args.bs,
                                   args.dropout, args.epoch, args.en)
# 最后将参数传入定义好的函数中即可。
```

## 参数解析
### ArgumentParser对象
(1) 语法：  
```python
argparse.ArgumentParser(prog=None, usage=None, description=None, epilog=None, parents=[], formatter_class=argparse.HelpFormatter, prefix_chars='-', fromfile_prefix_chars=None, argument_default=None, conflict_handler='error', add_help=True, allow_abbrev=True)
```
(2) 参数：  
- description：在参数帮助文档之前显示的文本（默认值：无）   


### add_argument()方法
(1) 语法：  
```python
ArgumentParser.add_argument(name or flags...[, action][, nargs][, const][, default][, type][, choices][, required][, help][, metavar][, dest])
```
(2) 参数：  
- name or flags：一个命名或者一个选项字符串的列表，例如 foo 或 -f, --foo    例如：("--targetid",), ("--input_file", "-i",)，好像写一个写两个都行。  
- type：命令行参数应当被转换成的类型。  
- default：当参数未在命令行中出现时使用的值。   
- metavar：在使用方法消息中使用的参数值示例。  
- help：一个此选项作用的简单描述。  
- required：此命令行选项是否可省略（仅选项可用）。  
- nargs：命令行参数应当消耗的数目（N：表示前面的参数接收N个参数；?：接收0或1个参数；*：接收0或多个参数；+：接收1或多个参数，未传参数时，报错）。    