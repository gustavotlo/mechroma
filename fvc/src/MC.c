#include "../inc/types.h"
#include "../inc/MC.h"

void MC(gsl_matrix_uchar *RF, gsl_matrix_uchar *RecF, gsl_vector_int *vector) {
    int h, w;
    int i, j;
    int k1, k2;
    int vpos = 0;
    unsigned char value = 0;

    gsl_matrix_uchar_view tempV;

    for (h = 0; h < RF->size1; h += BLOCKSIZE) {
        for (w = 0; w < RF->size2; w += BLOCKSIZE) {

            k1 = gsl_vector_int_get(vector, vpos);
            k2 = gsl_vector_int_get(vector, vpos + 1);
            vpos += 2;

            tempV.matrix = gsl_matrix_uchar_submatrix(RF, h - k1, w + k2 , BLOCKSIZE, BLOCKSIZE).matrix;

            for (i = 0; i < BLOCKSIZE; i++) {
                for (j = 0; j < BLOCKSIZE; j++) {
                    value = gsl_matrix_uchar_get(&tempV.matrix, i, j);
                    gsl_matrix_uchar_set(RecF, i + h, j + w, value);
                }
            }
        }
    }
    return;
}

void MCCROMA(gsl_matrix_uchar *RF, gsl_matrix_uchar *RecF, gsl_vector_int *vector) {
    int h, w;
    int i, j;
    int k1, k2;
    int vpos = 0;
    unsigned char value = 0;

    gsl_matrix_uchar_view tempV;

    for (h = 0; h < RF->size1; h += CHROMABLOCKSIZE) {
        for (w = 0; w < RF->size2; w += CHROMABLOCKSIZE) {

            k1 = gsl_vector_int_get(vector, vpos);
            k2 = gsl_vector_int_get(vector, vpos + 1);
            vpos += 2;

            tempV.matrix = gsl_matrix_uchar_submatrix(RF, h - k1, w + k2 , CHROMABLOCKSIZE, CHROMABLOCKSIZE).matrix;

            for (i = 0; i < CHROMABLOCKSIZE; i++) {
                for (j = 0; j < CHROMABLOCKSIZE; j++) {
                    value = gsl_matrix_uchar_get(&tempV.matrix, i, j);
                    gsl_matrix_uchar_set(RecF, i + h, j + w, value);
                }
            }
        }
    }
    return;
}

