#include <stdio.h>
#include <mpi.h>

int main (int argc, char *argv[]) {

	int baf[1000][1000];

    MPI_Init (NULL, NULL);
	//pid
   
    //machine id
	int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	if(world_rank==0) baf[0][0] =1;


    MPI_Bcast(&bag, 1, MPI_INT, root, MPI_COMM_WORLD);
	MPI_Barrier (MPI_COMM_WORLD);
    double secs = - MPI_Wtime();
    printf("[%d]: After Bcast, buf is %d, took %12.6f \n", rank, buf[0][0],secs);
    MPI_Finalize();
    
    return 0;
} 
