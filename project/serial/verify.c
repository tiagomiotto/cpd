#include <stdio.h>
#include <stdbool.h>
#include <math.h>

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