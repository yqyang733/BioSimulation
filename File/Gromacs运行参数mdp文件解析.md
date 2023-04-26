👏 Gromacs|运行参数mdp文件解析

---
[TOC]

---
**mdp: Molecular dynamics parameters**
## 常用的一套参数
### 添加离子ion.mdp
```text
; ions.mdp - used as input into grompp to generate ions.tpr
; Parameters describing what to do, when to stop and what to save
integrator  = steep         ; Algorithm (steep = steepest descent minimization) # 指定积分算法。md：蛙跳牛顿积分算法，用于平衡动力学积分。steep：最陡下降法，用于能量最小化。cg：共轭梯度法，用于能量最小化（需要双精度，且需先做steep）。
emtol       = 1000.0        ; Stop minimization when the maximum force < 1000.0 kJ/mol/nm
emstep      = 0.01          ; Minimization step size # 步长
nsteps      = 50000         ; Maximum number of (minimization) steps to perform # 步幅

; Parameters describing how to find the neighbors of each atom and how to calculate the interactions
nstlist         = 1         ; Frequency to update the neighbor list and long range forces # 邻近列表更新频率。
cutoff-scheme   = Verlet    ; Buffered neighbor searching
ns_type         = grid      ; Method to determine neighbor list (simple, grid) # 邻近列表搜索算法。grid：网络搜索。simple：简单搜索。
coulombtype     = cutoff    ; Treatment of long range electrostatic interactions # 库伦相互作用计算方式。
rcoulomb        = 1.0       ; Short-range electrostatic cut-off # 短程库伦截断。
rvdw            = 1.0       ; Short-range Van der Waals cut-off # 短程范德华力截断。
pbc             = xyz       ; Periodic Boundary Conditions in all 3 dimensions # 周期性边界条件。xyz：使用周期性边界条件。no：没有使用周期性边界条件。xy：仅在xy方向使用周期性边界条件。
```

### 能量最小化em.mdp
```text
;====================================================
; Energy minimization
;====================================================

;----------------------------------------------------
; RUN CONTROL & MINIMIZATION
;----------------------------------------------------
define                 = -DFLEXIBLE
integrator             = steep
nsteps                 = 50000
emtol                  = 500
emstep                 = 0.01
nstcomm                = 1000

;----------------------------------------------------
; OUTPUT CONTROL
;----------------------------------------------------
nstxout                = 0      ; save coordinates to .trr every 250 steps
nstvout                = 0          ; don't save velocities to .trr
nstfout                = 0          ; don't save forces to .trr

nstxout-compressed     = 5000        ; xtc compressed trajectory output every 500 steps
compressed-x-precision = 1000
nstlog                 = 500        ; update log file every 500 steps
nstenergy              = 500        ; save energies every 500 steps
nstcalcenergy          = 500

;----------------------------------------------------
; NEIGHBOR SEARCHING
;----------------------------------------------------
cutoff-scheme          = Verlet
ns-type                = grid
nstlist                = 1
rlist                  = 1.2
pbc                    = xyz

;----------------------------------------------------
; BONDS
;----------------------------------------------------
constraints            = h-bonds
constraint-algorithm     = lincs
lincs_iter               = 1
lincs-order              = 4
continuation             = no

;----------------------------------------------------
; ELECTROSTATICS
;----------------------------------------------------
coulombtype            = PME
rcoulomb               = 1.2
pme-order              = 4
fourierspacing         = 0.10
ewald-rtol             = 1e-6

;----------------------------------------------------
; VDW
;----------------------------------------------------
vdwtype                = cut-off
rvdw                    = 1.2
vdw-modifier            = Potential-Shift
ewald-rtol-lj           = 1e-3
lj-pme-comb-rule        = Geometric
DispCorr                = EnerPres

;----------------------------------------------------
; TEMPERATURE & PRESSURE COUPL
;----------------------------------------------------
Tcoupl              = no
Pcoupl              = no
gen_vel             = no
```

