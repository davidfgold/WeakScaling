NODES=10
PROC=40
T=001
SLURM="#!/bin/bash\n\
#SBATCH --nodes=${NODES}\n\
#SBATCH --ntasks-per-node=${PROC}\n\
#SBATCH --exclusive\n\
#SBATCH --job-name=DTLZ2_MS_$(($NODES*$PROC))\n\
#SBATCH --output=output/MS_$(($NODES*$PROC))_${T}_5.out\n\
#SBATCH --error=output/MS_$(($NODES*$PROC))_${T}_5.err\n\
#SBATCH --time=1:00:00\n\
time mpirun -n $(($NODES*$PROC)) ./dtlz2_ms.exe"
echo -e $SLURM | sbatch