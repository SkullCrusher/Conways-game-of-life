
#include "matrix.h"
#include "gameoflife.h"

int debugging3 = 2;

	// Generate the matrix.
int GenerateInitialGoL(int Seed, int NdivideByP, int N){
	
		// Reseed with the random.
	srand(Seed);
	
		// Malloc the correct number of rows.	
	x = malloc(NdivideByP * sizeof(int*));
	
	int i;
	
		// Malloc n number of columns for each row.
	for (i = 0; i < NdivideByP; i++) {
	  x[i] = malloc(N * sizeof(int));
	}

		// Fill in the array with alive or dead.	
	int h, g;
	
	for(h = 0; h < NdivideByP; h++){
		for(g = 0; g < N; g++){
			x[h][g] = rand() % 2;
		}
	}
	/*
		// debugging.
	for(h = 0; h < NdivideByP; h++){
	
		for(g = 0; g < N; g++){
			if(x[h][g] == 1){
				printf("D ");
			}else{
				printf("A ");
			} 
		}
		printf("\n");
	}*/

	
	
	return 0;
}

	// Delete the matrix that we created.
void CleanupMatrix(int n){
	int i;
	
	for (i = 0; i < n; i++) {free(x[i]);}
	
	free(x);
}