### nvt.mdp
```text
;====================================================
; NVT equilibration
;====================================================

;----------------------------------------------------
; RUN CONTROL
;----------------------------------------------------
define       = -DPOSRES
integrator   = md            ; stochastic leap-frog integrator
nsteps       = 50000          ; 2 * 5,000 fs = 10 ps
dt           = 0.002         ; 2 fs
comm-mode    = Linear        ; remove center of mass translation
nstcomm      = 100           ; frequency for center of mass motion removal

;----------------------------------------------------
; OUTPUT CONTROL
;----------------------------------------------------
nstxout                = 0          ; don't save coordinates to .trr
nstvout                = 0          ; don't save velocities to .trr
nstfout                = 0          ; don't save forces to .trr
nstxout-compressed     = 10000       ; xtc compressed trajectory output every 5000 steps
compressed-x-precision = 1000       ; precision with which to write to the compressed trajectory file
nstlog                 = 5000       ; update log file every 10 ps
nstenergy              = 5000       ; save energies every 10 ps
nstcalcenergy          = 1000       ; calculate energies every 100 steps

;----------------------------------------------------
; BONDS
;----------------------------------------------------
constraint_algorithm   = lincs      ; holonomic constraints
constraints            = h-bonds  ; hydrogens only are constrained
lincs_iter             = 1          ; accuracy of LINCS (1 is default)
lincs_order            = 4          ; also related to accuracy (4 is default)
lincs-warnangle        = 30         ; maximum angle that a bond can rotate before LINCS will complain (30 is default)
continuation           = no         ; formerly known as 'unconstrained-start' - useful for exact continuations and reruns

;----------------------------------------------------
; NEIGHBOR SEARCHING
;----------------------------------------------------
cutoff-scheme   = Verlet
ns-type         = grid   ; search neighboring grid cells
nstlist         = 40     ; 20 fs (default is 10)
rlist           = 1.0    ; short-range neighborlist cutoff (in nm)
pbc             = xyz    ; 3D PBC

;----------------------------------------------------
; ELECTROSTATICS
;----------------------------------------------------
coulombtype      = PME      ; Particle Mesh Ewald for long-range electrostatics
rcoulomb         = 1.0      ; short-range electrostatic cutoff (in nm)
ewald_geometry   = 3d       ; Ewald sum is performed in all three dimensions
pme-order        = 4        ; interpolation order for PME (default is 4)
fourierspacing   = 0.10     ; grid spacing for FFT
ewald-rtol       = 1e-6     ; relative strength of the Ewald-shifted direct potential at rcoulomb

;----------------------------------------------------
; VDW
;----------------------------------------------------
vdwtype                = cut-off
rvdw                    = 1.0
vdw-modifier            = Potential-Shift
ewald-rtol-lj           = 1e-3
lj-pme-comb-rule        = Geometric
DispCorr                = EnerPres

;----------------------------------------------------
; TEMPERATURE & PRESSURE COUPL
;----------------------------------------------------
tcoupl     = V-rescale
tc_grps    =  Protein RNA Water_and_ions
tau_t      =  1.0     1.0   1.0
ref_t      =  300.0   300.0  300.0
pcoupl     =  no

;----------------------------------------------------
; VELOCITY GENERATION
;----------------------------------------------------
gen_vel      = yes      ; Velocity generation is on (if gen_vel is 'yes', continuation should be 'no')
gen_seed     = -1       ; Use random seed
gen_temp     = 300.0
```

