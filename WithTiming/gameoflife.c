#include <stdio.h>
#include <stdlib.h>

	// Contains globals.
#include "gameoflife.h"

	// Handles matrix operations.
#include "matrix.h"
	
	// Handles transfer between nodes.
#include "network.h"

	// simulate the matrix.
#include "simulate.h"

	// Timing for the final report.
#include "timing.h"


	// Starts the game of life. (Please note: only rank 0 uses the argument seed).
int GameOfLife(int Seed, int N, int Generations){
	
		// Set the Global value for the matrix size.
	N_global = N;
	Gen = 0;
	
		// Determine how many processors there are.
    MPI_Comm_size(MPI_COMM_WORLD, &NumberOfProcessors);
	
		// Get a new seed from rank 0.
	int RealSeed = PropagateSeed(Seed);

		// Generate the matrix for all of the nodes.
	GenerateInitialGoL(RealSeed, (N / NumberOfProcessors), N);
	
		// Wait for every node to generate their matrix.
	Start_Timer(Time_MPI_Barrier);	// timing.
	MPI_Barrier(MPI_COMM_WORLD);
	End_Timer(Time_MPI_Barrier);	// timing.
	
	int i = 0;
	int step = 1;
	
	Start_Timer(Time_Avg_Display); // timing.
	//DisplayGoL(N);
	End_Timer(Time_Avg_Display); // timeing
	
		// simulate until generations is met.
	for(i = 0; i < Generations; i += step){
		Gen++;
			
		Start_Timer(Time_Avg_SingleGen); // timing.		
		simulate(step);		
		End_Timer(Time_Avg_SingleGen); // timing.

		
		
			// Debug dump the matrix to the console.
		Start_Timer(Time_Avg_Display); // timing.
		//DisplayGoL(N);
		End_Timer(Time_Avg_Display); // timing.
		
			
			// Wait for the print out to continue;
		Start_Timer(Time_MPI_Barrier); // timing.
		MPI_Barrier(MPI_COMM_WORLD);
		End_Timer(Time_MPI_Barrier);	// timing.	
	}
	

		// Delete the local matrix that was created.
	CleanupMatrix((N / NumberOfProcessors));
	
		// Wait for rank 0 to get the user input.
	Start_Timer(Time_MPI_Barrier); // timing.
	MPI_Barrier(MPI_COMM_WORLD);
	End_Timer(Time_MPI_Barrier);	// timing.
	

	return 0;
}