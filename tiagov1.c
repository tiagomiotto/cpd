#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include <mpi.h>
#define FIR_DIV 4

int bactrack_serial(int **puzzle, int size, int** stabl2);
int row_valid(int** total,int row, int num,int tam);
int collum_valid(int** total,int collum, int num,int tam);
int sq_valid(int** total, int row, int collum,int num, int tam );
int is_valid(int** total, int row, int collum,int num, int tam );
void  printm(int** matrix,int size);
void freematrix(int **puzzle, int size);
int flag=0; //Received a message from the neighbourg
int flag1=0; //Active when someone requested work from me
int flag2=0; //Received work from neighbourg
int flag3=0; 
int size_master=0; //Copia master do size pra nao dar erro em frees a toa


int main(int argc, char **argv){
	int size,i=0,j=0,send_id,pos_id,resp,tag=123,a[2],tag2=321,k1;
	char *file;
	int val;
	int id,p,point;
	int fim=0,ifinish=0;
	int ini=1;
	int **matrix;
	MPI_Init(NULL, NULL);
	MPI_Comm_rank (MPI_COMM_WORLD, &id);
	MPI_Comm_size (MPI_COMM_WORLD, &p);
	
	int *lucky;
	int *first_sends; 
	MPI_Request request;
	MPI_Status status,status2,status3;

	if(id==0){
		send_id=p-1;
	}else{
		send_id=id-1;
	}
	if(id==p-1){
		pos_id=0;
	}else{
		pos_id=id+1;
	}

	/* Read the file containing the sudoku*/
	if(id==0){
		FILE *stream;
		if(argc != 2){ 
			printf("erro nos argumentos\n");
			exit(1);
		}else{
			file = strchr(argv[1], '.');
			if(strcmp(file, ".txt") != 0){
				printf("erro no formato de ficheiro\n");
				exit(1);
			}
			stream = fopen(argv[1], "r");	
			if(stream == NULL){
				perror("Error");
				exit(1);
			}	
		}
		fscanf(stream, "%d", &size); 
		
		int *data = (int *)malloc(size*size*size*size*sizeof(int));
		matrix= (int **)malloc(size*size*sizeof(int*));
		for ( i=0; i<size*size; i++)
			matrix[i] = &(data[size*size*i]);
		
		for (i = 0; i < size * size; i++){
			for(j = 0; j < size * size; j++) {
				fscanf(stream, "%d", &val);
				matrix[i][j]=val;
			}
		}
		
	
		size_master=size*size; 
	}

	int divisions = p/FIR_DIV-1; //Number of initial divisions

	/*Saves the ones that will receive first and sort them in a binary search fashion*/
	if(id==0)
	{

	lucky=malloc((p/FIR_DIV)*sizeof(int)); 
	int x=0;
	for (int aux_id = 0; aux_id < p; ++aux_id) //Choose first one to recv
	{

	 if(aux_id % FIR_DIV == 0 && aux_id != 0 && (aux_id/FIR_DIV)<=divisions){
	   lucky[x]=aux_id; 
	   x++;	
	 }	
		
	}

	//order the receivers in a binary sorting manner
	first_sends=malloc((p/FIR_DIV)*sizeof(int)); 
	first_sends[0]=lucky[(divisions/2)];
	int j=1;
	for (int i = 1; i < divisions; i++) 
	{
		first_sends[j]=lucky[divisions-i];
		j+=2;
	}
	j=0;	
	for (int i = 2; i < divisions; i+=2)
	{	
		first_sends[i]=lucky[i-2-j];
		j++;

	}
	free(lucky);
	}


	int contador=0;
	int senders =1;
	int first_save,first_i,first_j,k; //Saves the postion and value master will start at
	/*Master sends to the first ones to receive*/
	if(id == 0) 
	{

        size=size*size;

		i=0;j=0;
	    
	    while(senders <= divisions )
	    {
			flag=0;
	        if(matrix[i][j] == 0)
	        {
	            for(k = 0; k < size; k++)
	            { 
	                if(is_valid(matrix, k + 1, i, j, size) == 1)
	                { 	
						for(k1 = k + 1; k1 < size; k1++)
						{
							if(is_valid(matrix, k1 + 1, i, j, size) == 1)
							{
								int get_id2 = first_sends[senders-1];
								MPI_Probe(get_id2, tag, MPI_COMM_WORLD, &status);
								MPI_Recv(a, 2, MPI_INT , get_id2, tag, MPI_COMM_WORLD, &status);
							
								matrix[i][j] = k + 1;
								MPI_Send(&(matrix[0][0]), size * size, MPI_INT , get_id2, tag, MPI_COMM_WORLD);
							
								matrix[i][j] = 0;
								senders++;
								if(senders>=divisions){ //Saves the starting point for the master
									flag=1;
									matrix[i][j]=k1+1;
									first_i=i;
									first_j=j;
									break;
								}else{
									k=k1;
								}
							}
			
							if(senders >= divisions) {	
								break;
							}
						}
						if(flag==0)matrix[i][j]=k+1; 
					}	
					if(senders > divisions || k1==size-1) break;  
				}
			}
			if(senders > divisions )break;
			if(j == size-1) //In case no other possibilities exist for the current cell, go to the next
			{
				i++;
				j = 0;
			}
			else j++;
	
		}

	 
	  size= sqrt(size);

	    ini=1;
	}
	/*Code for the first nodes who will recv*/
	if (id % FIR_DIV == 0 && id != 0 && (id/FIR_DIV)<=divisions) 

	{
	    a[0] = 1;
	    if (id < a[1])a[1] = id;
	    a[1]=id;

	    MPI_Isend(&a, 2, MPI_INT, 0, tag, MPI_COMM_WORLD, &request);

	    

	        MPI_Probe(0, tag, MPI_COMM_WORLD, &status);
	        MPI_Get_count(&status, MPI_INT, &size);
	        size = sqrt(size);
	        int *data = (int *)malloc(size * size * sizeof(int));

	        matrix = (int **)malloc(size * sizeof(int *));
	        for ( i = 0; i < size; i++)
	            matrix[i] = &(data[size * i]);
	
	        MPI_Recv(&(matrix[0][0]), size * size, MPI_INT , 0, tag, MPI_COMM_WORLD, &status);
	 
	        size=sqrt(size);


	    ini = 1;
	}
	else if(id!=0)ini = 0;
 	MPI_Barrier(MPI_COMM_WORLD);
	while(fim==0){

		if (ini == 1) //Startup code for all the ones that already have a sudoku board
		{
		    ini = 0;
		
		    if(id != 0){ //Not master
		    	resp = bactrack_serial(matrix, size * size, NULL);
		    }

		    else
		    { //Master
		    	size=size*size;
		        //Create special stable matrix for master so it can backtrack to the first postion
		        int **stable = (int **)malloc(size * sizeof(int *));
		        for(i = 0; i < size; i++) stable[i] = (int *)malloc(size * sizeof(int));
		
		
		        //Check what values are locked
		            for(i = 0; i < size; i++)
		            {
		                for(j = 0; j < size; j++)
		                {
		                	if(i==first_i && j== first_j){
		                		stable[i][j]=0;
		                	}
		                	else{
		                    if(matrix[i][j] == 0)
		                        stable[i][j] = 0;
		                    else
		                        stable[i][j] = 1;
		                }
		                }
		            }
		        size=sqrt(size);

		        resp = bactrack_serial(matrix, size * size, stable);
		    }
			if(resp==0){ //In case a solution is found
				a[0]=0;
				a[1]=p;
				
				MPI_Send(&a, 2, MPI_INT, send_id, tag, MPI_COMM_WORLD);
				
                                
				MPI_Recv(&a,2,MPI_INT , pos_id, tag, MPI_COMM_WORLD, &status);

				ifinish=1;
				fim=1;
			}else if(resp==1){ //In case another node sent me a termination message, propagate and terminate
				
				a[0]=0;
				MPI_Isend(&a, 2, MPI_INT, send_id, tag, MPI_COMM_WORLD,&request);
				fim=1;
			}
		}
		/*Code for the ones that haven't yet received a sudoku board*/
		/*They will ask the left neighbourg and wait until the request is fulfiled*/
		else{ 
			a[1]=id;
			MPI_Iprobe(pos_id, tag, MPI_COMM_WORLD, &flag1 , &status2);
			if(flag1==1){
					MPI_Recv(&a,2,MPI_INT , pos_id, tag, MPI_COMM_WORLD, &status2);
					if (a[0]==1){
						flag=1;
						flag1=0;
						flag3=1;
					}
				}
			a[0]=1;
			if (id<a[1])a[1]=id;
			point=a[1];
			MPI_Isend(&a, 2, MPI_INT, send_id, tag, MPI_COMM_WORLD,&request);
			
			while(flag1==0 && flag2==0 && fim==0){
				MPI_Iprobe(pos_id, tag, MPI_COMM_WORLD, &flag1 , &status2);
				MPI_Iprobe(send_id, tag, MPI_COMM_WORLD,&flag2 , &status);
				if(flag1==1){
					MPI_Recv(&a,2,MPI_INT , pos_id, tag, MPI_COMM_WORLD, &status2);
					if (a[0]==1){
						flag=1;
						flag1=0;
						flag3=1;
						if(a[1]==id){ //In case the no solution message circles the ring, print no solution and finish
							flag2=0;
							fim=1;
							a[0]=0;
							
							MPI_Send(&a, 2, MPI_INT, send_id, tag, MPI_COMM_WORLD);
							MPI_Probe(pos_id, tag, MPI_COMM_WORLD, &status3);
							MPI_Recv(&a,2,MPI_INT , pos_id, tag, MPI_COMM_WORLD, &status3);
							printf("No solution\n");
							
						}
						if(a[1]<point){
							point=a[1];
							a[0]=1;
							MPI_Isend(&a, 2, MPI_INT, send_id, tag, MPI_COMM_WORLD,&request);
						}
					}else{
						flag2=0;
						
					}
				}
				
			}
			if (flag1==1){//The left neigbourgh sent an termination message
				
				a[0]=0;
				MPI_Isend(&a, 2, MPI_INT, send_id, tag, MPI_COMM_WORLD,&request);
				fim=1;
				flag2=0;
			}
			if (flag2==1){ //The left neighbourg sent work
				point=id;
				a[1]=id;
			
				MPI_Probe(send_id, tag, MPI_COMM_WORLD, &status);
				MPI_Get_count(&status, MPI_INT, &size);
				size=sqrt(size);
				
			
				int *data = (int *)malloc(size*size*sizeof(int));
				matrix= (int **)malloc(size*sizeof(int*));
				for ( i=0; i<size; i++)
					matrix[i] = &(data[size*i]);
				
				MPI_Recv(&(matrix[0][0]),size*size,MPI_INT , send_id, tag, MPI_COMM_WORLD, &status); 
				

				resp=bactrack_serial(matrix, size, NULL); //Start working
				
				if(resp==0){ //In case a solution is found
					a[0]=0;
					
					MPI_Send(&a, 2, MPI_INT, send_id, tag, MPI_COMM_WORLD);
					
					MPI_Recv(&a,2,MPI_INT , pos_id, tag, MPI_COMM_WORLD, &status);
					ifinish=1;
					size=sqrt(size);
					fim=1;
				}else if(resp==1){ //In case another node made me terminate calculation and exit
					MPI_Iprobe(send_id, tag, MPI_COMM_WORLD, &flag1 , &status2);
					if(flag1==1)MPI_Recv(&a,2,MPI_INT , send_id, tag, MPI_COMM_WORLD, &status2);
					a[0]=0;
					

					MPI_Isend(&a, 2, MPI_INT, send_id, tag, MPI_COMM_WORLD,&request);
					
					ifinish=0;
					fim=1;
				}
			}
			flag1=0;
			flag2=0;

		}
	}
	MPI_Barrier(MPI_COMM_WORLD); //Wait for all the nodes
	if(ifinish==1){ //If i found the solution, print
		if(size!=size_master)size=size_master;
				printm(matrix,size);
				free(matrix[0]);free(matrix);
	}
	
	MPI_Finalize();
	return 0;
}