### npt.mdp
```text
;====================================================
; NPT equilibration
;====================================================

;----------------------------------------------------
; RUN CONTROL
;----------------------------------------------------
define       = -DPOSRES
integrator   = md            ; stochastic leap-frog integrator
nsteps       = 50000         ; 2 * 50,000 fs = 100 ps
dt           = 0.002         ; 2 fs
comm-mode    = Linear        ; remove center of mass translation
nstcomm      = 100           ; frequency for center of mass motion removal

;----------------------------------------------------
; OUTPUT CONTROL
;----------------------------------------------------
nstxout                = 0          ; don't save coordinates to .trr
nstvout                = 0          ; don't save velocities to .trr
nstfout                = 0          ; don't save forces to .trr
nstxout-compressed     = 10000       ; xtc compressed trajectory output every 5000 steps
compressed-x-precision = 1000       ; precision with which to write to the compressed trajectory file
nstlog                 = 5000       ; update log file every 10 ps
nstenergy              = 5000       ; save energies every 10 ps
nstcalcenergy          = 1000        ; calculate energies every 100 steps

;----------------------------------------------------
; BONDS
;----------------------------------------------------
constraint_algorithm   = lincs      ; holonomic constraints
constraints            = h-bonds  ; hydrogens only are constrained
lincs_iter             = 1          ; accuracy of LINCS (1 is default)
lincs_order            = 4          ; also related to accuracy (4 is default)
lincs-warnangle        = 30         ; maximum angle that a bond can rotate before LINCS will complain (30 is default)
continuation           = yes         ; formerly known as 'unconstrained-start' - useful for exact continuations and reruns

;----------------------------------------------------
; NEIGHBOR SEARCHING
;----------------------------------------------------
cutoff-scheme   = Verlet
ns-type         = grid   ; search neighboring grid cells
nstlist         = 20     ; 20 fs (default is 10)
rlist           = 1.0    ; short-range neighborlist cutoff (in nm)
pbc             = xyz    ; 3D PBC

;----------------------------------------------------
; ELECTROSTATICS
;----------------------------------------------------
coulombtype      = PME      ; Particle Mesh Ewald for long-range electrostatics
rcoulomb         = 1.0      ; short-range electrostatic cutoff (in nm)
ewald_geometry   = 3d       ; Ewald sum is performed in all three dimensions
pme-order        = 4        ; interpolation order for PME (default is 4)
fourierspacing   = 0.10     ; grid spacing for FFT
ewald-rtol       = 1e-6     ; relative strength of the Ewald-shifted direct potential at rcoulomb

;----------------------------------------------------
; VDW
;----------------------------------------------------
vdwtype                 = cut-off
rvdw                    = 1.0
vdw-modifier            = Potential-Shift
ewald-rtol-lj           = 1e-3
lj-pme-comb-rule        = Geometric
DispCorr                = EnerPres

;----------------------------------------------------
; TEMPERATURE & PRESSURE COUPL
;----------------------------------------------------
tcoupl     = V-rescale
tc_grps    =  Protein RNA Water_and_ions
tau_t      =  1.0     1.0   1.0
ref_t      =  300.0   300.0  300.0
pcoupl           = Berendsen
pcoupltype       = isotropic
tau_p            = 0.5                  ; time constant (ps)
ref_p            = 1.0                  ; reference pressure (bar)
compressibility  = 4.5e-05              ; isothermal compressibility of water (bar^-1)
refcoord-scaling = all

;----------------------------------------------------
; VELOCITY GENERATION
;----------------------------------------------------
gen_vel      = no
```

