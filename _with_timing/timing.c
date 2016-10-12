#include "gameoflife.h"
#include "timing.h"
#include <sys/time.h>

/*

Timing: Your code should measure the following times for reporting purposes:
	- Total runtime (excluding the time for display) to run the simulation for the userspecified G generations
	- Average time per generation (excluding time for display)
	- Average time per the display function
	- Time for the different communication steps 
	(i.e., if you are using say MPI_Barrier, MPI_Send, MPI_Recv, MPI_Gather, MPI_Scatter, MPI_Bcast, MPI_Reduce, etc. in different places of your code, then attach a timer specific to each of those calls and report them separately at the end – i.e., one timer per MPI function).

	These timings should be reported (from each rank) at the end before the program terminates. 
*/


	// The time structures.
struct timeval TTR_t1, TTR_t2;
struct timeval TAS_t1, TAS_t2;
struct timeval TAD_t1, TAD_t2;
struct timeval TMB_t1, TMB_t2;
struct timeval TMS_t1, TMS_t2;
struct timeval TMR_t1, TMR_t2;
struct timeval TM_Scatter_t1, TM_Scatter_t2;

	// If the timeval has been used before.
int TTR_Set = 0;
int TAS_Set = 0;
int TAD_Set = 0;

int TMB_Set = 0;
int TMS_Set = 0;
int TMR_Set = 0;

int TM_Scatter_Set = 0;

	// Result containers.
unsigned int TTR_Result = 0;

	// These are avg so they require two variables extra.
unsigned int TAS_Result = 0;
unsigned int TAS_Count = 0;
unsigned int TAS_Running_Total =  0;

unsigned int TAD_Result = 0;
unsigned int TAD_Count = 0;
unsigned int TAD_Running_Total =  0;

unsigned int TMB_Result = 0;
unsigned int TMS_Result = 0;
unsigned int TMR_Result = 0;
unsigned int TM_Scatter_Result = 0;



	// Starts a timer based on a defined type above.
void Start_Timer(int Type){
	
		// If the time is already started, block from restarting.	
	if(Type == Time_Total_Runtime){		
		if(TTR_Set == 0){gettimeofday(&TTR_t1,NULL);TTR_Set=1;}		
		return;
	}
	
	if(Type == Time_Avg_SingleGen){
		if(TAS_Set == 0){gettimeofday(&TAS_t1,NULL);TAS_Set=1;}		
		return;
	}
	
	if(Type == Time_Avg_Display){
		if(TAD_Set == 0){gettimeofday(&TAD_t1,NULL);TAD_Set=1;}		
		return;
	}
	
	if(Type == Time_MPI_Barrier){
		if(TMB_Set == 0){gettimeofday(&TMB_t1,NULL);TMB_Set=1;}		
		return;
	}
	
	if(Type == Time_MPI_Send){
		if(TMS_Set == 0){gettimeofday(&TMS_t1,NULL);TMS_Set=1;}		
		return;
	}
	
	if(Type == Time_MPI_Recv){
		if(TMR_Set == 0){gettimeofday(&TMR_t1,NULL);TMR_Set=1;}		
		return;
	}
	
	if(Type == Time_MPI_Scatter){
		if(TM_Scatter_Set == 0){gettimeofday(&TM_Scatter_t1,NULL);TM_Scatter_Set=1;}		
		return;
	}
}

	// Stops a timer based on a defined type above.
