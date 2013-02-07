#include "../../inc/types.h"
#include "../../inc/ME.h"
#include "../../inc/coder.h"

int fullSearch(MEPARM *meparm, unsigned int meRange) {
    int half = meRange / 2;
    int h, w;

    int SAD, bestSAD = MAX;

    int operations = 0;

    int truncFLAG = 0;

    gsl_matrix_uchar *candBlock;
    gsl_matrix_uchar *targetBlock = meparm->targetBlock;
    gsl_matrix_uchar_view tempV;

    for (h = half; h >= -half; h--) {
		for (w = -half; w <= half; w++) {
            if (isInsideFrame(h, w)) {
   		extractCandBlock(h, w);
                truncFLAG = SADCalc(targetBlock, candBlock, &SAD, bestSAD, meparm->PEL, &operations);
                if (!truncFLAG) {
                    meparm->vh = h;
                    meparm->vw = w;
                    bestSAD = SAD;
                }
                operations++;
            }
        }
    }
    return operations;
}