### prod.mdp
```text
;====================================================
; Production simulation
;====================================================

;----------------------------------------------------
; RUN CONTROL
;----------------------------------------------------
integrator   = md            ; stochastic leap-frog integrator
nsteps       = 5000000       ; 2 * 250,000 fs = 500 ps
dt           = 0.002         ; 2 fs
comm-mode    = Linear        ; remove center of mass translation
nstcomm      = 100           ; frequency for center of mass motion removal

;----------------------------------------------------
; OUTPUT CONTROL
;----------------------------------------------------
nstxout                = 0          ; don't save coordinates to .trr
nstvout                = 0          ; don't save velocities to .trr
nstfout                = 0          ; don't save forces to .trr
nstxout-compressed     = 10000      ; xtc compressed trajectory output every 1000 steps (2 ps)
compressed-x-precision = 10000       ; precision with which to write to the compressed trajectory file
nstlog                 = 10000       ; update log file every 2 ps
nstenergy              = 10000       ; save energies every 2 ps
nstcalcenergy          = 1000        ; calculate energies every 100 steps
;----------------------------------------------------
; BONDS
;----------------------------------------------------
constraint_algorithm   = lincs      ; holonomic constraints
constraints            = h-bonds  ; hydrogens only are constrained
lincs_iter             = 1          ; accuracy of LINCS (1 is default)
lincs_order            = 4          ; also related to accuracy (4 is default)
lincs-warnangle        = 30         ; maximum angle that a bond can rotate before LINCS will complain (30 is default)
continuation           = yes

;----------------------------------------------------
; NEIGHBOR SEARCHING
;----------------------------------------------------
cutoff-scheme   = Verlet
ns-type         = grid   ; search neighboring grid cells
nstlist         = 40     ; 20 fs (default is 10)
rlist           = 1.0    ; short-range neighborlist cutoff (in nm)
pbc             = xyz    ; 3D PBC

;----------------------------------------------------
; ELECTROSTATICS
;----------------------------------------------------
coulombtype      = PME      ; Particle Mesh Ewald for long-range electrostatics
rcoulomb         = 1.0      ; short-range electrostatic cutoff (in nm)
ewald_geometry   = 3d       ; Ewald sum is performed in all three dimensions
pme-order        = 4        ; interpolation order for PME (default is 4)
fourierspacing   = 0.10     ; grid spacing for FFT
ewald-rtol       = 1e-6     ; relative strength of the Ewald-shifted direct potential at rcoulomb

;----------------------------------------------------
; VDW
;----------------------------------------------------
vdwtype                = cut-off
rvdw                    = 1.0
vdw-modifier            = Potential-Shift
ewald-rtol-lj           = 1e-3
lj-pme-comb-rule        = Geometric
DispCorr                = EnerPres

;----------------------------------------------------
; TEMPERATURE & PRESSURE COUPL
;----------------------------------------------------
tcoupl           = V-rescale
tc_grps          =  Protein RNA Water_and_ions
tau_t            =  1.0     1.0   1.0
ref_t            =  300.0   300.0  300.0
pcoupl           = Parrinello-Rahman
pcoupltype       = isotropic            ; uniform scaling of box vectors
tau_p            = 1.0                    ; time constant (ps)
ref_p            = 1.0                  ; reference pressure (bar)
compressibility  = 4.5e-05              ; isothermal compressibility of water (bar^-1)

;----------------------------------------------------
; VELOCITY GENERATION
;----------------------------------------------------
gen_vel      = no       ; Velocity generation is off (if gen_vel is 'yes', continuation should be 'no')
gen_seed     = -1       ; Use random seed
gen_temp     = 300.0
```

## 官方教程-202204
### General information
括号中或者选项中列出的第一个是默认选项。列表中的第一个元素是默认选项。方括号中的是单位。

### Preprocessing
**include：**
**define：**
- DFLEXIBLE 选项将使用柔性水代替刚性水。
- DPOSRES 选项是控制位置限制的开关。

### Run control
**integrator：**
- md 用于积分牛顿运动方程的蛙跳算法。
- steep 用于能量最小化的最速下降算法。最大步长为 emstep，容差为 emtol。当体系原子所受最大力小于 emtol 时则最小化结束。

**tinit：**
**dt：**
- 步长，默认为0.001ps，即1fs。

**nsteps：**
- 步数，默认为0，若为-1则认为不设置步数。

**init-step：**

**simulation-part：**

**mts：**

**mts-levels：**

**mts-level2-forces：**

**mts-level2-factor：**

**comm-mode：**

**nstcomm：**

**comm-grps：**

### Langevin dynamics
**bd-fric：**

**ld-seed：**

### Energy minimization
**emtol：**

**emstep：**

**nstcgsteep：**

**nbfgscorr：**

### Shell Molecular Dynamics
**niter：**

**fcstep：**

### Test particle insertion
**rtpi：**

### Output control
**nstxout：**
- 默认为0，单位是steps，作用是将坐标写入输出轨迹文件（trr）的间隔步数，最后一个坐标始终写入，除非为0，当然这意味着坐标未写入轨迹文件。

**nstvout：**
- 默认为0，单位是steps，作用是将速度写入输出轨迹文件（trr）的间隔步数。

**nstfout：**
- 默认为0，单位是steps，作用是将力写入输出轨迹文件（trr）的间隔步数。

**nstlog：**
- 默认1000，单位是steps，作用是指定每隔多少步将能量写入log文件中，最后一步能量始终写入。

**nstcalcenergy：**
- 默认100，

**nstenergy：**
- 默认1000，单位是steps，

**nstxout-compressed：**
- 默认为0，单位是steps，作用是指定每隔多少步将坐标文件写入到.xtc轨迹文件中，0表示不输出轨迹.xtc文件。

