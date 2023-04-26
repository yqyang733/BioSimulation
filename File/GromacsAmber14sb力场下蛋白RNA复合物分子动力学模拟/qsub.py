import os
import subprocess
import time

param_file = './job_params_1.txt'
cwd = './'

with open(param_file, 'r') as f:
    f1 = f.readlines()
    for sample in f1:
        qsub_cmd = 'qsub -N {0} -d {1} -v job_num={0} job.pbs'.format(sample.strip(), os.path.join(cwd, sample.strip()))
        # print qsub_cwd
        
        exit_status = subprocess.call(qsub_cmd, shell=True)
        if exit_status == 1:
            print('Job "{}" failed to submit'.format(qsub_cmd))
        time.sleep(1)

print("Done submitting jobs!")
