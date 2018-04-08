#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <omp.h>
#include "verify.h"

int bactrack(int **puzzle, int size);
void printmatrix(int **puzzle, int size);
int threads = 1;
int sai=0;
omp_lock_t lck_a;

int main(int argc, char **argv){
        int size,i,j,Nthreads;
	char *file;
	int val;
	FILE *stream;
        omp_set_nested(1);
        Nthreads = 8;
        omp_set_num_threads(Nthreads);
        
        omp_init_lock(&lck_a);
        
        double start = omp_get_wtime();
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
	
	
    
    if(bactrack(matrix, size*size)==-1){
        return -1;
    }
if(sai==0)printmatrix(matrix,size*size);
    
//     printf("\n");
omp_destroy_lock(&lck_a);
    double end = omp_get_wtime();
    for (i = 0; i < size * size; i++){
		free(matrix[i]);
	}
	free(matrix);
//     printf("\n");
//     printf("Took  %f  seconds\n", end - start);
}

void printmatrix(int **matrix, int size){
    int i ,j;
    #pragma omp critical
    for (i = 0; i < size; i++){
        for(j = 0; j < size; j++) {
            printf("%d ",matrix[i][j]);
        }
        printf("\n");
    }
}

void changethread(int a)
{
    #pragma omp critical
    {
        if (a==1){
        threads++;}else{threads--;}
//         #pragma omp flush(threads)
    }          
                                        
}

int bactrack(int **puzzle, int size)
{
    
    
    int i, j, k,k1, found=0, temp=0, mythread,fim;
    int a, b,par=0;
    

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
 
    
    mythread=omp_get_thread_num();
//     printf("sou o master %d\n",mythread);
    //Move vertically
    
    for(i=0;i<size;i++)
    {
        //Move side ways
        for(j=0;j<size;j++)
        {
           if(stable[i][j]==0){
                //Increment values until finds a valid bvalue
                for(k=puzzle[i][j];k<size;k++)
                {
                    //Check for valid values
                    if(is_valid(puzzle,k+1, i, j, size)==1)
                    {
//                         if (threads<9)
//                         printf("nthreads %d %d %d\n",threads,omp_get_max_threads(),omp_get_thread_num());
                        
                        if (threads<omp_get_max_threads()){
                            
                            for(k1=k+1;k1<size;k1++)
                            {
                                if(is_valid(puzzle,k1+1, i, j, size)==1){
                                    omp_set_lock(&lck_a);
//                                     if (threads<omp_get_max_threads())
                                    #pragma omp parallel  if(threads<(omp_get_max_threads())) 
                                    {   
                                        omp_unset_lock(&lck_a);
                                    #pragma omp sections nowait
                                    {       
                                        #pragma omp section 
                                        {
                                            par=1;
                                            if(mythread!=omp_get_thread_num()){changethread(1);}
//                                             threads++;
//                                         printf("nova thread %d\n",omp_get_thread_num());
                                         
                                            int i1,i2,i3;
                                            int **matrix ;
                                            matrix = (int **)malloc(size * size * sizeof(int *));
                                            for(int i1 = 0; i1 < size ; i1++) matrix[i1] = (int *)malloc(size * size * sizeof(int));
                                            
                                            for(i2 = 0; i2 < size ; i2++){
                                                for(i3 = 0; i3 < size ; i3++){
                                                    matrix [i2][i3]= puzzle[i2][i3];
                                                } 
                                            }
                                            matrix [i][j]=k+1;
                                            if(bactrack(matrix, size)==-1){
//                                              printf("thread %d e deu mal (%d,%d)->%d\n",omp_get_thread_num(),i,j,matrix [i][j]);
                                            }else{
                                                if (sai==0){
//                                              printf("thread %d e deu bem (%d,%d)->%d\n",omp_get_thread_num(),i,j,matrix [i][j]);
                                                printmatrix(matrix,size);}
                                                sai=1;
                                                #pragma omp flush(sai)
                                            }
                                            for(i2 = 0; i2 < size ; i2++){
                                                    free(matrix [i2]);  
                                            }
                                            free(matrix);
                                                if(mythread!=omp_get_thread_num()){changethread(-1);}
//                                             threads--;
                                        
//                                         printf("morre thread %d\n",omp_get_thread_num());
                                        }
                                        #pragma omp section
                                        {
                                            if(mythread!=omp_get_thread_num()){changethread(1);}
//                                             threads++;
//                                             changethread(1);
                                            int i1,i2,i3;
                                            int **matrix ;
                                            matrix = (int **)malloc(size * size * sizeof(int *));
                                            for(int i1 = 0; i1 < size ; i1++) matrix[i1] = (int *)malloc(size * size * sizeof(int));
                                            
                                            for(i2 = 0; i2 < size ; i2++){
                                                for(i3 = 0; i3 < size ; i3++){
                                                    matrix [i2][i3]= puzzle[i2][i3];
                                                } 
                                            }
                                            matrix[i][j]=k1+1;
                                            if(bactrack(matrix, size)==-1){
//                                              printf("thread %d e deu mal (%d,%d)->%d\n",omp_get_thread_num(),i,j,matrix [i][j]);
                                            }else{
                                               if (sai==0){
//                                              printf("thread %d e deu bem (%d,%d)->%d\n",omp_get_thread_num(),i,j,matrix [i][j]);
                                                printmatrix(matrix,size);
                                                   }
                                                sai=1;
                                                #pragma omp flush(sai)
                                            }
                                            for(i2 = 0; i2 < size ; i2++){
                                                    free(matrix [i2]);
                                            }
                                            free(matrix);
//                                            threads--;
                                            if(mythread!=omp_get_thread_num()){changethread(-1);}

                                        }
                                        
                                    }
                                    
                                    }
                                    omp_unset_lock(&lck_a);
                                    
                                    break;
                                }
                            }
                            if (par==0){puzzle[i][j]=k+1;}else{par=0;k=k1;}
                            if(sai==1){
                                        
                                        break;
                                    }
                        }else{
                            //Set value on the puzzle
                            puzzle[i][j]=k+1;
                            break;
                        }
                        if (puzzle[i][j]>=k+1)break;
                    }
                }
            
                while(puzzle[i][j]==0 && sai==0)
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
//                             printf("This puzzle has no solution!!\n");
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
                if (sai==1){
                    i=size;j=size;
                }
            }
        }
        
    }
    return 0;
}