void End_Timer(int Type){
	
		// If the time is not started, block from ending.	
	if(Type == Time_Total_Runtime){		
		if(TTR_Set == 1){
			gettimeofday(&TTR_t2,NULL);
			
			TTR_Result = ((TTR_t2.tv_sec-TTR_t1.tv_sec)*1000 + (TTR_t2.tv_usec-TTR_t1.tv_usec)/1000);
		}		
		return;
	}
		
	if(Type == Time_Avg_SingleGen){
		if(TAS_Set == 1){
			gettimeofday(&TAS_t2,NULL);
			
				// Add to the running so it can be averaged later.
			TAS_Running_Total += ((TAS_t2.tv_sec-TAS_t1.tv_sec)*1000 + (TAS_t2.tv_usec-TAS_t1.tv_usec)/1000);
			TAS_Count += 1;
			
			TAS_Set = 0; // reset.
		}		
		return;
	}
	
	if(Type == Time_Avg_Display){
		if(TAD_Set == 1){
			gettimeofday(&TAD_t2,NULL);
			
				// Add to the running so it can be averaged later.
			TAD_Running_Total += ((TAD_t2.tv_sec-TAD_t1.tv_sec)*1000 + (TAD_t2.tv_usec-TAD_t1.tv_usec)/1000);
			TAD_Count += 1;
			
			TAD_Set = 0; // reset.
		}		
		return;
	}
	
	if(Type == Time_MPI_Barrier){
		if(TMB_Set == 1){
			gettimeofday(&TMB_t2,NULL);
			
				// Add the time to the barrior.
			TMB_Result += ((TMB_t2.tv_sec-TMB_t1.tv_sec)*1000 + (TMB_t2.tv_usec-TMB_t1.tv_usec)/1000);
			
			TMB_Set = 0; // Reset.
		}		
		return;
	}
	
	if(Type == Time_MPI_Send){
		if(TMS_Set == 1){
			gettimeofday(&TMS_t2,NULL);
			
				// Add the time to the MPI_Send.
			TMS_Result += ((TMS_t2.tv_sec-TMS_t1.tv_sec)*1000 + (TMS_t2.tv_usec-TMS_t1.tv_usec)/1000);
			
			TMS_Set = 0; // Reset.
		}		
		return;
	}
	
	if(Type == Time_MPI_Recv){
		if(TMR_Set == 1){
			gettimeofday(&TMR_t2,NULL);
			
			TMR_Result += ((TMR_t2.tv_sec-TMR_t1.tv_sec)*1000 + (TMR_t2.tv_usec-TMR_t1.tv_usec)/1000);
			
			TMR_Set = 0; // Reset.
		}		
		return;
	}
	
	if(Type == Time_MPI_Scatter){
		if(TM_Scatter_Set == 1){
			gettimeofday(&TM_Scatter_t2,NULL);
			
			TM_Scatter_Result += ((TM_Scatter_t2.tv_sec-TM_Scatter_t1.tv_sec)*1000 + (TM_Scatter_t2.tv_usec-TM_Scatter_t1.tv_usec)/1000);
			
			TM_Scatter_Set = 0; // Reset.
		}		
		return;
	}
}

	// This does any processing required and prints it to the console.
void Timer_Print(){
	
		// Prevent divide by zero.
	if(TAS_Count != 0 && TAD_Count != 0){
	
			// Calculate the averages.
		TAS_Result = TAS_Running_Total / TAS_Count;
		TAD_Result = TAD_Running_Total / TAD_Count;	
	}

	
			
	int i;
	
		// Print each node one at a time.
	for(i = 0; i < NumberOfProcessors; i++) {
		
		MPI_Barrier(MPI_COMM_WORLD);
		if (i == rank) {
				// Print it all as one printf to help prevent it break apart.
			printf("Rank: %d\n    Total runtime: %d\n    Avg single generation: %d\n    Avg display: %d\n    MPI_Barrier: %d\n    MPI_Send: %d\n    MPI_Recv: %d\n    MPI_Scatter: %d\n\n",rank, TTR_Result, TAS_Result, TAD_Result, TMB_Result, TMS_Result, TMR_Result, TM_Scatter_Result);
			
				// Print it as keywords instead of full name.
			//printf("Rank: %d\n    TTR: %d\n    TAS: %d\n    TAD: %d\n    TMB: %d\n    TMS: %d\n    TMR: %d\n\n",rank, TTR_Result, TAS_Result, TAD_Result, TMB_Result, TMS_Result, TMR_Result);
		}
	}
}

	// Set up the timing stuff (not really used).
void InitTiming(){}