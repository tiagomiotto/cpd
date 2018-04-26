#include <stdio.h>
#include <mpi.h>

int main (int argc, char *argv[]) {

  

    MPI_Init (NULL, NULL);
	//pid
   
    //machine id
	int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);


	char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);
    
    printf("Process %d sends greetings from machine %s\n " ,world_rank,processor_name);
    
    MPI_Finalize();
    
    return 0;
} 
