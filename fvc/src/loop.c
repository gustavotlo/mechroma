#include "../inc/types.h"
#include "../inc/loop.h"

/*Faz malloc para matrizes de tamanho generico*/
short int **malloc_matrix(int w, int h) {
    short int **matrix;
    int i;
    if ((matrix = malloc(h * sizeof (short int*))) == NULL) {
        printf("Nao foi possivel alocar memoria!\n");
        exit(1);
    }
    for (i = 0; i < h; i++) {
        if ((matrix[i] = malloc(w * sizeof (short int))) == NULL) {
            printf("Nao foi possivel alocar memoria!\n");
            exit(1);
        }
    }
    return matrix;
}

/*faz malloc para matries float de tamanho generico - necessario para IT*/

float **malloc_matrix_float(int w, int h) {
    float **matrix;
    int i;
    if ((matrix = malloc(h * sizeof (float*))) == NULL) {
        printf("Nao foi possivel alocar memoria!\n");
        exit(1);
    }
    for (i = 0; i < h; i++) {
        if ((matrix[i] = malloc(w * sizeof (float))) == NULL) {
            printf("Nao foi possivel alocar memoria!\n");
            exit(1);
        }
    }
    return matrix;
}

/*recebe um bloco 4x4, faz as chamadas para cada modulo do loop e retorna
 *o bloco reconstruido*/
short int **loop(short int **block) {
    short int **t_block, **q_block, **iq_block, **it_block, **rec_block;

    t_block = transform(block);
    q_block = quantize(t_block);
    iq_block = inverse_quantize(q_block);
    it_block = inverse_transform(iq_block);
    rec_block = post_scale(it_block);

    return rec_block;
}
