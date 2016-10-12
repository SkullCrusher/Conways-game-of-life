#ifndef GAMEOFLIFE_H
#define GAMEOFLIFE_H

#include <stdio.h>
#include <stdint.h> 
#include <mpi.h> 
#include <assert.h>
#include <sys/time.h>
#include <time.h>

	// The function main calls to start everything.
int GameOfLife(int, int, int);

	// MPI Rank and number of nodes.
int rank, p;

	// The local matrix.
int ** x;

	// The number of processors.
int NumberOfProcessors;

	// Matrix size, N by N
int N_global;

	// How many generations.
int Gen;

int Display;

#define ALIVE 1
#define DEAD  0

#endif