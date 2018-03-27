#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "verify.h"

void bactrack(int **puzzle, int *attempt, int *backtracks, int size);
int checkValid(int **puzzle,int i,int j,int value, int size);

int main(int argc, char **argv){
    int size,i,j;
	char *file;
	int val;
    int attempt=0, backtracks=0;
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
    
    bactrack(matrix, &attempt, &backtracks, size*size);
	
    for (i = 0; i < size * size; i++){
        for(j = 0; j < size * size; j++) {
            printf("%d ",matrix[i][j]);
        }
        printf("\n");
    }
    
}



void bactrack(int **puzzle, int *attempt, int *backtracks, int size)
{
    
    
    int i, j, k, found=0, temp=0;
    

    
    int **stable = (int **)malloc(size * size * sizeof(int *));
    for(i = 0; i < size * size; i++) stable[i] = (int *)malloc(size * size * sizeof(int));
    
    
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
    //printf("1\n");
    
    for(i=0;i<size;i++)
    {
        //printf("2\n");
        for(j=0;j<size;j++)
        {
            //printf("3\n");
            for(k=0;k<size;k++)
            {
                //printf("4\n");
                if(checkValid(puzzle, i, j, k+1, size)==1)
                {
                    puzzle[i][j]=k+1;
                    break;
                }
                if(k==size-1)
                    break;
            }
            
            while(puzzle[i][j]==0)
            {
                //printf("5\n");
                found=0;
                temp=j-1;
                //printf("temp1=%d\n",temp);
                if(temp!=-1){
                    for(j=temp;j>=0;j--)
                    {
                        //printf("6\n");
                        if(stable[i][j]==0)
                        {
                            found=1;
                            break;
                        }
                    }
                }
                temp=i-1;
                
                if(found==0)
                {
                    //printf("temp2=%d\n",temp);
                    if(temp!=-1){
                        for(i=temp;i>=0;i--)
                        {
                            //printf("7\n");
                            
                            for(j=size-1;j>=0;j--)
                            {
                                //printf("8\n");
                                if(stable[i][j]==0)
                                {
                                    found=1;
                                    break;
                                }
                            }
                            if(found==1)
                                break;
                        }
                    }
                }
                
                found=0;
                temp=puzzle[i][j]+1;
                
                for(k=temp;k<size;k++)
                {
                    //printf("9\n");
                    if(checkValid(puzzle, i, j, k+1,size)==1)
                    {
                        found=1;
                        puzzle[i][j]=k+1;
                        break;
                    }
                }
                if(found==0)
                    puzzle[i][j]=0;
            }
        }
    }
}

int checkValid(int **puzzle,int i,int j,int value,int size){
    int a,b,c;
    a = row_valid(puzzle,i,value,size);
    b = collum_valid(puzzle,j,value,size);
    c = sq_valid(puzzle, i, j,value, size );
    if(a && b && c){
        return 1;
    }
    
    
    return 0;
}
