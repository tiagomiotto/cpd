#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <omp.h>
#include <math.h>
#include <stdbool.h>

int bactrack_serial(int **puzzle, int size);
int row_valid(int** total,int row, int num,int tam);
int collum_valid(int** total,int collum, int num,int tam);
int sq_valid(int** total, int row, int collum,int num, int tam );
int is_valid(int** total, int row, int collum,int num, int tam );
void  printm(int** matrix,int size);

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
	
	printm(matrix,size*size);
    printf("\n");
    
	double start_serial = omp_get_wtime();
    if(bactrack_serial(matrix, size*size)==-1){
        printf("Serial took %f second",omp_get_wtime()-start_serial );
        return -1;

    }
    double end_serial = omp_get_wtime();

    printf("Serial took %f second",end_serial-start_serial );
    printf("\n");
    return 0;
    
    
}



int bactrack_serial(int **puzzle, int size)
{
    
    
    int i, j, k, found=0, temp=0;
    int a, b;
    

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
            
                //Increment values until finds a valid bvalue
                for(k=puzzle[i][j];k<size;k++)
                {
                    //Check for valid values
                    if(is_valid(puzzle,k+1, i, j, size)==1 && stable[i][j]==0)
                    {
                        //Set value on the puzzle
                        puzzle[i][j]=k+1;
                        break;
                    }
                    //Desnecessary
                    if(k==size-1)
                        break;
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
            //}
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