# .bashrc

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

# Uncomment the following line if you don't like systemctl's auto-paging feature:
# export SYSTEMD_PAGER=

# User specific aliases and functions
source /public/software/profile.d/compiler_intel-compiler-2017.5.239.sh
source /public/software/apps/gromacs-5.1.4_float/bin/GMXRC
