#include "gameoflife.h"
#include "simulate.h"

/*

Rules:

	1. A living cell that has less than 2 living neighbors dies.
		if(getNeighbors() < 2){ die=true; }

	2. A living cell that has more than 3 neighbors dies.
		if(getNeighbors() > 3){ die=true; }
		
	3. A living cell with 2 or 3 living neightbors lives on to the next generations.
		NO OP
	
	4. A dead cell with 3 living neighbors will become alive.
		if(cell==dead && getNeighbors() == 3){ alive=true; }

*/

	// The temporary matrix for storing the updated matrix.
int **Temp;

	// Stores the row above and below the node's section.
int *RowAbove;
int *RowBelow;

	// Only the edges have special case so count the others normally.
int CountNeightbors_NonEdge(int r, int c){
	
		// Count how many alive cells are around x[r][c]	
	int count = 0;
		
		// row 1.
	if(x[r-1][c-1] == ALIVE){count++;}
	if(x[r-1][c-0] == ALIVE){count++;}
	if(x[r-1][c+1] == ALIVE){count++;}
	
		// row 2.
	if(x[r-0][c-1] == ALIVE){count++;}
	if(x[r-0][c+1] == ALIVE){count++;}
	
		// row 3.
	if(x[r+1][c-1] == ALIVE){count++;}
	if(x[r+1][c-0] == ALIVE){count++;}
	if(x[r+1][c+1] == ALIVE){count++;}
	
	return count;
}


	// Count the edge case.
int CountNeightbors_Edge(int r, int c){
	
		
			// Count how many alive cells are around x[r][c]	
	int count = 0;
	
	int P1 = c - 1;		
	if(P1 < 0){P1 = N_global - 1;}	
	
	int P2 = c;
	int P3 = (c + 1) % (N_global);
	
		
	// Add up to top.
	
		// Case top (if it is the far left or far right).	
	if(r == 0){
		if(RowAbove[P1] == ALIVE){count++;}
		if(RowAbove[P2] == ALIVE){count++;}
		if(RowAbove[P3] == ALIVE){count++;}
	}else{
		
			// it is not the top row so use x.
		if(x[r-1][P1] == ALIVE){count++;}
		if(x[r-1][P2] == ALIVE){count++;}
		if(x[r-1][P3] == ALIVE){count++;}
	}
	
		// row 2.
	if(x[r-0][P1] == ALIVE){count++;}
	if(x[r-0][P3] == ALIVE){count++;}
	
	
		// Row 3.
	if(r == ((N_global / NumberOfProcessors) - 1)){
		if(RowBelow[P1] == ALIVE){count++;}
		if(RowBelow[P2] == ALIVE){count++;}
		if(RowBelow[P3] == ALIVE){count++;}
	
	}else{
			// row 3.
		if(x[r+1][P1] == ALIVE){count++;}
		if(x[r+1][P2] == ALIVE){count++;}
		if(x[r+1][P3] == ALIVE){count++;}
	}
	
	//printf("r: %d, c: %d, count: %d, P1: %d, P2: %d, P3: %d\n", r, c, count, P1, P2, P3);
	
	return count;
}

	// Gets how many neighbors there are.
int GetNeightbors(int r, int c){
	
	//if r == 1 or r = (N_global / NumberOfProcessors)
	// use the arrays from the other ranks.
	
		// temp bypass.
	if(r == 0 || r == ((N_global / NumberOfProcessors) - 1) || c == 0 || c == (N_global - 1)){		
		return CountNeightbors_Edge(r, c);		
	}else{
		return CountNeightbors_NonEdge(r, c);
	}
	
	return 0;
}

	// Trade rows with the correct rows.
