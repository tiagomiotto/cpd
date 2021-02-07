#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include <mpi.h>

int bactrack_serial(int **puzzle, int size);
int row_valid(int** total,int row, int num,int tam);
int collum_valid(int** total,int collum, int num,int tam);
int sq_valid(int** total, int row, int collum,int num, int tam );
int is_valid(int** total, int row, int collum,int num, int tam );
void  printm(int** matrix,int size);
int flag=0;


int main(int argc, char **argv){
	int size,i,j,send_id,pos_id,resp,tag=123,a,tag2=321;
	char *file;
	int val;
	int id,p;
	int fim=0;
	int ini=1;
	int **matrix;
	
	MPI_Init(NULL, NULL);
	MPI_Comm_rank (MPI_COMM_WORLD, &id);
	MPI_Comm_size (MPI_COMM_WORLD, &p);
	
	MPI_Request request;
	MPI_Status status;
	
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
// 	printf("hi mates id=%d\n",id);
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
		
		printm(matrix,size*size);
		printf("\n");
	}
// 	MPI_Barrier(MPI_COMM_WORLD);
	while(fim==0){
		if (id==0 && ini==1){
			ini=0;
			resp=bactrack_serial(matrix, size*size);
			if(resp==0){
				a=0;
				printf("sol id=%d enviar para %d\n",id,send_id);
				MPI_Isend(&a, 1, MPI_INT, send_id, tag, MPI_COMM_WORLD,&request);
				printf("sol id=%d espera por %d\n",id,pos_id);
				MPI_Recv(&a,1,MPI_INT , pos_id, tag, MPI_COMM_WORLD, &status);
				fim=1;
			}else if(resp==1){
				printf("vou sair id=%d enviar para %d\n",id,send_id);
				a=0;
				MPI_Isend(&a, 1, MPI_INT, send_id, tag, MPI_COMM_WORLD,&request);
				fim=1;
			}
		}else{
			a=1;
			MPI_Isend(&a, 1, MPI_INT, send_id, tag, MPI_COMM_WORLD,&request);
			printf("pedido de id= %d para send_id= %d\n",id,send_id);
			MPI_Probe(send_id, tag, MPI_COMM_WORLD, &status);
			MPI_Get_count(&status, MPI_INT, &size);
			size=sqrt(size);
			
			
			int *data = (int *)malloc(size*size*sizeof(int));
			matrix= (int **)malloc(size*sizeof(int*));
			for ( i=0; i<size; i++)
				matrix[i] = &(data[size*i]);
			
			MPI_Recv(&(matrix[0][0]),size*size,MPI_INT , send_id, tag, MPI_COMM_WORLD, &status);
			printf("recebo size=%d  id=%d  from=%d\n",size,id,send_id);
			resp=bactrack_serial(matrix, size);
			free(matrix[0]);
			free(matrix);
			if(resp==0){
				a=0;
				printf("sol id=%d enviar para %d\n",id,send_id);
				MPI_Isend(&a, 1, MPI_INT, send_id, tag, MPI_COMM_WORLD,&request);
				printf("Sol id=%d espera por %d\n",id,pos_id);
				MPI_Recv(&a,1,MPI_INT , pos_id, tag, MPI_COMM_WORLD, &status);
				fim=1;
			}else if(resp==1){
				a=0;
				printf("vou sair id=%d enviar para %d\n",id,send_id);
				MPI_Isend(&a, 1, MPI_INT, send_id, tag, MPI_COMM_WORLD,&request);
				fim=1;
			}
		}
	}
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	return 0;
}



int bactrack_serial(int **puzzle, int size)
{
    
    int i, j, k,k1, found=0, temp=0,p,tag=123,tag2=321;
    int a, b,id,get_id,envia=0;
	
	
    MPI_Comm_rank (MPI_COMM_WORLD, &id);
	MPI_Comm_size (MPI_COMM_WORLD, &p);
	MPI_Status status;
	MPI_Request request;
	tag=123;
	
	if(id==p-1){
		get_id=0;
	}else{
		get_id=id+1;
	}

    //allocate a matrix
    int **stable = (int **)malloc(size * sizeof(int *));
    for(i = 0; i < size; i++) stable[i] = (int *)malloc(size * sizeof(int));
    
    //Check what values are lock
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
						if (!flag)MPI_Iprobe(get_id, tag, MPI_COMM_WORLD, &flag , &status);
						if (flag){
							for(k1=k+1;k1<size;k1++){
								 if(is_valid(puzzle,k1+1, i, j, size)==1){
									flag=0;
									envia=1;
									puzzle[i][j]=k1+1;
									MPI_Irecv(&a,1,MPI_INT , get_id, tag, MPI_COMM_WORLD, &request);
									if (a==0)return 1;
									printf("enviar copia de %d para %d (%d,%d)\n",id,get_id,i,j);
									
									int *data = (int *)malloc(size*size*sizeof(int));
									int **matrix= (int **)malloc(size*sizeof(int*));
									for (int i1=0; i1<size; i1++)
										matrix[i1] = &(data[size*i1]);
									
									for (int i1 = 0; i1 < size; i1++){
										for(int j1 = 0; j1 < size; j1++) {
											matrix[i1][j1]= puzzle[i1][j1];
// 											printf(" %d (%d,%d)",id,i1,j1);
										}
									}
// 									printf("\n");
									matrix[i][j]=k+1;
									MPI_Send(&(matrix[0][0]),size*size,MPI_INT ,get_id, tag, MPI_COMM_WORLD);
									free(matrix[0]);
									free(matrix);
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
                            printf("This puzzle has no solution!!\n");
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
    printm(puzzle,size);
    return 0;

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
