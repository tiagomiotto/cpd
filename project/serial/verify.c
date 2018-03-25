#include <stdio.h>
#include <stdbool.h>
#include <math.h>



bool row_valid(int** total,int row, int num,int tam);
bool collum_valid(int** total,int collum, int num,int tam);
bool sq_valid(int** total, int row, int collum,int num, int tam );
bool is_valid(int** total, int row, int collum,int num, int tam );
void print(); 

bool row_valid(int** total,int row, int num,int tam){

	int i;
	for (i = 0; i < tam; ++i)
	{
		if(total[row][i]==num) return false;
	}
	return true;
}
bool collum_valid(int** total,int collum, int num, int tam){

	int i;
	for (i = 0; i < tam; ++i)
	{
		if(total[i][collum]==num) return false;
	}
	return true;
}
//https://cboard.cprogramming.com/c-programming/150917-how-check-square-block-condition-sudoku-its-validity.html
bool sq_valid(int** total, int row, int collum,int num, int tam ){
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

	for(i=1;i<aux;i++) 
	{
		if((collum/aux)<i+1){
			aux_col=aux*i;
			break;
		}
	}
	

	//Verifica o subquadrado
	for(i = aux_row; i < aux_row+aux; i++)
       for(j = aux_col; j < aux_col+aux; j++)
            if(total[i][j]==num)    return false;

   return true;

}
bool is_valid(int** total, int num, int row, int collum,int tam ){
	//Paralelizar essas 3
	if((row_valid(total,row,num,tam))==false) return false;  
	if((collum_valid(total,collum,num,tam))==false) return false;  
	if((sq_valid(total,row,collum,num,tam))==false) return false;  
	return true;
}
