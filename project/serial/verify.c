#include <stdio.h>
#include <stdbool.h>
#include <math.h>

//Under construction
int find_position(int tam, int my_row, int my_collum, int indice, int *next_row, int *next_collum){
	
	int aux=sqrt(tam),i,j,aux_row,aux_col,aux_indice;
	aux_indice=indice;


	if(indice<tam-1){ //Check if it's in my row

		if(indice>=my_collum) aux_indice++;
		if(!(aux_indice>tam-2)){
			*next_collum=aux_indice;
			*next_row=my_row;
			return 0;}
		}
	
	aux_indice=indice;

	if(indice<(2*tam-2)){ //Check if it's in my collum

		if((indice-(tam-2))>=my_row) aux_indice++;
		if(!(aux_indice>(2*tam-2))){
			*next_collum=my_collum;
			*next_row=aux_indice-tam+1;
			return 0;
		}
	}


	//Checks Square
	aux_row=my_row-(my_row) % aux; //sets edges
	j=1;
	for(i=0;i<(indice-(2*tam-1));i++){ //Descobre qual row esta o indice
		if(i>=aux*j){
			aux_row++;
			j++;
		}
		if(i==my_row%aux){ //Não sei se isso vai funcionar
			aux_row++;
		}
	}
	aux_col= my_collum-(my_collum) % aux; //sets edges
	aux_col= aux_col + i%aux; //descobre a coluna do indice 
	if(aux_col>= my_collum%aux){ //tambem nao sei se isso vai funcionar
			aux_col++;
	}

	*next_row=aux_row;
	*next_collum=aux_col;


//Não sei se a funcção que pula a linha e a coluna vai funcionar, se não, aumentamos os indices para conter
//o quadrado todo e verificamos o quadrado todo

}
bool is_valid_paralel(){
	//for(cria intervalos consoante ao numero de threads)
		//for(usa a funcao find position e verifica se o numero existe) ---Esse é paralelizado
		//se encontrar o numero, da break no for de cima e já retorna, evitando fazer mais calculos
	//como esse é um codigo serial, vai ter só 1 thread, logo o for de cima cria intervalos 1 a 1
	//e o de baixo verifica de 1 a 1
	//Porque precisamos que o algoritmo seja o mesmo
}