**compressed-x-precision：**

**compressed-x-grps：**

**energygrps：**

### Neighbor searching
**cutoff-scheme：**
- Verlet 
- group

**nstlist：**
- 默认为10，单位是steps，
- \>0 
- 0 
- <0 

**pbc：**
- xyz 在所有方向上使用周期性边界条件
- no 
- xy

**periodic-molecules：**

**verlet-buffer-tolerance：**

**rlist：**

### Electrostatics
**coulombtype：**
- Cut-off 

**coulomb-modifier：**

**rcoulomb-switch：**

**rcoulomb：**

**epsilon-r：**

**epsilon-rf：**

### Van der Waals
**vdwtype：**

**vdw-modifier：**

**rvdw-switch：**

**rvdw：**

**DispCorr：**

### Tables
**table-extension：**

**energygrp-table：**

### Ewald
**fourierspacing：**

**fourier-nx：**

**fourier-ny：**

**fourier-nz：**

**pme-order：**

**ewald-rtol：**

**ewald-rtol-lj：**

**lj-pme-comb-rule：**

**ewald-geometry：**

**epsilon-surface：**

### Temperature coupling
**tcoupl：**

**nsttcouple：**

**nh-chain-length：**

**print-nose-hoover-chain-variables：**

**tc-grps：**

**tau-t：**

**ref-t：**

### Pressure coupling
**pcoupl：**

**pcoupltype：**

**nstpcouple：**

**tau-p：**

**compressibility：**

**ref-p：**

**refcoord-scaling：**

### Simulated annealing
**annealing：**

**annealing-npoints：**

**annealing-time：**

**annealing-temp：**

### Velocity generation
**gen-vel：**

**gen-temp：**

**gen-seed：**

### Bonds
**constraints：**

**constraint-algorithm：**

**continuation：**

**shake-tol：**

**lincs-order：**

**lincs-iter：**

**lincs-warnangle：**

**morse：**

### Energy group exclusions
**energygrp-excl：**

### Walls
**nwall：**

**wall-atomtype：**

**wall-type：**

**table：**

**wall-r-linpot：**

**wall-density：**

**wall-ewald-zfac：**

### COM pulling
**pull：**

**pull-cylinder-r：**

**pull-constr-tol：**

**pull-print-com：**

**pull-print-ref-value：**

**pull-print-components：**

**pull-nstxout：**

**pull-nstfout：**

**pull-pbc-ref-prev-step-com：**

**pull-xout-average：**

**pull-fout-average：**

**pull-ngroups：**

**pull-ncoords：**

**pull-group1-name：**

**pull-group1-weights：**

**pull-group1-pbcatom：**

**pull-coord1-type：**

**pull-coord1-potential-provider：**

**pull-coord1-geometry：**

**pull-coord1-expression：**

**pull-coord1-dx：**

**pull-coord1-groups：**

**pull-coord1-dim：**

**pull-coord1-origin：**

**pull-coord1-vec：**

**pull-coord1-start：**

**pull-coord1-init：**

**pull-coord1-rate：**

**pull-coord1-k：**

**pull-coord1-kB：**

### AWH adaptive biasing
**awh：**

**awh-potential：**

**awh-share-multisim：**

**awh-seed：**

**awh-nstout：**

**awh-nstsample：**

**awh-nsamples-update：**

**awh-nbias：**

**awh1-error-init：**

**awh1-growth：**

**awh1-equilibrate-histogram：**

**awh1-target：**

**awh1-target-beta-scaling：**

**awh1-target-cutoff：**

**awh1-user-data：**

**awh1-share-group：**

**awh1-ndim：**

**awh1-dim1-coord-provider：**

**awh1-dim1-coord-index：**

**awh1-dim1-force-constant：**

**awh1-dim1-start：**

**awh1-dim1-end：**

**awh1-dim1-diffusion：**

**awh1-dim1-cover-diameter：**

### Enforced rotation
**rotation：**

**rot-ngroups：**

**rot-group0：**

**rot-type0：**

**rot-massw0：**

**rot-vec0：**

**rot-pivot0：**

