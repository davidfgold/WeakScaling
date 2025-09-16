/* Copyright 2012-2013 The Pennsylvania State University
 *
 * This software was written by David Hadka and others.
 * 
 * The use, modification and distribution of this software is governed by the
 * The Pennsylvania State University Research and Educational Use License.
 * You should have received a copy of this license along with this program.
 * If not, contact <dmh309@psu.edu>.
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mpi.h>
#include "borgms.h"

#define PI 3.14159265358979323846

int nvars = 13;
int nobjs = 5;

// The evaluation function is defined in the same way as the serial example
// (see dtlz2_serial.c), but is evaluated in parallel by the slave processes.
void dtlz2(double* vars, double* objs, double* consts) {
	int i;
	int j;
	int k = nvars - nobjs + 1;
	double g = 0.0;

	for (i=nvars-k; i<nvars; i++) {
		g += pow(vars[i] - 0.5, 2.0);
	}

	for (i=0; i<nobjs; i++) {
		objs[i] = 1.0 + g;

		for (j=0; j<nobjs-i-1; j++) {
			objs[i] *= cos(0.5*PI*vars[j]);
		}

		if (i != 0) {
			objs[i] *= sin(0.5*PI*vars[nobjs-i-1]);
		}
	}
	usleep(500000);
}

int main(int argc, char* argv[]) {
	int i, j;
	int rank;
	char runtime[256];
	char outputFilename[256];
	FILE* outputFile = NULL;

	// All master-slave runs need to call startup and set the runtime
	// limits.
	BORG_Algorithm_ms_startup(&argc, &argv);
	BORG_Algorithm_ms_max_time(0.005);
	//BORG_Algorithm_ms_max_evaluations(50000);

	// Define the problem.  Problems are defined the same way as the
	// serial example (see dtlz2_serial.c).
	BORG_Problem problem = BORG_Problem_create(nvars, nobjs, 0, dtlz2);

	for (j=0; j<nvars; j++) {
		BORG_Problem_set_bounds(problem, j, 0.0, 1.0);
	}

	for (j=0; j<nobjs; j++) {
		BORG_Problem_set_epsilon(problem, j, 0.01);
	}

	// Get the rank of this process.  The rank is used to ensure each
	// parallel process uses a different random seed.
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// When running experiments, we want to run the algorithm multiple
	// times and average the results.
	for (i=0; i<5; i++) {
		// Save runtime dynamics to a file.  Only the master node
		// will write to this file.  Note how we create separate
		// files for each run.
		sprintf(outputFilename, "./sets/DTLZ2_S%d.set", i); 
		sprintf(runtime, "runtime/runtime_%d.txt", i);
		BORG_Algorithm_output_runtime(runtime);

		// Seed the random number generator.
		BORG_Random_seed(37*i*(rank+1));

		// Run the master-slave Borg MOEA on the problem.
		BORG_Archive result = BORG_Algorithm_ms_run(problem);

		// Only the master process will return a non-NULL result.
		// Print the Pareto optimal solutions to the screen.
		if (result != NULL) {
			outputFile = fopen(outputFilename, "w");
			if (!outputFile) {
			BORG_Debug("Unable to open final output file\n");
		}
			BORG_Archive_print(result, outputFile);
			BORG_Archive_destroy(result);
			fclose(outputFile);
		}
	}

	// Shutdown the parallel processes and exit.
	BORG_Algorithm_ms_shutdown();
	BORG_Problem_destroy(problem);
	return EXIT_SUCCESS;
}

