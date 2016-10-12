default:
	rm -f david_harkins
	mpicc gameoflife.c -w -c
	mpicc main.c -w -c
	mpicc matrix.c -w -c
	mpicc network.c -w -c
	mpicc simulate.c -w -c
	mpicc timing.c -w -c
	mpicc gameoflife.o timing.o simulate.o network.o matrix.o main.o -o david_harkins
	

result: default
	mpirun -np 4 ./david_harkins 100 4 10 >> results/4.txt
	mpirun -np 4 ./david_harkins 100 8 10 >> results/8.txt
	mpirun -np 4 ./david_harkins 100 16 10 >> results/16.txt
	mpirun -np 4 ./david_harkins 100 32 10 >> results/32.txt
	mpirun -np 4 ./david_harkins 100 64 10 >> results/64.txt
	mpirun -np 4 ./david_harkins 100 128 10 >> results/128.txt
	mpirun -np 4 ./david_harkins 100 256 10 >> results/256.txt
	mpirun -np 4 ./david_harkins 100 512 10 >> results/512.txt
	mpirun -np 4 ./david_harkins 100 1024 10 >> results/1024.txt	
	mpirun -np 4 ./david_harkins 100 2048 10 >> results/2048.txt
	mpirun -np 4 ./david_harkins 100 4096 10 >> results/4096.txt
	mpirun -np 4 ./david_harkins 100 8192 10 >> results/8192.txt
	mpirun -np 4 ./david_harkins 100 16384 10 >> results/16384.txt