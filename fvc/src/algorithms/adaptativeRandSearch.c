#include "../../inc/types.h"
#include "../../inc/ME.h"
#include "../../inc/coder.h"
#include <time.h>

int adaptativeRandSearch(MEPARM *meparm, unsigned int meRange) {

    srand(time(0));

    int SAD, bestSAD = MAX;
    int truncFLAG = 0;
    int h = 0, w = 0;
    int try = 0;
    int operations = 0;
    int half = meRange / 2;
    int center[2] = {0, 0};
    int iterMax = 15, i, pos;

    gsl_matrix_uchar *candBlock;
    gsl_matrix_uchar *targetBlock = meparm->targetBlock;
    gsl_matrix_uchar_view tempV;

    int SDSP[5][2] = {
        {0, 0},
        {0, 1},
        {1, 0},
        {0, -1},
        {-1, 0}
    };

    for (i = 0; i < 3; i++) {
        center[0] = meparm->vh;
        center[1] = meparm->vw;

        for (try = 0; try < iterMax; ) {
			
            h = center[0] + rand() % meRange - half;
            w = center[1] + rand() % meRange - half;

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
        meRange = meRange / 3;
        iterMax = iterMax - 4;
    }
	
    for (pos = 0, i = 0; pos < 5; pos++) {
        h = SDSP[pos][0] + center[0];
        w = SDSP[pos][1] + center[1];

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

    meparm->SAD = bestSAD;
    return 15+10+5+operations;
}


