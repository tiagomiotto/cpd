#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <omp.h>

//Under construction
int find_position(int tam, int my_row, int my_collum, int indice, int *next_row, int *next_collum){
	
	int aux=sqrt(tam),i,j,aux_row,aux_col,aux_indice;
	aux_indice=indice;


	if(indice<tam-1){ //Check if it's in my row
                
		if(indice>=my_collum) aux_indice++;
		if(!(aux_indice>tam-1)){
			*next_collum=aux_indice;
			*next_row=my_row;
			return 0;}
		}
	
	aux_indice=indice;

	if(indice<(2*tam-2)){ //Check if it's in my collum
                if((indice-(tam-1))>=my_row) aux_indice++;
		if(!(aux_indice>(2*tam-2))){
			*next_collum=my_collum;
			*next_row=aux_indice-tam+1;
//                         printf("ind %d (%d,%d)->(%d,%d)\n",indice,my_row,my_collum,*next_row,*next_collum);
			return 0;
		}
                    
                    
                    
                    
// 		if((indice-(tam-2))>=my_row) aux_indice++;
// 		if(!(aux_indice>(2*tam-2))){
// 			*next_collum=my_collum;
// 			*next_row=aux_indice-tam-1;
// 			return 0;
// 		}
	}

	aux_row=my_row;
	aux_col=my_collum;
	//Checks Square
	aux_row=aux_row-(aux_row) % aux; //sets edges
	j=1;
	for(i=0;i<(indice-(2*tam-3));i++){ //Descobre qual row esta o indice
		if(i==((aux-1)*j)){
			aux_row++;
			j++;
		}
		if(aux_row==my_row){ //Não sei se isso vai funcionar
			aux_row++;
		}
	}
	
	aux_col= aux_col-(aux_col % aux); //sets edge
	aux_col= aux_col + i%(aux-1); //descobre a coluna do indice 
	if(aux_col>= my_collum){ //tambem nao sei se isso vai funcionar
			aux_col++;
	}
	*next_row=aux_row;
	*next_collum=aux_col;


//Não sei se a funcção que pula a linha e a coluna vai funcionar, se não, aumentamos os indices para conter
//o quadrado todo e verificamos o quadrado todo

}
bool is_valid(int** total, int num, int row, int collum,int tam ){
	//for(cria intervalos consoante ao numero de threads)
	//for(usa a funcao find position e verifica se o numero existe) ---Esse é paralelizado
	//se encontrar o numero, da break no for de cima e já retorna, evitando fazer mais calculos
	//como esse é um codigo serial, vai ter só 1 thread, logo o for de cima cria intervalos 1 a 1
	//e o de baixo verifica de 1 a 1
	//Porque precisamos que o algoritmo seja o mesmo
    int i,indice,next_row,next_collum,Nthreads,ite,j,flag,sai,initial,final;
    Nthreads = 8;
    omp_set_num_threads(Nthreads);
//     int test[Nthreads];
    sai=0;
    ite=((2*(tam-1)+tam-(2*sqrt(tam)-1)+Nthreads-1))/Nthreads;
//     for (j=0; j<Nthreads;j++){
//             test[j]=1;
//         }
 #pragma omp parallel
    for (i=0;i<ite;i++){
		#pragma omp single
        {initial=i*Nthreads;
        final=(i+1)*Nthreads;
        if (final>(2*(tam-1)+tam-(2*sqrt(tam)-1))) final=(2*(tam-1)+tam-(2*sqrt(tam)-1));}
//         if (row==3) printf("initial %d final %d\n",initial,final);
        #pragma for private(next_row,next_collum)
        for (indice=initial; indice<final;indice++){
            
            find_position(tam, row, collum, indice, &next_row, &next_collum);
            //printf("Number of threads %d\n", omp_get_num_threads() );
//             test[omp_get_thread_num()]=1;
            if (total[next_row][next_collum]==num) {
                sai=1; //isto pode dar problema?????
            }
//             /*if (row==3 && collum==4)*/ printf("%d ( %d, %d) ind %d (%d , %d) val %d, %d , res %d\n",omp_get_thread_num(),row,collum,indice,next_row,next_collum,total[next_row][next_collum],num,test[omp_get_thread_num()]);
        }
         
//         for (j=0; j<final-initial;j++){
//             if (test[j]==0) {sai=1;  break;}
//             test[j]=1;
//         }
	
        if (sai==1) {break;}
    }
    if (sai==1) {/*if (row==3) printf("sai0\n");*/return 0;}
    else{ /*if (row==3) printf("sai1\n");*/return 1;}
    return 1;
}