**rot-rate0：**

**rot-k0：**

**rot-slab-dist0：**

**rot-min-gauss0：**

**rot-eps0：**

**rot-fit-method0：**

**rot-potfit-nsteps0：**

**rot-potfit-step0：**

**rot-nstrout：**

**rot-nstsout：**

### NMR refinement
**disre：**

**disre-weighting：**

**disre-mixed：**

**disre-fc：**

**disre-tau：**

**nstdisreout：**

**orire：**

**orire-fc：**

**orire-tau：**

**orire-fitgrp：**

**nstorireout：**

### Free energy calculations
**free-energy：**

**expanded：**

**init-lambda：**

**delta-lambda：**

**init-lambda-state：**

**fep-lambdas：**

**coul-lambdas：**

**vdw-lambdas：**

**bonded-lambdas：**

**restraint-lambdas：**

**mass-lambdas：**

**temperature-lambdas：**

**calc-lambda-neighbors：**

**sc-function：**

**sc-alpha：**

**sc-r-power：**

**sc-coul：**

**sc-power：**

**sc-sigma：**

**sc-gapsys-scale-linpoint-lj：**

**sc-gapsys-scale-linpoint-q：**

**sc-gapsys-sigma-lj：**

**couple-moltype：**

**couple-lambda0：**

**couple-lambda1：**

**couple-intramol：**

**nstdhdl：**

**dhdl-derivatives：**

**dhdl-print-energy：**

**separate-dhdl-file：**

**dh-hist-size：**

**dh-hist-spacing：**

### Expanded Ensemble calculations
**nstexpanded：**

**lmc-stats：**

**lmc-mc-move：**

**lmc-seed：**

**mc-temperature：**

**wl-ratio：**

**wl-scale：**

**init-wl-delta：**

**wl-oneovert：**

**lmc-repeats：**

**lmc-gibbsdelta：**

**lmc-forced-nstart：**

**nst-transition-matrix：**

**symmetrized-transition-matrix：**

**mininum-var-min：**

**init-lambda-weights：**

**lmc-weights-equil：**

**simulated-tempering：**

**sim-temp-low：**

**sim-temp-high：**

**simulated-tempering-scaling：**

### Non-equilibrium MD
**acc-grps：**

**accelerate：**

**freezegrps：**

**freezedim：**

**cos-acceleration：**

**deform：**

### Electric fields
**electric-field-x：**

**electric-field-y：**

**electric-field-z：**

### Mixed quantum/classical molecular dynamics
**QMMM-grps：**

**QMMM：**

### Computational Electrophysiology
**swapcoords：**

**swap-frequency：**

**split-group0：**

**split-group1：**

**massw-split0：**

**massw-split1：**

**solvent-group：**

**coupl-steps：**

**iontypes：**

**iontype0-name：**

**iontype0-in-A：**

**iontype0-in-B：**

**bulk-offsetA：**

**bulk-offsetB：**

**threshold：**

**cyl0-r：**

**cyl0-up：**

**cyl0-down：**

**cyl1-r：**

**cyl1-up：**

**cyl1-down：**

### Density-guided simulations
**density-guided-simulation-active：**

**density-guided-simulation-group：**

**density-guided-simulation-similarity-measure：**

**density-guided-simulation-atom-spreading-weight：**

**density-guided-simulation-force-constant：**

**density-guided-simulation-gaussian-transform-spreading-width：**

**density-guided-simulation-gaussian-transform-spreading-range-in-multiples-of-width：**

**density-guided-simulation-reference-density-filename：**

**density-guided-simulation-nst：**

**density-guided-simulation-normalize-densities：**

**density-guided-simulation-adaptive-force-scaling：**

**density-guided-simulation-adaptive-force-scaling-time-constant：**

**density-guided-simulation-shift-vector：**

**density-guided-simulation-transformation-matrix：**

### QM/MM simulations with CP2K Interface
**qmmm-cp2k-active：**

**qmmm-cp2k-qmgroup：**

**qmmm-cp2k-qmmethod：**

**qmmm-cp2k-qmcharge：**

**qmmm-cp2k-qmmultiplicity：**

**qmmm-cp2k-qmfilenames：**
