#ifndef TIMING_H
#define TIMING_H

void InitTiming();

void Start_Timer(int Type);
void End_Timer(int Type);
void Timer_Print();

#define Time_Total_Runtime 0
#define Time_Avg_SingleGen 1
#define Time_Avg_Display   2
#define Time_MPI_Barrier   3
#define Time_MPI_Send      4
#define Time_MPI_Recv      5
#define Time_MPI_Scatter   6




#endif