#include "../inc/types.h"
#include "../inc/loop.h"

/*Componente responsavel por realizar a interface entre o ME core e o Loop.
 *Recebe um quadro e divide esse quadro em blocos 4x4, chama o loop pra cada bloco e
 *concatena os blocos reconstruidos em um frame de saida*/
short int **partition(short int **frame, int h, int w) {
    int i, j, k, l;
    short int **frame_out, **block, **block_out;

    block = malloc_matrix(4, 4);
    frame_out = malloc_matrix(w, h);

    for (i = 0; i < h; i += 4) {
        for (j = 0; j < w; j += 4) {
            for (k = i; k < i + 4; k++) {
                for (l = j; l < j + 4; l++) {
                    block[k - i][l - j] = frame[k][l];//pega um bloco do quadro de entrada
                }
            }
            block_out = loop(block); //aplica loop no bloco
            for (k = i; k < i + 4; k++) {
                for (l = j; l < j + 4; l++) {
                    frame_out[k][l] = block_out[k - i][l - j]; //concatena o bloco reconstruido na saida
                }
            }
        }
    }
    return frame_out;
}