int bactrack_serial(int **puzzle, int size, int** stable2)
{
    
    int i, j, k,k1, found=0, temp=0,p,tag=123;
    int a[2], b,id,get_id,envia=0;
	size_master=size;
	
    MPI_Comm_rank (MPI_COMM_WORLD, &id);
	MPI_Comm_size (MPI_COMM_WORLD, &p);
	MPI_Status status;
	MPI_Request request;
	tag=123;
	
	//Calculate the id of the node's left neighbourg
	if(id==p-1){  
		get_id=0;
	}else{
		get_id=id+1;
	}

    int **stable = (int **)malloc(size * sizeof(int *));
    for(i = 0; i < size; i++) stable[i] = (int *)malloc(size * sizeof(int));
    
	
    //The node is not the master, so it doesnt need a new matrix to say which values are locked
    if(stable2==NULL){
    for(i=0;i<size;i++)
    {
        for(j=0;j<size;j++)
        {
            if(puzzle[i][j]==0)
                stable[i][j]=0;
            else
                stable[i][j]=1;
        }
    }
}
    else { //In case master
   	for(i=0;i<size;i++)
    {
        for(j=0;j<size;j++)
        {
            stable[i][j]=stable2[i][j];
        }
    }
	freematrix(stable2,size);}
 
    

    //Move vertically
    
    for(i=0;i<size;i++)
    {
        
        //Move side ways
        for(j=0;j<size;j++)
        {
            if (stable[i][j]==0){
                //Increment values until finds a valid bvalue
                for(k=puzzle[i][j];k<size;k++)
                {

                    //Check for valid values
                    if(is_valid(puzzle,k+1, i, j, size)==1)
                    {
						if (flag==0)MPI_Iprobe(get_id, tag, MPI_COMM_WORLD, &flag , &status); //Check if there is a request from the neighbourg
						if (flag==1 && i<=size*3/4){ //Stop sending after the 3/4*size line, to lower the number of requests
							for(k1=k+1;k1<size;k1++){
								 if(is_valid(puzzle,k1+1, i, j, size)==1){
									flag=0;
									envia=1;
									a[0]=1;
									if (flag3==0)MPI_Irecv(&a,2,MPI_INT , get_id, tag, MPI_COMM_WORLD, &request); 
									if (a[0]==0){  //Received a termination signal
										freematrix(stable,size);
										free(puzzle[0]);
										free(puzzle);
										return 1;
									}
									flag3=0;
								
									puzzle[i][j]=k+1;
									MPI_Send(&(puzzle[0][0]),size*size,MPI_INT ,get_id, tag, MPI_COMM_WORLD); //Send worrk
									puzzle[i][j]=k1+1;
									break;
								}
							}
						}
                        //Set value on the puzzle
                        if(envia==0)
						{
							puzzle[i][j]=k+1;
							break;
						}
						envia=0;
                    }
                }
            
                while(puzzle[i][j]==0)
                {
                    //No valid options found
                    found=0;
                    //Go back in the row
                    temp=j-1;
                    //Keep inside of the matrix
                    if(temp!=-1){
                        
                        //Go back until a value that can be change is found
                        for(j=temp;j>=0;j--)
                        {
                            //Check if the value can be change
                            if(stable[i][j]==0)
                            {
                                //Flag that found a value and stop going backwards
                                found=1;
                                break;
                            }
                        }
                    }
                    
                    
                    
                    if(found==0)
                    {
                        //If there isnt a value to be change in the row go back one collum
                        temp=i-1;
                        //Keep incide the bounderies
                        if(temp!=-1){
                            //Move back on the collums untils finds value to be change
                            for(i=temp;i>=0;i--)
                            {
                                
                                //starting on the right find a value to be change
                                for(j=size-1;j>=0;j--)
                                {
                                    //if value can be change flag and stop moving on the sideways
                                    if(stable[i][j]==0)
                                    {
                                        found=1;
                                        break;
                                    }
                                }
                                //value was found stop moving vertically
                                if(found==1)
                                    break;
                            }
                        } else {
				freematrix(stable,size);
				free(puzzle[0]);
				free(puzzle);
                            //printf("This puzzle has no solution!! id=%d\n",id);
                            return -1;
                        }
                    }
                    
                    //Reset flag
                    found=0;
                    //set the last value
					if(i<0)i=0;
                    if(j<0)j=0;
                    temp=puzzle[i][j];
                    
                    //Go up on the possible values
                    for(k=puzzle[i][j];k<size;k++)
                    {
                        //Check if the value is valid
                        if(is_valid(puzzle, k+1,i, j,size)==1 && stable[i][j]==0)
                        {
                            //If it is value, update and set flag
                            found=1;
                            puzzle[i][j]=k+1;
                            break;
                        }
                    }
                    //if no valid value was found set to zero and continue moving back
                    if(found==0){
                        puzzle[i][j]=0;
                        continue;
                    }
                }
            }
        }
        
    }
   // printm(puzzle,size);
 	freematrix(stable,size);
    return 0;

}

