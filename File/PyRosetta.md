👏 PyRosetta

---
[TOC]

---
## 安装


## 参考资料
1. [PyRosetta module documentation 所有函数详解](https://graylab.jhu.edu/PyRosetta.documentation/index.html#)


## Rosetta中用于蛋白建模以及设计的能量打分函数
1. Alford R F, Leaver-Fay A, Jeliazkov J R, et al. The Rosetta all-atom energy function for macromolecular modeling and design[J]. Journal of chemical theory and computation, 2017, 13(6): 3031-3048. [pdf](PyRosetta/acs.jctc.7b00125.pdf)

**PyRosetta打分函数**
```python
import pyrosetta
import os
import pandas as pd
from typing import *

pyrosetta.distributed.maybe_init(extra_options='-mute all')

# ------------------------------------------------------------------------------------
# not all these method were used.

def get_weights(scorefxn:Union[str, pyrosetta.ScoreFunction]) -> Dict[str, float]:
    """
    Gets weights for scorefxn
    """
    if isinstance(scorefxn, str):
        #scorefxn = pyrosetta.create_score_function(scorefxn)
        # mod:
        scorefxn = get_scorefxn(scorefxn)
    get_weight = lambda scorefxn, score_type: scorefxn.get_weight(getattr(pyrosetta.rosetta.core.scoring.ScoreType, score_type.name))
    return {'name': scorefxn.get_name(),
            **{score_type.name: get_weight(scorefxn, score_type) for score_type in scorefxn.get_nonzero_weighted_scoretypes()},
           **get_ref_values_badly(scorefxn, prefix=True)}

def get_ref_values_badly(scorefxn: pyrosetta.ScoreFunction,
                        prefix: Optional[bool]=True) -> Dict[str, float]:
    """
    I could not find a direct way to get_method_weights.
    Here this horrid way.
    """
    # similarity order 'IVLFCMAGTSWYPHNDEQKR'
    # name1 alphabetical order 'ACDEFGHIKLMNPQRSTVWY'
    # name3 alphabetical order 'ARNDCQEGHILKMFPSTWYV'
    pose = pyrosetta.pose_from_sequence('ARNDCQEGHILKMFPSTWYV')
    scorefxn(pose)
    aa_ref = {}
    for res in range(1, pose.total_residue() + 1):
        name = pose.residue(res).name3()
        value = pose.energies().residue_total_energies(res)[pyrosetta.rosetta.core.scoring.ScoreType.ref]
        if prefix:
            aa_ref[f'ref_{name}'] = value
        else:
            aa_ref[name] = value
    return aa_ref

def get_scorefxn(scorefxn_name:str) -> pyrosetta.ScoreFunction:
    """
    Gets the scorefxn with appropriate corrections.
    """
    corrections = {'beta_july15': False,
                   'beta_nov16': False,
                   'gen_potential': False,
                   'restore_talaris_behavior': False
                  }
    if 'beta_july15' in scorefxn_name or 'beta_nov15' in scorefxn_name:
        # beta_july15 is ref2015
        corrections['beta_july15'] = True
    elif 'beta_nov16' in scorefxn_name:
        corrections['beta_nov16'] = True
    elif 'genpot' in scorefxn_name:
        corrections['gen_potential'] = True
        pyrosetta.rosetta.basic.options.set_boolean_option('corrections:beta_july15', True)
    elif 'talaris' in scorefxn_name:  #2013 and 2014
        corrections['restore_talaris_behavior'] = True
    else:
        pass
    for corr, value in corrections.items():
        pyrosetta.rosetta.basic.options.set_boolean_option(f'corrections:{corr}', value)
    return pyrosetta.create_score_function(scorefxn_name)

def get_possible_scorefxn() -> List[str]:
    """
    Returns the scorefxn names
    """
    folder = os.path.join(os.path.split(pyrosetta.__file__)[0], 'database', 'scoring', 'weights')
    return sorted([fn.replace('.wts', '') for fn in os.listdir(folder) if '.wts' == os.path.splitext(fn)[1]])
    

def get_scorefxn_block(scorefxn_name) -> str:
    """
    Read the file given a scorefxn name
    """
    filename = os.path.join(os.path.split(pyrosetta.__file__)[0], 'database', 'scoring', 'weights', f'{scorefxn_name}.wts')
    with open(filename, 'r') as fh:
        return fh.read()
    
def find_metion(word:str) -> List[str]:
    """
    Find all the scorefxn names whose files contain the string word (case insensitive).
    
    >>> find_metion('spades')
    
    returns ``['hydrate_score12']``
    """
    mentionants = []
    scorefxn_names = get_possible_scorefxn()
    for scorefxn_name in scorefxn_names:
        block = get_scorefxn_block(scorefxn_name)
        if word.lower() in block.lower():
            mentionants.append(scorefxn_name)
    return mentionants

# ------- main ---------------------------------------------------------------------
scorefxn_names = ['ref2015', 'ref2015_cart',  'beta_nov16', 'beta_nov16_cart']
# 'talaris2013' and 'talaris2014' ommitted due to clutter
# 'franklin2019', 'beta_genpot' will not work due to the ref method weights business.
terms = pd.DataFrame(map(get_weights, scorefxn_names)).fillna(0).transpose()
print(terms.to_markdown())
```

| name                 | ref2015  | ref2015_cart | beta_nov16 | beta_nov16_cart |
|:---------------------|:---------|:-------------|:-----------|:----------------|
| fa_atr               | 1.0      | 1.0          | 1.0        | 1.0             |
| fa_rep               | 0.55     | 0.55         | 0.55       | 0.55            |
| fa_sol               | 1.0      | 1.0          | 1.0        | 1.0             |
| fa_intra_rep         | 0.005    | 0.005        | 0.0        | 0.0             |
| fa_intra_sol_xover4  | 1.0      | 1.0          | 1.0        | 1.0             |
| lk_ball_wtd          | 1.0      | 1.0          | 0.0        | 0.0             |
| fa_elec              | 1.0      | 1.0          | 1.0        | 1.0             |
| pro_close            | 1.25     | 0.0          | 1.25       | 0.0             |
| hbond_sr_bb          | 1.0      | 1.0          | 1.0        | 1.0             |
| hbond_lr_bb          | 1.0      | 1.0          | 1.0        | 1.0             |
| hbond_bb_sc          | 1.0      | 1.0          | 1.0        | 1.0             |
| hbond_sc             | 1.0      | 1.0          | 1.0        | 1.0             |
| dslf_fa13            | 1.25     | 1.25         | 1.25       | 1.25            |
| omega                | 0.4      | 0.4          | 0.48       | 0.48            |
| fa_dun               | 0.7      | 0.7          | 0.0        | 0.0             |
| p_aa_pp              | 0.6      | 0.6          | 0.61       | 0.61            |
| yhh_planarity        | 0.625    | 0.625        | 0.0        | 0.0             |
| ref                  | 1.0      | 1.0          | 1.0        | 1.0             |
| rama_prepro          | 0.45     | 0.45         | 0.5        | 0.5             |
| ref_ALA              | 1.32468  | 1.32468      | 2.3386     | 2.3386          |
| ref_ARG              | -0.09474 | -0.09474     | -1.281     | -1.281          |
| ref_ASN              | -1.34026 | -1.34026     | -0.873554  | -0.873554       |
| ref_ASP              | -2.14574 | -2.14574     | -2.2837    | -2.2837         |
| ref_CYS              | 3.25479  | 3.25479      | 3.2718     | 3.2718          |
| ref_GLN              | -1.45095 | -1.45095     | -1.0644    | -1.0644         |
| ref_GLU              | -2.72453 | -2.72453     | -2.5358    | -2.5358         |
| ref_GLY              | 0.79816  | 0.79816      | 1.2108     | 1.2108          |
| ref_HIS              | -0.30065 | -0.30065     | 0.134426   | 0.134426        |
| ref_ILE              | 2.30374  | 2.30374      | 1.0317     | 1.0317          |
| ref_LEU              | 1.66147  | 1.66147      | 0.729516   | 0.729516        |
| ref_LYS              | -0.71458 | -0.71458     | -1.6738    | -1.6738         |
| ref_MET              | 1.65735  | 1.65735      | 1.2334     | 1.2334          |
| ref_PHE              | 1.21829  | 1.21829      | 1.4028     | 1.4028          |
| ref_PRO              | -1.64321 | -1.64321     | -5.1227    | -5.1227         |
| ref_SER              | -0.28969 | -0.28969     | -1.1772    | -1.1772         |
| ref_THR              | 1.15175  | 1.15175      | -1.425     | -1.425          |
| ref_TRP              | 2.26099  | 2.26099      | 3.035      | 3.035           |
| ref_TYR              | 0.58223  | 0.58223      | 0.964136   | 0.964136        |
| ref_VAL              | 2.64269  | 2.64269      | 2.085      | 2.085           |
| cart_bonded          | 0.0      | 0.5          | 0.0        | 0.5             |
| fa_intra_atr_xover4  | 0.0      | 0.0          | 1.0        | 1.0             |
| fa_intra_rep_xover4  | 0.0      | 0.0          | 0.55       | 0.55            |
| lk_ball              | 0.0      | 0.0          | 0.92       | 0.92            |
| lk_ball_iso          | 0.0      | 0.0          | -0.38      | -0.38           |
| lk_ball_bridge       | 0.0      | 0.0          | -0.33      | -0.33           |
| lk_ball_bridge_uncpl | 0.0      | 0.0          | -0.33      | -0.33           |
| fa_intra_elec        | 0.0      | 0.0          | 1.0        | 1.0             |
| fa_dun_dev           | 0.0      | 0.0          | 0.69       | 0.69            |
| fa_dun_rot           | 0.0      | 0.0          | 0.76       | 0.76            |
| fa_dun_semi          | 0.0      | 0.0          | 0.78       | 0.78            |
| hxl_tors             | 0.0      | 0.0          | 1.0        | 1.0             |