default:
	rm -f david_harkins
	mpicc gameoflife.c -w -c
	mpicc main.c -w -c
	mpicc matrix.c -w -c
	mpicc network.c -w -c
	mpicc simulate.c -w -c
	mpicc timing.c -w -c
	mpicc gameoflife.o timing.o simulate.o network.o matrix.o main.o -o david_harkins
	mpirun -np 4 ./david_harkins 100 8 2

result:
	mpirun -np 4 ./david_harkins 100 8 2
	mpirun -np 4 ./david_harkins 100 8 2
	mpirun -np 4 ./david_harkins 100 8 2
	mpirun -np 4 ./david_harkins 100 8 2
	mpirun -np 4 ./david_harkins 100 8 2
	mpirun -np 4 ./david_harkins 100 8 2
	mpirun -np 4 ./david_harkins 100 8 2
	mpirun -np 4 ./david_harkins 100 8 2
	mpirun -np 4 ./david_harkins 100 8 2
	mpirun -np 4 ./david_harkins 100 8 2
	mpirun -np 4 ./david_harkins 100 8 2
	mpirun -np 4 ./david_harkins 100 8 2
	mpirun -np 4 ./david_harkins 100 8 2