void freematrix(int **puzzle, int size){
	for (int i=0;i<size; i++)
		free(puzzle[i]);
	free(puzzle);
}

int row_valid(int** total,int row, int num,int tam){

	int i;
	for (i = 0; i < tam; ++i)
	{
		if(total[row][i]==num) return 0;
	}
	return 1;
}
int collum_valid(int** total,int collum, int num, int tam){

	int i;
	for (i = 0; i < tam; ++i)
	{
		if(total[i][collum]==num) return 0;
	}
	return 1;
}
//https://cboard.cprogramming.com/c-programming/150917-how-check-square-block-condition-sudoku-its-validity.html
int sq_valid(int** total, int row, int collum,int num, int tam ){
	int i,j,aux_col,aux_row;
	double aux;
	//Calcula o tamanho do  quadrado pequeno
	aux=sqrt(tam); 

	// Define a posição do elemento no canto superior esquerdo do subquadrado
	for(i=0;i<aux;i++) 
	{
		if((row/aux)<i+1){
			aux_row=aux*i;
			break;
		}
	}

	for(i=0;i<aux;i++)
	{
		if((collum/aux)<i+1){
			aux_col=aux*i;
			break;
		}
	}
	

	//Verifica o subquadrado
	for(i = aux_row; i < aux_row+aux; i++)
       for(j = aux_col; j < aux_col+aux; j++)
            if(total[i][j]==num)    return 0;

   return 1;

}
int is_valid(int** total, int num, int row, int collum,int tam ){
	//Paralelizar essas 3
	if((row_valid(total,row,num,tam))==false) return 0;  
	if((collum_valid(total,collum,num,tam))==false) return 0;
	if((sq_valid(total,row,collum,num,tam))==false) return 0;
	return 1;
}


void  printm(int** matrix,int size){
	int i,j;

	    for (i = 0; i < size ; i++){
        for(j = 0; j < size ; j++) {
            printf("%d ",matrix[i][j]);
        }
        printf("\n");
    }
}





bool is_in(int* arr,int size, int num){
	for (int i = 0; i < size; ++i)
	{
		if(arr[i]==num) return true;
	}
	return false;
}