int TransferRows(){
	
	int AmountToTransfer = N_global;	
	
	int RequestDataFromBelow = (rank + 1) % NumberOfProcessors;
	int RequestDataFromAbove = 0;
	
		// Handle special case.
	if(rank == 0){
		RequestDataFromAbove = NumberOfProcessors - 1;
	}else{
		RequestDataFromAbove = rank - 1;
	}
	
		// debugging.
	//printf("Req: %d -> (%d) -> %d\n", RequestDataFromAbove, rank , RequestDataFromBelow);
	
	MPI_Status status; //status.MPI_SOURCE
	int dest;
	
		// Sync all of the nodes to make it more stable.
	MPI_Barrier(MPI_COMM_WORLD);
	
		// If it should recv first or send first.
	if(rank % 2 == 0){
		
		// Send top row and recv the other nodes bottom row.
						
			// Recv (the row above) first.
		MPI_Recv(RowAbove, AmountToTransfer, MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
		
			// Send (the row below) back.
		dest = RequestDataFromBelow;
		MPI_Send(x[0], AmountToTransfer, MPI_INT, dest, 0, MPI_COMM_WORLD); // change source.
		
		// Send bottom row and recv the other nodes top row.
				// Recv (the row above) first.
		MPI_Recv(RowBelow, AmountToTransfer, MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
		
			// Send (the row below) back.
		dest = RequestDataFromAbove;
		MPI_Send(x[(N_global / NumberOfProcessors) - 1], AmountToTransfer, MPI_INT, dest, 0, MPI_COMM_WORLD); // change source.
		
		
	}else{
		
		// Send bottom row and recv the other nodes top row.
		
			// Send (the bottom row) first.
		dest = RequestDataFromBelow;
		MPI_Send(x[(N_global / NumberOfProcessors) - 1], AmountToTransfer, MPI_INT, dest, 0, MPI_COMM_WORLD);  // change source.
			
			// Recv the (other nodes top row).
		MPI_Recv(RowBelow, AmountToTransfer, MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
		
		
		// Send top row and recv the other nodes bottom row.
		
			// Send (the bottom row) first.
		dest = RequestDataFromAbove;
		MPI_Send(x[0], AmountToTransfer, MPI_INT, dest, 0, MPI_COMM_WORLD);  // change source.
			
			// Recv the (other nodes top row).
		MPI_Recv(RowAbove, AmountToTransfer, MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
		
	}
	
	
	// debug
	MPI_Barrier(MPI_COMM_WORLD);
	/*
	if(rank == 0){
		
		int i;
		for(i = 0; i < N_global; i++){
			if(RowAbove[i] == ALIVE){
				printf("A ");
			}else{
				printf("D ");
			}
			
		}		
		printf("\nCC\n");
		
		for(i = 0; i < N_global; i++){
			if(RowBelow[i] == ALIVE){
				printf("A ");
			}else{
				printf("D ");
			}
			
		}		
		printf("\n\n");
	}*/
	
		// All exit at the same time.
	MPI_Barrier(MPI_COMM_WORLD);
	
}

void DebugPrintCell(int r, int c, int before){
	return;
	if(rank != 0){
		return;
	}
	
	int R1C1 = 0;
	int R1C2 = 0;
	int R1C3 = 0;
	
	int R2C1 = 0;
	int R2C2 = Temp[r][c];
	int R2C3 = 0;
	
	int R3C1 = 0;
	int R3C2 = 0;
	int R3C3 = 0;
	
	if(r == 0){
		
	}
	
	if(before == 0){	
		printf("\nBefore: (%d)(%d)\n%d %d %d\n%d %d %d\n%d %d %d\n", r,c, R1C1, R1C2, R1C3, R2C1, R2C2,R2C3,R3C1,R3C2,R3C3);
	
	}else{
		printf("\nAfter: (%d)(%d)\n%d %d %d\n%d %d %d\n%d %d %d\n", r,c, R1C1, R1C2, R1C3, R2C1, R2C2,R2C3,R3C1,R3C2,R3C3);
	}
	
}

	// Process a single cell from our local matrix.
int Process_Cell(int r, int c){
	
	//if(rank == 0){
	//	printf("Rank: %d, PC: %d: %d : %d\n", rank, r, c, GetNeightbors(r, c));
	//}
	
	DebugPrintCell(r, c, 0);
			
		// 1. A living cell that has less than 2 living neighbors dies.
	if(x[r][c] == ALIVE && GetNeightbors(r, c) < 2){ 
		Temp[r][c] = DEAD;
		return 0;
	}

		// 2. A living cell that has more than 3 neighbors dies.
	if(x[r][c] == ALIVE && GetNeightbors(r, c) > 3){ 
		Temp[r][c] = DEAD;
		return 0;
	}
	
		// 4. A dead cell with 3 living neighbors will become alive.
	if(x[r][c] == DEAD && GetNeightbors(r, c) == 3){ 
		Temp[r][c] = ALIVE; 
		return 0;
	}
	
	DebugPrintCell(r, c, 1);
	
	return 0;
}

int SingleStepSimulate(){
	
		// Get the rows from the other matrixs
	TransferRows();
	
	int i, g;
	
		// Copy the current matrix.
	for(g = 0; g < (N_global / NumberOfProcessors); g++){
		for(i = 0; i < N_global; i++){
			Temp[g][i] = x[g][i];		
		}
	}
	
		// Process all of the cells of the 'current' matrix.	
	for(g = 0; g < (N_global / NumberOfProcessors); g++){
		for(i = 0; i < N_global; i++){
			Process_Cell(g,i);	
		}
	}
		
		// Copy the modified matrix over the 'current' one.
	for(g = 0; g < (N_global / NumberOfProcessors); g++){
		for(i = 0; i < N_global; i++){
			x[g][i] = Temp[g][i];		
		}
	}
		
	return 0;
}

int simulate(int steps){
	
	int i = 0;
		
		// Create the temp rows for the other nodes to transfer.
	RowAbove = malloc(N_global * sizeof(int*));
	RowBelow = malloc(N_global * sizeof(int*));
	
		// Copy a temp matrix to store the partly processed matrix.	
		// Malloc the correct number of rows.	
	Temp = malloc((N_global / NumberOfProcessors) * sizeof(int*));
	
		
		// Malloc n number of columns for each row.
	for (i = 0; i < (N_global / NumberOfProcessors); i++) {
		Temp[i] = malloc(N_global * sizeof(int));
	}
	

		// Simulate 'steps' time.
	for(i = 0; i < steps; i++){
		
			// Simulate one step.
		SingleStepSimulate();		
	
			// Wait for all nodes to simulate before continuing.;
		MPI_Barrier(MPI_COMM_WORLD);

	}
	
		// Clean up the matrix.	
	for (i = 0; i < (N_global / NumberOfProcessors); i++) {
		free(Temp[i]);
	}	
	
	free(Temp);	
	free(RowAbove);
	free(RowBelow);
	
	return 0;
}