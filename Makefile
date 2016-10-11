default:
	rm -f david_harkins
	mpicc gameoflife.c -c
	mpicc main.c -c
	mpicc matrix.c -c
	mpicc network.c -c
	mpicc simulate.c -c
	mpicc gameoflife.o simulate.o network.o matrix.o main.o -o david_harkins
	mpirun -np 4 ./david_harkins 100 8 2
