#include <stdio.h>
#include <stdbool.h>
#include <math.h>




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
bool is_valid(int** total, int num, int row, int collum,int tam ){
	//Paralelizar essas 3
	if((row_valid(total,row,num,tam))==false) return 0;  
	if((collum_valid(total,collum,num,tam))==false) return 0;
	if((sq_valid(total,row,collum,num,tam))==false) return 0;
	return 1;
}


//Under construction
int find_position(int tam, int my_row, int my_collum, int indice, int *next_row, int *next_collum){
	
	int aux=sqrt(tam),i,j,aux_row,aux_col,aux_indice;
	aux_indice=indice;


	if(indice<tam-1){ //Check if it's in my row

		if(indice>=my_collum) aux_indice++;
		if(!(aux_indice>tam-2));
			*next_collum=aux_indice;
			*next_row=my_row;
			return 0;
		}
	}
	aux_indice=indice;

	if(indice<(2*tam-2){ //Check if it's in my collum

		if((indice-(total-2))>=my_row) aux_indice++;
		if(!(aux_indice>(2*tam-2))){
			*next_collum=my_collum;
			*next_row=aux_indice-tam+1;
			return 0;
		}
	}
	aux_indice;
	/*Check if it's in the square*/

	for(i=0;i<aux;i++) //Queria fazer isso sem for
	{
		if((my_row/aux)<i+1){
			aux_row=aux*i;
			break;
		}
	}

	for(i=0;i<aux;i++)
	{
		if((my_collum/aux)<i+1){
			aux_col=aux*i;
			break;
		}
	}
	/*O numero de checks tem que ser igual a (n-1)*(n-1) onde n*n é a área do subquadrado*/
	if(indice<(aux-1)*(aux-1)){

	}
	else {
			printf("Indice fora do intervalo\n");
			return -1;
		}


}
bool is_valid_paralel(){
	//for(cria intervalos consoante ao numero de threads)
		//for(usa a funcao find position e verifica se o numero existe) ---Esse é paralelizado
		//se encontrar o numero, da break no for de cima e já retorna, evitando fazer mais calculos
	//como esse é um codigo serial, vai ter só 1 thread, logo o for de cima cria intervalos 1 a 1
	//e o de baixo verifica de 1 a 1
	//Porque precisamos que o algoritmo seja o mesmo
}