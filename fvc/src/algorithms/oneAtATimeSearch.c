#include "../../inc/types.h"
#include "../../inc/ME.h"
#include "../../inc/coder.h"

int oneAtATimeSearch(MEPARM *meparm) {

    int SAD, bestSAD = MAX;
    int truncFLAG = 0;
    int h = 0, w = 0;
    int operations = 0;
    int iter = 3;
    //int iteration_counter= -1;
	int flag_direction;		//0 esquerda, 1 direita

    gsl_matrix_uchar *candBlock;
    gsl_matrix_uchar *targetBlock = meparm->targetBlock;
    gsl_matrix_uchar_view tempV;

    int center[2] = {0, 0}; // coordenadas (h,w) -> (Y,X)
	int left[2]   = {0,-1};
	int right[2]  = {0, 1};
	int up[2]	  = {1, 0};
	int down[2]	  = {-1, 0};

	extractCandBlock(center[0],center[1]); //extrai centro
	truncFLAG = SADCalc(targetBlock, candBlock, &SAD, bestSAD, meparm->PEL, &operations);
    if (!truncFLAG) {
        meparm->vh = center[0];
        meparm->vw = center[1];
        bestSAD = SAD;
    }
	

	h=center[0]+left[0];
	w=center[1]+left[1];	
	if(isInsideFrame(h,w)){			//extrai esquerda
		extractCandBlock(h,w);
		
        truncFLAG = SADCalc(targetBlock, candBlock, &SAD, bestSAD, meparm->PEL, &operations);

        if (!truncFLAG) {
            meparm->vh = h;
            meparm->vw = w;
            bestSAD = SAD;   
        }
    }		
	
	h=center[0]+right[0];
	w=center[1]+right[1];	
	if(isInsideFrame(h,w)){			//extrai direita
		extractCandBlock(h,w);
		
        truncFLAG = SADCalc(targetBlock, candBlock, &SAD, bestSAD, meparm->PEL, &operations);

        if (!truncFLAG) {
            meparm->vh = h;
            meparm->vw = w;
            bestSAD = SAD;
        }
    }



	if(meparm->vw!=0){	//se o melhor entre os 3 (esquerda,centro,direita) for diferente do centro
		if(meparm->vw==1)			//seta a flag que decide qual lado para ir
			flag_direction = 1;		//direita
		else{
			flag_direction = 0;		//esquerda
		}
		do{
			center[0]=meparm->vh;
			center[1]=meparm->vw;	
			iter++;
			switch (flag_direction){
				case 0:
					h=center[0]+left[0];
					w=center[1]+left[1];

		            if (isInsideFrame(h, w)) {
		                extractCandBlock(h, w);
		                truncFLAG = SADCalc(targetBlock, candBlock, &SAD, bestSAD, meparm->PEL, &operations);

				        if (!truncFLAG) {
				            meparm->vh = h;
				            meparm->vw = w;
				            bestSAD = SAD;
				        }
					}
				break;
				case 1:
					h=center[0]+right[0];
					w=center[1]+right[1];

		            if (isInsideFrame(h, w)) {
		                extractCandBlock(h, w);
		                truncFLAG = SADCalc(targetBlock, candBlock, &SAD, bestSAD, meparm->PEL, &operations);

				        if (!truncFLAG) {
				            meparm->vh = h;
				            meparm->vw = w;
				            bestSAD = SAD;
						}				      
					}

				break;
			}
		}while(meparm->vh != center[0] || meparm->vw != center[1]);		
	}	
	//etapa vertical
	
	iter+=2;
	//high[1]=center[1];
	//low[1]=center[1];
	h=center[0]+up[0];
	w=center[1]+up[1];
	if(isInsideFrame(h,w)){			//extrai acima
		extractCandBlock(h,w);

		truncFLAG = SADCalc(targetBlock, candBlock, &SAD, bestSAD, meparm->PEL, &operations);

		if (!truncFLAG) {
			meparm->vh = h;
			meparm->vw = w;
			bestSAD = SAD;
		}
	}
	
	
	h=center[0]+down[0];
	w=center[1]+down[1];
	if(isInsideFrame(h,w)){			//extrai abaixo
		extractCandBlock(h,w);

		truncFLAG = SADCalc(targetBlock, candBlock, &SAD, bestSAD, meparm->PEL, &operations);

		if (!truncFLAG) {
			meparm->vh = h;
			meparm->vw = w;
			bestSAD = SAD;
		}
	}

	if(meparm->vh!=0){	//se o melhor entre os 3 (acima,centro,abaixo) for diferente do centro
		if(meparm->vh==1)			//seta a flag que decide qual lado para ir
			flag_direction = 1;		//acima
		else{
			flag_direction = 0;		//abaixo
		}
		do{
			center[0]=meparm->vh;
			center[1]=meparm->vw;	
			iter++;
			switch (flag_direction){
				case 0:
					h=center[0]+down[0];
					w=center[1]+down[1];
					 if (isInsideFrame(h, w)) {
						extractCandBlock(h, w);
						truncFLAG = SADCalc(targetBlock, candBlock, &SAD, bestSAD, meparm->PEL, &operations);

						if (!truncFLAG) {
							meparm->vh = h;
							meparm->vw = w;
							bestSAD = SAD;
						}
					}
				break;
				case 1:
					h=center[0]+up[0];
					w=center[1]+up[1];

					if (isInsideFrame(h, w)) {
						extractCandBlock(h, w);
						truncFLAG = SADCalc(targetBlock, candBlock, &SAD, bestSAD, meparm->PEL, &operations);

						if (!truncFLAG) {
							meparm->vh = h;
							meparm->vw = w;
							bestSAD = SAD;
						}
					}

				break;
			}
		}while(meparm->vh != center[0] || meparm->vw != center[1]);		
	}			
    return iter;
}

