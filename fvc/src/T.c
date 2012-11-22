#include "../inc/types.h"
#include "../inc/loop.h"

/*Faz o calculo da transformada: T = C*Res*CT
 */
short int **transform(short int ** block) {
    short int MC[4][4] = {
        {1, 1, 1, 1},
        {2, 1, -1, -2},
        {1, -1, -1, 1},
        {1, -2, 2, -1}
    };

    short int MCT[4][4] = {
        {1, 2, 1, 1},
        {1, 1, -1, -2},
        {1, -1, -1, 2},
        {1, -2, 1, -1}
    };
    short int **transform;
    short int **temp;
    int i, j, k;

    transform = malloc_matrix(4, 4);
    temp = malloc_matrix(4, 4);

    for (i = 0; i < 4; i++) { //faz a multiplicacao C * block, armazenando em temp
        for (j = 0; j < 4; j++) {
            temp[i][j] = 0;
            for (k = 0; k < 4; k++)
                temp[i][j] += (MC[i][k] * block[k][j]);
        }
    }

    for (i = 0; i < 4; i++)//faz a multiplicacao temp * CT, armazenando em transform
    {
        for (j = 0; j < 4; j++) {
            transform[i][j] = 0;
            for (k = 0; k < 4; k++)
                transform[i][j] += (temp[i][k] * MCT[k][j]);
        }
    }

    free(temp);
    return transform;
}