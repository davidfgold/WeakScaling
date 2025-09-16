Evaluating the parallel efficiency of the Master Worker Borg MOEA

Scaling experiments help us characterize and understand code performance on High Performance Computing (HPC) systems and are often required for HPC allocation proposals. Previous posts have explored different types of scaling experiments, as well as metrics for evaluating the parallel performance of the Many Objective Evolutionary Algorithms (MOEAs).  In this post, I'll demonstrate how to assess the weak scaling of the Master Worker Borg MOEA. You can find the code and input files (with the exception of the Borg MOEA) in this Github repo. You can request a license for the Borg MOEA (for academic purposes) at the Borg homepage (access is determined on a case by case basis).
Measuring parallel efficiency

For this analysis, I'll focus on weak scaling using the parallel efficiency metric:

$ latex efficiency = \frac{NFE_{P}}{p \times NFE_{s}} $

Where:

$  p $ = number of processors

$ {P} $ = total number of function evaluations with a parallel MOEA run on p processors

$ NFE_{S} $ = total number of function evaluations with a serial MOEA

Perfect scaling yields an efficiency of 1.0, meaning the parallel algorithm achieves p times the number of function evaluations (NFE) of the serial version. In practice, communication overhead, load balancing issues, and algorithmic bottlenecks typically reduce efficiency as processor count increases.
Experimental design

For this scaling experiment we'll need to specify four things:

    The multiobjective problem

    For this problem, I'll use the DTLZ2 test problem with 5 objectives as the multiobjective test problem. DTLZ2 is a well-studied benchmark that provides a known Pareto front for validation. 
    ﻿
    The time it takes to evaluate a single solution (the function evaluation time)

    To evaluate the parallel efficiency, we can fix the total time the MW Borg has to explore the problem. In this experiment, I'll use a 20-second runtime to reduce the burden on the HPC resource and keep things manageable. The performance of MOEAs can vary based on the time it takes to run one function evaluation (this is often the simulation time when we are coupling the MOEA with a water resources system model). Shorter function evaluation times are likely to lead to less efficient scaling since they increase the potential for computational bottlenecks. 

    To explore how MW Borg performs across various function evaluation times, I'll use three different times for this experiment:T = 0.05 seconds; T = 0.10 seconds; T = 0.50 seconds
    ﻿
    The number of processors we explore performance across. 

    I performed this experiment on a small cluster, using processor counts of 2, 20, 40, 200, 400, and 600 cores.

To account for the inherent stochasticity of the MOEA, I ran 5 random seed replicates for each combination of function evaluation time and processor count (a total of 90 experimental runs)
Results
This image has an empty alt attribute; its file name is weakscaling.png

The weak scaling results reveal strong difference in how MW Borg Scales across the three function evaluation times. For the fastest function evaluations (T = 0.05 seconds), the parallel efficiency peaks at 40 cores before rapidly declining. This pattern indicates computational bottlenecks where function evaluations complete faster than the master process can generate new candidate solutions, distribute work, and process results. Worker processes spend increasing time idle as core count grows.

For the intermediate function evaluation time (T = 0.10 seconds), peak efficiency occurs at 200 cores with a gradual decline at higher processor counts. The longer evaluation time provides the master process sufficient time to maintain worker utilization, but communication overhead eventually dominates at very high core counts.

For the slowest function evaluation time (T = 0.50 seconds), we observe no efficiency decline within our tested range, suggesting the algorithm could scale effectively beyond 600 cores. The extended evaluation time eliminates computational bottlenecks, allowing the master-worker architecture to operate at full efficiency.
Final remarks

This post provides an example for performing a weak scaling evaluation of the MW Borg MOEA. This type of analysis is useful when planning your own HPC experiment, or when requesting a new HPC allocation at a large resource. In practice, scaling experiments should be conducted on the system you are intending to run a full scale experiment on (often you can get a trial allocation for this purpose). The results from this small experiment show how more computational resources don't necessarily lead to improved efficiency. Importantly, however, the results of this small example are promising for the MW Borg MOEA. When dealing with real world water resources systems, we often have function evaluation times greatly exceeding the times shown here (Waterpaths for example, historically has a function evaluation time of around 20 seconds). For these larger evaluation times, the MW Borg MOEA is likely to scale very well. However, the only way to find out is to try for yourself!
