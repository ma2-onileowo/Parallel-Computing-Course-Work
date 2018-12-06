mpi: mpi.c
	mpicc -c mpi.c mpi -lcrypto -fopenmp
	


	mpirun -nolocal -np 6 -machinefile machinefile mpi

	mpirun -quiet -np 8 BruteForceIfMPI
