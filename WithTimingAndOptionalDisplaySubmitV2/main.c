// David Harkins - Project #2
#include <limits.h>  // for INT_MAX
#include <stdlib.h>  // for strtol

	// The game of life simulation.
#include "gameoflife.h"

	// Timing for the final report.
#include "timing.h"

void PrintHelp(){
	
	if(rank != 0){
		return;
	}
	
	printf("There was a problem with the commandline arguments.\n");
	printf("Please provide: 'seed' 'n' 'generations' 'display' in that order. (display is 1 or 0 to enable it or not)\n");
}

	// main.c - Used only to init MPI and process arguments.
int main(int argc,char *argv[]){
	
		// MPI Set up.
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&p);
	
		// Set up the timing.
	InitTiming();
	
		// Start up the total time timer.
	Start_Timer(Time_Total_Runtime);
	
	int Seed = 0;
	int Generations = 0;
	int N = 0;
	
		// Arguments are seed, n, generations	
	int Result = 0;
	
	if(argc != 5){
		PrintHelp();
		
			// Clean up.
		MPI_Finalize();
	
		return 0;
	}
	
		// Process the arguments.
	//if(rank == 0){

	long conv = strtol(argv[1], NULL, 10);
	if (conv > INT_MAX || conv < 0){PrintHelp(); Result=1;} else {Seed = conv; }
		
	conv = strtol(argv[2], NULL, 10);
	if (conv > INT_MAX || conv < 0){PrintHelp(); Result=1;} else {N = conv; }
		
	conv = strtol(argv[3], NULL, 10);
	if (conv > INT_MAX || conv < 0){PrintHelp(); Result=1;} else {Generations = conv; }
	
	conv = strtol(argv[4], NULL, 10);
	if (conv > INT_MAX || conv < 0){PrintHelp(); Result=1;} else {Display = conv; }
	//}
	
		// If there was not a arg error run the game of life.
	if(Result == 0){	
		Result = GameOfLife(Seed, N, Generations);
	}
	
		// Stop the total timer.
	End_Timer(Time_Total_Runtime);
	
		// Process and display all of the data.
	Timer_Print();
	
		// Clean up.
	MPI_Finalize();
	
	return Result;
}

