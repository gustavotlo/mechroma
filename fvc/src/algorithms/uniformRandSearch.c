#include "../../inc/types.h"
#include "../../inc/ME.h"
#include "../../inc/coder.h"
#include <time.h>

int uniformRandSearch(MEPARM *meparm, unsigned int meRange) {

    srand(time(0));

    int SAD, bestSAD = MAX;

    int truncFLAG = 0;

    int h = 0, w = 0;

    int try = 0;

    int operations = 0;

    int half = meRange / 2;

    gsl_matrix_uchar *candBlock;
    gsl_matrix_uchar *targetBlock = meparm->targetBlock;
    gsl_matrix_uchar_view tempV;

    for (try = 0; try < meparm->randons;) {


        h = rand() % meRange - half;
        w = rand() % meRange - half;

        if (isInsideFrame(h, w)) {
            extractCandBlock(h, w);
            truncFLAG = SADCalc(targetBlock, candBlock, &SAD, bestSAD, meparm->PEL, &operations);
			try++;
            if (!truncFLAG) {
                meparm->vh = h;
                meparm->vw = w;
                bestSAD = SAD;
            }
        }
    }

    meparm->SAD = bestSAD;
    return meparm->randons;
}

