#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <omp.h>
#include "verify.h"

int bactrack(int **puzzle,  int size);

int main(int argc, char **argv){
    int size,i,j,Nthreads;
	char *file;
	int val;
    int attempt=0, backtracks=0;
    
    omp_set_nested;
    Nthreads = 8;
    omp_set_num_threads(Nthreads);
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
	
	int **matrix = (int **)malloc(size * size * sizeof(int *));
	for(i = 0; i < size * size; i++) matrix[i] = (int *)malloc(size * size * sizeof(int));
	for (i = 0; i < size * size; i++){
		for(j = 0; j < size * size; j++) {
			fscanf(stream, "%d", &val);
			matrix[i][j]=val;
		}
	}
	
	for (i = 0; i < size * size; i++){
		for(j = 0; j < size * size; j++) {
			printf("%d ",matrix[i][j]);
		}
		printf("\n");
	}
    printf("\n");
    
    if(bactrack(matrix, size*size)==-1){
        return -1;
    }

    for (i = 0; i < size * size; i++){
        for(j = 0; j < size * size; j++) {
            printf("%d ",matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    printf("Backtracks:%d and %d Attempts\n",backtracks,attempt);
    
}



int bactrack(int **puzzle, int size)
{
    
    
    int i, j, k, found=0, temp=0;
    int a, b;
    

    //allocate a matrix
    int **stable = (int **)malloc(size * sizeof(int *));
    for(i = 0; i < size; i++) stable[i] = (int *)malloc(size * sizeof(int));
    
//     printf("%d %d\n\n",omp_get_num_threads(),omp_get_max_threads());
    
    
    
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
                
//                     printf("olaaa\n");
                    if (omp_get_num_threads()<omp_get_max_threads())
                    {
                        int i1,i2,i3,sai=0;
                        int **matrix ;
                        printf("threads novas! %d %d\n\n",omp_get_num_threads(),omp_get_max_threads());
//                         #pragma omp parallel num_threads (-omp_get_num_threads()+omp_get_max_threads()+1)
//                         {
                    #pragma omp parallel for/* nowait*/private(matrix,i1,i2,i3) num_threads (-omp_get_num_threads()+omp_get_max_threads()+1)
                        for(k=puzzle[i][j];k<size;k++)
                        { 
                            //Check for valid values
                            matrix = (int **)malloc(size * size * sizeof(int *));
                            for(int i1 = 0; i1 < size ; i1++) matrix[i1] = (int *)malloc(size * size * sizeof(int));
                            
                            for(i2 = 0; i2 < size ; i2++){
                                for(i3 = 0; i3 < size ; i3++){
                                    matrix [i2][i3]= puzzle[i2][i3];
                            } 
                            }
                            
                            if(is_valid(puzzle,k+1, i, j, size)==1)
                            {
                                matrix[i][j]=k+1;
                                printf("eu sou %d %d %d e vou tentar o %d\n",omp_get_thread_num(),i ,j,k+1);
                                if(bactrack(matrix, size)==-1){
                                    printf("eu sou %d %d %d e nao deu certo o %d\n",omp_get_thread_num(),i ,j,k+1);
                                }else{
                                    printf("eu sou %d %d %d e deu certo o %d :D\n",omp_get_thread_num(),i ,j,k+1);
                                    for(i2 = 0; i2 < size ; i2++){
                                        for(i3 = 0; i3 < size ; i3++){
                                            printf("%d ",matrix [i2][i3]);
                                    } 
                                    printf("\n");
                                }
                                printf("\n");
                                sai=1;
                                }
                            }else{
                                printf("eu sou %d %d %d e nao é valido o %d\n",omp_get_thread_num(),i ,j,k+1);
                            }
                            
                        }
//                         }
                        if (sai==1){
                            return 0;
                        }
                    }else{
                for(k=puzzle[i][j];k<size;k++)
                    {
                        //Check for valid values                            
                        if(is_valid(puzzle,k+1, i, j, size)==1)
                        {
                            //Set value on the puzzle
                            puzzle[i][j]=k+1;
                            break;
                        }
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
                    temp=puzzle[i][j];
                    
                    //Go up on the possible values
                    if(stable[i][j]==0){
                    for(k=puzzle[i][j];k<size;k++)
                    {
                        //Check if the value is valid
                        if(is_valid(puzzle, k+1,i, j,size)==1 )
                        {
                            //If it is value, update and set flag
                            found=1;
                            puzzle[i][j]=k+1;
                            break;
                        }
                    }
                    }
                    //if no valid value was found set to zero and continue moving back
                    if(found==0){
                        puzzle[i][j]=0;
                        continue;
                    }
                }
            //}
        }
        
    }}
    return 0;
}

