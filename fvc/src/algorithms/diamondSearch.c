#include "../../inc/types.h"
#include "../../inc/ME.h"
#include "../../inc/coder.h"

float iteracoes = 0; 
float vetores = 0;

int diamondSearch(MEPARM *meparm,unsigned int meRange) {

	//printf("entro DS LUMA\n");
	int half = meRange / 2; //half contem inicio e fim da area de pesquisa ex: area 64 de -32 ate 32
    int SAD, bestSAD = MAX;
    int truncFLAG = 0;
    int pos,postmp=0;
    int h = 0, w = 0;
    int operations = 0;
    int BCC = 13;	//blocos candidatos calculados, 9 diamond e 4 do refinamento
    int iteration_counter= -1;

    gsl_matrix_uchar *candBlock;
    gsl_matrix_uchar *targetBlock = meparm->targetBlock;
    gsl_matrix_uchar_view tempV;

    int center[2] = {meparm->vh, meparm->vw}; // coordenadas (h,w) -> (Y,X)

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
    	//printf("Passou 0 DS\n");
        center[0] = meparm->vh;
        center[1] = meparm->vw;

		for (pos = 0; pos < 9; pos++) {
            h = LDSP[pos][0] + center[0];
            w = LDSP[pos][1] + center[1];
			
            if (isInsideFrame(h, w)) {
				//printf("PASSOU 1 DS\n");                
				extractCandBlock(h, w);
				//printf("PASSOU 2 DS\n");
                truncFLAG = SADCalc(targetBlock, candBlock, &SAD, bestSAD, meparm->PEL, &operations);
				//printf("PASSOU 3 DS\n");
				
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
	iteration_counter++;
    }while ((meparm->vh != center[0] || meparm->vw != center[1]) && (meparm->vh >= -half && meparm->vh <= half && meparm->vw >= -half && meparm->vw <= half));
	
	//printf("Passou 3,5 FS\n");
    for (pos = 0; pos < 5; pos++) {
        h = SDSP[pos][0] + center[0];
        w = SDSP[pos][1] + center[1];
		
		//printf("PASSOU 4 DS\n");
        if (isInsideFrame(h, w)) {
            extractCandBlock(h, w);
            truncFLAG = SADCalc(targetBlock, candBlock, &SAD, bestSAD, meparm->PEL, &operations);
			//printf("PASSOU 5 DS\n");
            if (!truncFLAG) {
                meparm->vh = h;
                meparm->vw = w;
                bestSAD = SAD;
				//printf("Passou 6 DS\n");
            }
        }
    }
    meparm->SAD = bestSAD;
   
	//printf("Passou 7 DS\n");
	iteracoes+=iteration_counter;
	vetores++;

//	FILE *fp;
//	fp=fopen("DS_iter.txt","a");
//	if(!fp){
//		puts("nao foi possivel criar o arquivo\n");
//		}
//	fprintf(fp,"%d\n",iteration_counter);
//	fclose(fp);
	
	//printf("saiu DS LUMA\n");
    return BCC;
}


int diamondSearchCROMA(MEPARM *meparm,unsigned int meRange) {

	//printf("entro DS CROMA\n");
	int half = meRange / 2; //half contem inicio e fim da area de pesquisa ex: area 64 de -32 ate 32
    int SAD, bestSAD = MAX;
    int truncFLAG = 0;
    int pos,postmp=0;
    int h = 0, w = 0;
    int operations = 0;
    int BCC = 13;	//blocos candidatos calculados, 9 diamond e 4 do refinamento
    int iteration_counter= -1;

    gsl_matrix_uchar *candBlock;
    gsl_matrix_uchar *targetBlock = meparm->targetBlock;
    gsl_matrix_uchar_view tempV;

    int center[2] = {meparm->vh, meparm->vw}; // coordenadas (h,w) -> (Y,X)

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
    	//printf("Passou 0 DS\n");
        center[0] = meparm->vh;
        center[1] = meparm->vw;

		for (pos = 0; pos < 9; pos++) {
            h = LDSP[pos][0] + center[0];
            w = LDSP[pos][1] + center[1];
			
            if (isInsideFrameCROMA(h, w)) {
				//printf("PASSOU 1 DS\n");                
				extractCandBlockCROMA(h, w);
				//printf("PASSOU 2 DS\n");
                truncFLAG = SADCalcCROMA(targetBlock, candBlock, &SAD, bestSAD, meparm->PEL, &operations);
				//printf("PASSOU 3 DS\n");
				
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
	iteration_counter++;
    }while ((meparm->vh != center[0] || meparm->vw != center[1]) && (meparm->vh >= -half && meparm->vh <= half && meparm->vw >= -half && meparm->vw <= half));
	
	//printf("Passou 3,5 FS\n");
    for (pos = 0; pos < 5; pos++) {
        h = SDSP[pos][0] + center[0];
        w = SDSP[pos][1] + center[1];
		
		//printf("PASSOU 4 DS\n");
        if (isInsideFrameCROMA(h, w)) {
            extractCandBlockCROMA(h, w);
            truncFLAG = SADCalcCROMA(targetBlock, candBlock, &SAD, bestSAD, meparm->PEL, &operations);
			//printf("PASSOU 5 DS\n");
            if (!truncFLAG) {
                meparm->vh = h;
                meparm->vw = w;
                bestSAD = SAD;
				//printf("Passou 6 DS\n");
            }
        }
    }
    meparm->SAD = bestSAD;
   
	//printf("Passou 7 DS\n");
	iteracoes+=iteration_counter;
	vetores++;

//	FILE *fp;
//	fp=fopen("DS_iter.txt","a");
//	if(!fp){
//		puts("nao foi possivel criar o arquivo\n");
//		}
//	fprintf(fp,"%d\n",iteration_counter);
//	fclose(fp);
	
	//printf("saiu DS CROMA\n");
    return BCC;
}

