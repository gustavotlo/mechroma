#include "../../inc/types.h"
#include "../../inc/ME.h"
#include "../../inc/coder.h"


int SDS_fix(MEPARM *meparm) {

    int SAD, bestSAD = MAX;
    int truncFLAG = 0;
    int pos,postmp=0;
    int h = 0, w = 0;
    int operations = 0;
    int BCC = 13;	//blocos candidatos calculados, 9 diamond e 4 do refinamento
	int count_iter=0;
	int flag_first_passing=0;
	
    gsl_matrix_uchar *candBlock;
    gsl_matrix_uchar *targetBlock = meparm->targetBlock;
    gsl_matrix_uchar_view tempV;

    int center[2] = {0, 0}; // coordenadas (h,w) -> (Y,X)

    int LDSP[9][2] = {// coordenadas (h,w) -> (Y,X)
        { 0, 0}, //0
        { 0, 2}, //1
        { 0, -2}, //2
        { 2, 0}, //3
        {-2, 0}, //4
        { 1, 1}, //5
        {-1, -1}, //6
        { 1, -1}, //7
        {-1, 1} //8
    };

    int SDSP[5][2] = {
        { 0, 0},
        { 0, 1},
        { 1, 0},
        { 0, -1},
        {-1, 0}
    };
 
    do {
        center[0] = meparm->vh;
        center[1] = meparm->vw;
		

		if (flag_first_passing)
		{

			count_iter++;				

		}
		else
		{	
			flag_first_passing=1;
		
		}	
		

        for (pos = 0; pos < 9; pos++) {
            h = LDSP[pos][0] + center[0];
            w = LDSP[pos][1] + center[1];

            if (isInsideFrame(h, w)) {
                extractCandBlock(h, w);
                truncFLAG = SADCalc(targetBlock, candBlock, &SAD, bestSAD, meparm->PEL, &operations);

                if (!truncFLAG) {
                    meparm->vh = h;
                    meparm->vw = w;
                    bestSAD = SAD;
					postmp=pos;
                }
            }
        }
		if (meparm->vh != center[0] || meparm->vw != center[1]){
			if (postmp == 1 || postmp == 2 || postmp == 3 || postmp == 4)
		        BCC += 5;
		    else
		        if (postmp == 5 || postmp == 6 || postmp == 7 || postmp == 8)
		        BCC += 3;
		}
    }while ((meparm->vh != center[0] || meparm->vw != center[1]) && count_iter < N_iter_block ) ;

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
    meparm->SAD = bestSAD;
	
	
    return BCC;
}
