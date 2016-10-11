#include "gameoflife.h"
#include "network.h"

int debugging2 = 0;

	// Rank 0 needs to generate seeds and pass them out.
int PropagateSeed(int seed){
	
		// The list of all the random seeds.
    int *globaldata=NULL;
	
		// The local seed.
    int localdata;
	
	int i = 0;

		// Rank 0 creates all of the seeds.
    if (rank == 0) {
			// Seed the generator.
		srand(seed);
		
        globaldata = malloc(NumberOfProcessors * sizeof(int) );
        for (i=0; i < NumberOfProcessors; i++){
			// Not sure why I have to use a prime to cap this but it's in the instructions.
            globaldata[i] = rand() % 93563; 
		}

			// debugging.
		if(debugging2 == 1){
			
			printf("Processor %d has data: ", rank);
		
			for (i=0; i<NumberOfProcessors; i++)
				printf("%d ", globaldata[i]);
			printf("\n");
		}
    }

		// Spread the seeds from rank 0 to all of the nodes.
    MPI_Scatter(globaldata, 1, MPI_INT, &localdata, 1, MPI_INT, 0, MPI_COMM_WORLD);

		// Debugging.
	if(debugging2 == 1){
		printf("Processor %d has data %d\n", rank, localdata);
	}
	
		// Clean up the data.
    if (rank == 0){
        free(globaldata);
	}
   	
		// Return the local seed.
	return localdata;
}



	// Print out the matrix for the game of life.
void DisplayGoL(int N){
			
	int *single = malloc((N * (N / NumberOfProcessors)) * sizeof(int*));
	
	
	
		// fill in the single with the current x matrix.
	if(rank != 0){
		
		int h, g;
		
			// Create a single long array.
		for(h = 0; h < (N / NumberOfProcessors); h++){
			for(g = 0; g < N; g++){
				single[(h * N) + g] = x[h][g];
			}
		}
				
			// send to rank 0.
		MPI_Send(single, (N * (N / NumberOfProcessors)), MPI_INT, 0, 0, MPI_COMM_WORLD);
		
		
	}else{
		int h, g;	
		int i = 0;
		
		int ** t;
	
			// Malloc the N rows.
		t = malloc(N * sizeof(int*));
	
			// Malloc n number of columns for each row.
		for (i = 0; i < N; i++) {
			t[i] = malloc(N * sizeof(int));
		}
		
			// Zero it out (mostly for debugging).
		for(h = 0; h < N; h++){
			for(g = 0; g < N; g++){
				t[h][g] = 0;				
			}
		}
		
			// Fill in rank 0 into the matrix.
		for(h = 0; h < (N / NumberOfProcessors); h++){
			for(g = 0; g < N; g++){
				t[h][g] = x[h][g];
			}
		}
		
		
		MPI_Status status;				
		for(i = 0; i < (NumberOfProcessors - 1); i++){
			
			MPI_Recv(single,(N * (N / NumberOfProcessors)),MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
	
			
		
				// Create a single long array.
			for(h = 0; h < (N / NumberOfProcessors); h++){
				for(g = 0; g < N; g++){
					
					//printf("%d : %d\n",status.MPI_SOURCE, h + (status.MPI_SOURCE + (N / NumberOfProcessors)));
					
						// Set the value into the matrix.
					t[h + (status.MPI_SOURCE * (N / NumberOfProcessors))][g] = single[(h * N) + g];
					/*
						// Debugging.
					if(single[(h * N) + g] == 1){						
						printf("A ");
					}else{						
						printf("D ");
					}*/
				}
				//printf("\n ");
			}
		
			//printf("\n%d\n\n\n", status.MPI_SOURCE);
		}
	
		// Print out the final array.
			printf("GEN: %d\n",Gen);
		for(h = 0; h < N; h++){
				// debugging?
				
			if(h < 10){
				printf(" %d: ", h);
			}else{
				printf("%d: ", h);
			}
			for(g = 0; g < N; g++){
				
				if(t[h][g] == 1){
					printf("A ");
				}else{
					printf("D ");
				} 
			}
			printf("\n");
		}
	
	}
	
/*
		// The matrix for rank 0.
	int ** t;
	
	if(rank == 0){
	
			// Malloc the N rows.
		t = malloc(N * sizeof(int*));
	
		int i;
	
			// Malloc n number of columns for each row.
		for (i = 0; i < N; i++) {
			t[i] = malloc(N * sizeof(int));
		}
	
	}*/

		// Fill in the array with alive.	
	//int h, g;

	//MPI_Gather(&localdata, 1, MPI_INT, globaldata, 1, MPI_INT, 0, MPI_COMM_WORLD);
	

	/*
		// Debugging.
	if(debugging2 == 1 && rank == 0){
	
		for(h = 0; h < N; h++){
	
			for(g = 0; g < N; g++){
				if(t[h][g] == 1){
					printf("D ");
				}else{
					printf("A ");
				} 
			}
			printf("\n");
		}
	}*/
	
		// Clean up the single long.
	free(single);
	
}