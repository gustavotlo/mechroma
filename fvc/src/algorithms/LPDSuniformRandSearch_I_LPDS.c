#include "../../inc/types.h"
#include "../../inc/ME.h"
#include "../../inc/coder.h"
#include <time.h>

/*
* RS4 = small diamond no centro, sorteios em toda area de pesquisa, 
* refinamento final no melhor ponto com small diamond 
*/

int LPDSuniformRandSearch_i_LPDS(MEPARM *meparm, unsigned int meRange) {

    srand(time(0));

    int SAD, bestSAD = MAX;

    int truncFLAG = 0, pos;

    int h = 0, w = 0;

    int try = 0;

    int lIter = 0;

    int operations = 0;

    int half = meRange / 2;

    gsl_matrix_uchar *candBlock;
    gsl_matrix_uchar *targetBlock = meparm->targetBlock;
    gsl_matrix_uchar_view tempV;

    int center[2] = {0, 0}; // coordenadas (h,w) -> (Y,X)

    int SDSP[5][2] = {
        {0, 0},
        {0, 1},
        {1, 0},
        {0, -1},
        {-1, 0}
    };

/*
* refinamento central com small diamond
*/
    for (pos = 0; pos < 5; pos++) {
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
    center[0] = meparm->vh;
    center[1] = meparm->vw;
/*
*sorteios na area de pesquisa
*/

    for (try = 0; try < meparm->randons; ) {


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

/*
* refinamento final no melhor ponto achado
*/
    do {
        center[0] = meparm->vh;
        center[1] = meparm->vw;

        lIter++;

        for (pos = 0; pos < 5; pos++) {
            h = SDSP[pos][0] + center[0];
            w = SDSP[pos][1] + center[1];

            if (isInsideFrame(h, w)) {
                extractCandBlock(h, w);
                truncFLAG = SADCalc(targetBlock, candBlock, &SAD, bestSAD, meparm->PEL, &operations);
				
                if (!truncFLAG) {
                    meparm->vh = h;
                    meparm->vw = w;
                    bestSAD = SAD;
                }
            }
        }
    } while ((meparm->vh != center[0] || meparm->vw != center[1])&& (meparm->vh >= -half && meparm->vh <= half && meparm->vw >= -half && meparm->vw <= half));

    meparm->SAD = bestSAD;
    return meparm->randons+operations+(3*lIter);
}

