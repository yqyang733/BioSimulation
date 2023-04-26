👏 Gromacs|gmx grompp

---
[TOC]

---
## 简介
生成运行输入文件。

## 格式
```text
gmx grompp [-f [<.mdp>]] [-po [<.mdp>]] [-c [<.gro/.g96/...>]]
    [-r [<.gro/.g96/...>]] [-rb [<.gro/.g96/...>]] [-n [<.ndx>]]
    [-p [<.top>]] [-pp [<.top>]] [-o [<.tpr/.tpb/...>]]
    [-t [<.trr/.cpt/...>]] [-e [<.edr>]] [-imd [<.gro>]]
    [-ref [<.trr/.cpt/...>]] [-nice ] [-[no]v] [-time ]
    [-[no]rmvsbds] [-maxwarn ] [-[no]zero] [-[no]renum]
```

## 参数
**输入/输出文件选项**
|参数|默认值|类型|说明|
|:----|:----|:----|:----|
|-f [<.mdp>]|grompp.mdp|输入|包含MD参数的grompp输入文件。|
|-po [<.mdp>]|mdout.mdp|输出|包含MD参数的grompp输入文件。|
|-c [<.gro/.g96/...>]|conf.gro|输入|结构文件：gro g96 pdb brk ent esp tpr tpb tpa|
|-r [<.gro/.g96/...>]|conf.gro|输入，可选|结构文件：gro g96 pdb brk ent esp tpr tpb tpa|
|-rb [<.gro/.g96/...>]|conf.gro|输入，可选|结构文件：gro g96 pdb brk ent esp tpr tpb tpa|
|-n [<.ndx>]|index.ndx|输入，可选|索引文件。|
|-p [<.top>]|topol.top|输入|拓扑文件。|
|-pp [<.top>]|processed.top|输出，可选|拓扑文件。|
|-o [<.tpr/.tpb/...>]|topol.tpr|输出|运行输入文件: tpr tpb tpa|
|-t [<.trr/.cpt/...>]|traj.trr|输入，可选|全精度轨迹：trr cpt trj tng|
|-e [<.edr>]|ener.edr|输入，可选|能量文件。|
|-imd [<.gro>]|imdgroup.gro|输出，可选|Gromos-87格式的坐标文件。|
|-ref [<.trr/.cpt/...>]|rotref.trr|输入/输出，可选|全精度轨迹: trr cpt trj tng|

**控制选项**
|参数|默认值|说明|
|:----|:----|:----|
|-nice \<int>|0|设置优先级。|
|-[no]v|no|输出更多信息。|
|-time \<real>|-1|采用此刻的帧或此刻之后的第一帧。|
|-[no]rmvsbds|yes|移除涉及虚拟位点的具有恒定能量的键合相互作用|
|-maxwarn \<int>|0|处理输入过程中所允许的最大警告数目. 如果非正常使用可能导致体系不稳定。|
|-[no]zero|no|对没有默认值的键合相互作用，将其参数设置为零，而不是产生错误。|
|-[no]renum|yes|重新对原子类型进行编号以使原子类型的数目最小。|

## 常规使用方法
（1）**gmx grompp(gromacs预处理器)读取分子的拓扑文件，检查其合理性，并将拓扑从分子描述拓展到原子描述。** 拓扑文件包含了分子类型和分子数目的信息，预处理器会复制每个需要的分子。对分子类型的数目没有限制。键和键角可以转换为约束，对氢原子和重原子独立进行。然后读入坐标文件，如果需要可以由Maxwell分布生成速度。

（2）**gmx grompp还会读取用于gmx mdrun的参数(例如：MD的步数，时间步长和截断)以及其他一些参数，如NEMD参数。** 程序会对这些参数进行校正以使得净加速度为零。程序最终会生成一个二进制文件，它可以单独地作为MD程序的输入文件。  

（3）**gmx grompp使用来自拓扑文件的原子名称。** 当坐标文件中(选项-c)的原子名称与拓扑文件中的不一致时，会产生警告。注意，原子名称与模拟无关，因为生成相互作用参数只使用了原子类型。

（4）gmx grompp使用内置的预处理器来解决包含，宏等问题。预处理器支持下面的关键词：
```shell
#ifdef VARIABLE
#ifndef VARIABLE
#else
#endif
#define VARIABLE
#undef VARIABLE
#include "filename"
#include <filename>
```
在.mdp文件中，拓扑文件中的这些语句的功能可以通过以下两个选项来模块化：
```shell
define = -DVARIABLE1 -DVARIABLE2
include = -I/home/john/doe
```
指定-pp选项可以输出预处理的拓扑文件，这样你就可以验证其内容了。

（5）当使用位置限制时，可以使用-r选项提供一个包含限制坐标的文件，否则将会使用相对于来自-c选项的构象进行限制。对自由能计算，B拓扑的坐标可使用-rb提供，否则它们将等同于A拓扑的坐标。

（6）**起始坐标可以由-t提供的轨迹文件中读取。** 程序会读取最后一帧的坐标和速度，除非使用了-time选项。只有当这些信息缺失时才会使用由-c提供的文件中的坐标。注意，当.mdp文件中设置了gen_vel = yes时，不会使用文件中的速度信息。可以使用选项-e提供能量文件，以读取Nose-Hoover和/或Parrinello-Rahman耦合变量。

（7）**续跑相关。** gmx grompp可用于重启模拟(保持连续)，只需要使用-t选项提供一个检查点文件即可。然而，**如果只是简单地改变运行步数以延长模拟，使用gmx convert-tpr比使用gmx grompp更方便。你只需要使用-cpi选项将旧的检查点文件直接提供给gmx mdrun即可。如果想要改变系综或是输出频率等，建议使用-t为gmx grompp提供检查点文件，并使用-f提供新的.mdp文件。**

（8）**默认情况下，所有由构建虚拟位点引入的具有恒定能量的键合相互作用都会被移除。** 如果此恒定能量不为零，将会导致总能量移动。所有键合相互作用都可以通过关闭-rmvsbds来维持。另外，所有因虚拟位点构建引入的距离约束都具有恒定能量，它们都会被移除。如果仍然存在涉及虚拟位点的任何约束，将导致致命错误。

（9）**为验证运行输入文件，请注意屏幕上显示的所有警告，并对必要的警告加以纠正。** 此外也需要查看mdout.mdp文件的内容，其中包括注释行以及gmx grompp读入的输入信息。如果有疑问，你可以使用-debug选项启动gmx grompp，这将会生成一个grompp.log文件(以及真正的调试信息)，里面包含了更多的信息。你可以使用gmx dump程序查看运行输入文件的内容。gmx check可用于比较两个运行输入文件的内容。

（10）-maxwarn选项可用于覆盖gmx grompp给出的警告，否则会停止输出。在某些情况下，警告无关紧要，但大多数情况下并非如此。建议用户在使用这个选项绕过这些警告之前认真阅读并理解输出信息。

## 源码位置及解析
gromacs-2022.2/src/gromacs/gmxpreprocess/grompp.cpp

## 示例