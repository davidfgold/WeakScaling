TOTAL=2
NSEEDS=3
SEEDS=$(seq 0 ${NSEEDS})
for SEED in ${SEEDS}
do 
SLURM="#!/bin/bash\n\
#SBATCH --nodes=1\n\
#SBATCH --ntasks-per-node=1\n\
#SBATCH --exclusive\n\
#SBATCH --job-name=HV_${TOTAL}_${SEED}\n\
#SBATCH --output=output/MS_HV.out\n\
#SBATCH --error=output/MS_HV.err\n\
#SBATCH --time=12:00:00\n\

time java -cp MOEAFramework-4.5-Demo.jar org.moeaframework.analysis.tools.ResultFileEvaluator 
--problem DTLZ2_4 --epsilon 0.01,0.01,0.01,0.01 
--input runtime/DTLZ2_4O_${TOTAL}P/runtime_${SEED}.txt 
--reference sets/DTLZ2.4D.pf --output runtime/DTLZ2_4O_${TOTAL}P_S${SEED}.metrics"


echo -e $SLURM | sbatch
done

