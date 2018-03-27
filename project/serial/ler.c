#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <omp.h>
#include "verify.h"


bool checkValid(int **puzzle,int i,int j,int value, int size);

int main(int argc, char **argv){
    int size,i,j;
	char *file;
	int val;
    int **matrix;
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
	
	**matrix = (int **)malloc(size * size * sizeof(int *));
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
    
    bactrack(matrix, &attempt, &backtracks, size);
	
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
    
    
    for(i=0;i<size;i++)
    {
        for(j=0;j<size;j++)
        {
            for(k=0;k<size;k++)
            {
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
                found=0;
                temp=j-1;
                for(j=temp;j>=0;j--)
                {
                    if(stable[i][j]==0)
                    {
                        found=1;
                        break;
                    }
                }
                temp=i-1;
                if(found==0)
                {
                    for(i=temp;i>=0;i--)
                    {
                        for(j=8;j>=0;j--)
                        {
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
                found=0;
                temp=puzzle[i][j]+1;
                for(k=temp;k<size;k++)
                {
                    if(checkValid(puzzle, i, j, k+1)==1)
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

bool checkValid(int **puzzle,int i,int j,int value,int size){
    bool a,b,c;
    a = row_valid(puzzle,i,value,size);
    b = collum_valid(puzzle,j,value,size);)
    c = sq_valid(puzzle, i, j,value, size );
    if(a && b && c){
        return true;
    }
    
    
    return false;
}
