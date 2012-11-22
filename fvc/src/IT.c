#include "../inc/types.h"
#include "../inc/loop.h"

/* Faz o calculo da transformada inversa: iT = iC*iQ*iCT
 */
short int **inverse_transform(short int **block) {
    float IMCT[4][4] = {

        {1, 1, 1, 0.5},
        {1, 0.5, -1, -1},
        {1, -0.5, -1, 1},
        {1, -1, 1, -0.5}
    };

    float IMC[4][4] = {

        {1, 1, 1, 1},
        {1, 0.5, -0.5, -1},
        {1, -1, -1, 1},
        {0.5, -1, 1, -0.5}
    };

    short int **i_transform;
    float **temp;
    int i, j, k;

    i_transform = malloc_matrix(4, 4);
    temp = malloc_matrix_float(4, 4);


    for (i = 0; i < 4; i++) {//faz a multiplicacao residuo * ICT, armazenando em temp
        for (j = 0; j < 4; j++) {
            temp[i][j] = 0;
            for (k = 0; k < 4; k++) {
                temp[i][j] += (IMCT[i][k] * block[k][j]);
            }
        }
    }

    for (i = 0; i < 4; i++) {//faz a multiplicacao IC * temp, armazenando em i_transform
        for (j = 0; j < 4; j++) {
            i_transform[i][j] = 0;
            for (k = 0; k < 4; k++) {
                i_transform[i][j] += (short int) (temp[i][k] * IMC[k][j]);
            }
        }
    }

    free(temp);
    return i_transform;
}