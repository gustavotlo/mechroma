#include "../inc/types.h"
#include "../inc/loop.h"

/*Faz o calculo de arredondamento final ao sair da IT*/
short int **post_scale(short int **block) {
    int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            block[i][j] = (block[i][j] + 32) >> 6;
    return block;
}