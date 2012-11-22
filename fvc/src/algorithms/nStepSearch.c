#include "../../inc/types.h"
#include "../../inc/ME.h"
#include "../../inc/coder.h"

int nStepSearch(MEPARM *meparm, unsigned int STEPS) {

    int S;
    int h, w, i;
    int SAD, bestSAD = MAX;
    int truncFLAG = 0;
    int operations = 0;
    int pos;

    int center[2] = {0, 0};

    S = pow(STEPS - 1, 2);

    int NSSP[9][2] = {// coordenadas (h,w) -> (Y,X)
        {0, 0},
        {S, -S},
        {S, 0},
        {S, S},
        {0, -S},
        {0, S},
        {-S, -S},
        {-S, 0},
        {-S, S}
    };

    gsl_matrix_uchar *candBlock;
    gsl_matrix_uchar *targetBlock = meparm->targetBlock;
    gsl_matrix_uchar_view tempV;



    while (S >= 1) {
        center[0] = meparm->vh;
        center[1] = meparm->vw;

        for (pos = 0, i = 0; pos < 9; pos++) {
            h = NSSP[pos][0] + center[0];
            w = NSSP[pos][1] + center[1];
            if (isInsideFrame(h, w)) {
                extractCandBlock(h, w);

                truncFLAG = SADCalc(targetBlock, candBlock, &SAD, bestSAD, meparm->PEL, &operations);
                operations++;

                if (!truncFLAG) {
                    meparm->vh = h;
                    meparm->vw = w;
                    bestSAD = SAD;
                }
            }
        }
        S = S / 2;
        for (i = 0; i < 9; i++) {
            if (NSSP[i][0] != 0)
                NSSP[i][0] /= 2;
            if (NSSP[i][1] != 0)
                NSSP[i][1] /= 2;
        }
    }
	meparm->SAD = bestSAD;
    return operations;
}
