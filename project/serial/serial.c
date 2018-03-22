#include <stdio.h>



bool row_valid(int** total,int row, int num);
bool collum_valid();
bool sq_valid();
bool is_valid();
void print(); 

bool row_valid(int** total,int row, int num){

	int i;
	for (i = 0; i < 9; ++i)
	{
		if(total[row][i]==num) return false;
	}
	return true;
}
bool collum_valid(int** total,int collum, int num){

	int i;
	for (i = 0; i < 9; ++i)
	{
		if(total[i][collum]==num) return false;
	}
	return true;
}
bool sq_valid(int** total, int num, int row, int collum){
	int i,j;
	for(i = row; i < row+3; i++)
        for(j = collum; j < collum+3; j++)
            if(total[i][j]==num)    return false;
    return true;

}

/*
int isAvailable(int puzzle[][9], int row, int col, int num)
{
    int rowStart = (row/3) * 3;
    int colStart = (col/3) * 3;
    int i, j;

    for(i=0; i<9; ++i)
    {
        if (puzzle[row][i] == num) return 0;
        if (puzzle[i][col] == num) return 0;
        if (puzzle[rowStart + (i%3)][colStart + (i/3)] == num) return 0;
    }
    return 1;
}*/