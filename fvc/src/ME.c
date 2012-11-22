#include "../inc/types.h"
#include "../inc/ME.h"
#include "../inc/coder.h"
int total_iter=0;

int SADCalc(const gsl_matrix_uchar *targetBlock, gsl_matrix_uchar *candBlock,
        int *targetSAD, const int lastSmallSAD, const int PEL, int *operations) {

    int i, j;
    int auxSAD = 0;
    int oddLine=0;
	
    switch(PEL){
		case 0:
			for (i = 0; i < BLOCKSIZE; i++) {
				for (j = 0; j < BLOCKSIZE; j++) {
					auxSAD += ABS(gsl_matrix_uchar_get(candBlock, i, j) - gsl_matrix_uchar_get(targetBlock, i, j));
				}
				if (auxSAD + THROUGHPUT_CHOICE >= lastSmallSAD ) { //SAD summation truncation
					return 1;
				}
			}
		break;
		case 2:
			for (i=0,j=0; i < BLOCKSIZE; i++) {
				for (j=oddLine; j < BLOCKSIZE; j=j+2) {
					auxSAD += ABS(gsl_matrix_uchar_get(candBlock, i, j) - gsl_matrix_uchar_get(targetBlock, i, j));
				}
				if(oddLine)
					oddLine=0;
				else
					oddLine=1;
			}
			if (auxSAD + THROUGHPUT_CHOICE >= lastSmallSAD ) { //SAD summation truncation
				return 1;
			}
		break;
		case 4:
			for (i=0,j=0; i < BLOCKSIZE; i+=2) {
				for (j=oddLine; j < BLOCKSIZE; j=j+2) {
					auxSAD += ABS(gsl_matrix_uchar_get(candBlock, i, j) - gsl_matrix_uchar_get(targetBlock, i, j));
				}
				if(oddLine)
					oddLine=0;
				else
					oddLine=1;
			}
			if (auxSAD + THROUGHPUT_CHOICE >= lastSmallSAD ) { //SAD summation truncation
				return 1;
			}
		break;
		case 8:
			for (i=0; i < BLOCKSIZE; i=i+2) {
				for (j=0; j < BLOCKSIZE; j=j+4) {
					auxSAD += ABS(gsl_matrix_uchar_get(candBlock, i, j) - gsl_matrix_uchar_get(targetBlock, i, j));
				}
			}
			if (auxSAD + THROUGHPUT_CHOICE >= lastSmallSAD ) { //SAD summation truncation
				return 1;
			}
		break;
		case 16:
			for (i=0; i < BLOCKSIZE; i=i+2) {
				for (j=oddLine; j < BLOCKSIZE; j=j+8) {
					auxSAD += ABS(gsl_matrix_uchar_get(candBlock, i, j) - gsl_matrix_uchar_get(targetBlock, i, j));
				}
				if(oddLine)
					oddLine=0;
				else
					oddLine=4;
			}
			if (auxSAD + THROUGHPUT_CHOICE >= lastSmallSAD ) { //SAD summation truncation
				return 1;
			}
		break;
	}
	

    *(targetSAD) = auxSAD;
    return 0;
}

void meInt(gsl_matrix_uchar *AF, gsl_matrix_uchar *RF, ARG *a, gsl_vector_int * vector) {

	//printf("entrou ME LUMA\n");
    MEPARM *parm;

    if ((parm = malloc(sizeof (MEPARM))) == NULL) {
        printf("erro ao alocar parm em meInt");
        exit(1);
    }



    parm->RF = RF;
    parm->PEL = a->PEL;
    parm->targetBlock = gsl_matrix_uchar_alloc(BLOCKSIZE, BLOCKSIZE); //testar para identificar se sucesso na alocaÃ§Ã£o

    gsl_matrix_uchar_view tempV;
	
    unsigned int i, hmax = a->height; //
    unsigned int j, wmax = a->width; //
    unsigned int h = 0;
	int count_block=0;

	total_iter= LENGHT_LIST_BLOCK * N_iter_block;


    //CHECAR
    for (i = 0; i < hmax; i = i + BLOCKSIZE) { //lacos pra percorrer todos blocos BLOCKSIZExBLOCKSIZE de AF
        //printf("i:%d j:%d",i,j);
        for (j = 0; j < wmax; j = j + BLOCKSIZE) {
            //setar parm adequadamente
            parm->ch = i;
            parm->cw = j;
            parm->vh = 0;
            parm->vw = 0;
			parm->randons = a->sorteios;
			//printf("PASSOU 1 ME\n");
            tempV.matrix = gsl_matrix_uchar_submatrix(AF, i, j, BLOCKSIZE, BLOCKSIZE).matrix; //CHECAR se alterar a view altera a matriz, se sim (provavel), entao fuuuu ( vai ter que ser feito um set=get a mÃ£o [parm.targetBlock deve ser uma cÃ³pia?]) //memory leak aqui?
            //printf("PASSOU 2 ME\n");
            parm->targetBlock = &tempV.matrix;
			//printf("PASSOU 3 ME\n");
			count_block++;				//contador de blocos increm.
            switch (a->meMode) {
                case DS:
                    a->operations += diamondSearch(parm, a->meRange);
                    break;
                case FS:
                    a->operations += fullSearch(parm, a->meRange);
                    break;
                case MPDS:
                    a->operations += multiPointDiamondSearch(parm, a->distance, a->meRange);
                    break;
				case VMPDS:
                    a->operations += vectorMultiPointDiamondSearch(parm, a->distance, a->meRange);
                    break;
                case STCDS:
                    a->operations += sectorThreeStepSearchwithCenterDiamondSearch(parm, a->distance, a->meRange);
                    break;
                case NSS:
                    a->operations += nStepSearch(parm, a->reserved);
                    break;
                case RS:
                    a->operations += uniformRandSearch(parm, a->meRange);
                    break;
                case RS2:
                    a->operations += uniformRandSearchLPDS(parm, a->meRange);
                    break;
                case RS3:
                    a->operations += LPDSuniformRandSearchLPDS(parm, a->meRange);
                    break;
                case RS4:
                    a->operations += LPDSuniformRandSearch_i_LPDS(parm, a->meRange);
                    break;
				case RS5:
                    a->operations += random5(parm, a->meRange);
                    break;
				case RS6:
                    a->operations += random6(parm, a->meRange);
                    break;
				case RS7:
                    a->operations += random7(parm, a->meRange);
                    break;
				case RS8:
                    a->operations += random8(parm, a->meRange);
					break;
                case ARS:
                    a->operations += adaptativeRandSearch(parm, a->meRange);
                    break;
                case LRSF:
					a->operations += longRefinedStepSearch(parm, a->meRange);
					break;
				case OTS:
					a->operations += oneAtATimeSearch(parm);
					break;
				case DMPDS:
					a->operations += dynamicMultiPointDiamondSearch(parm, a->meRange);
					break;					
                case NO_ME:
					a->operations=0;
					parm->vh=0;
					parm->vw=0;
					break;
				case QSDS_DIC:

				case SDS_DIC:
			    	if (count_block == LENGHT_LIST_BLOCK)
					{
						total_iter= LENGHT_LIST_BLOCK * N_iter_block;
						count_block=0;
					}
					a->operations += SDS_dic(parm);
					break;
				case QSDS_FIX:
				case SDS_FIX:
					a->operations += SDS_fix(parm);
					break;
                default:
					printf("\nexiting...\nInvalid algorithm code.");
                    exit(32);

            }
            //printf("PASSOU 4 ME\n");
            //printf("h: %d vh: %d \n", h, parm->vh);
            gsl_vector_int_set(vector, h, parm->vh);
            //printf("PASSOU 6 ME\n");
            gsl_vector_int_set(vector, h + 1, parm->vw);
            //printf("PASSOU 7 ME\n");
            h += 2;
        }
    }

	//printf("saiu ME LUMA\n");
    free(parm);
    return;
}

int SADCalcCROMA(const gsl_matrix_uchar *targetBlock, gsl_matrix_uchar *candBlock,
        int *targetSAD, const int lastSmallSAD, const int PEL, int *operations) {

    int i, j;
    int auxSAD = 0;
    int oddLine=0;
	
    switch(PEL){
		case 0:
			for (i = 0; i < CROMABLOCKSIZE; i++) {
				for (j = 0; j < CROMABLOCKSIZE; j++) {
					auxSAD += ABS(gsl_matrix_uchar_get(candBlock, i, j) - gsl_matrix_uchar_get(targetBlock, i, j));
				}
				if (auxSAD + THROUGHPUT_CHOICE >= lastSmallSAD ) { //SAD summation truncation
					return 1;
				}
			}
		break;
		case 2:
			for (i=0,j=0; i < CROMABLOCKSIZE; i++) {
				for (j=oddLine; j < CROMABLOCKSIZE; j=j+2) {
					auxSAD += ABS(gsl_matrix_uchar_get(candBlock, i, j) - gsl_matrix_uchar_get(targetBlock, i, j));
				}
				if(oddLine)
					oddLine=0;
				else
					oddLine=1;
			}
			if (auxSAD + THROUGHPUT_CHOICE >= lastSmallSAD ) { //SAD summation truncation
				return 1;
			}
		break;
		case 4:
			for (i=0,j=0; i < CROMABLOCKSIZE; i+=2) {
				for (j=oddLine; j < CROMABLOCKSIZE; j=j+2) {
					auxSAD += ABS(gsl_matrix_uchar_get(candBlock, i, j) - gsl_matrix_uchar_get(targetBlock, i, j));
				}
				if(oddLine)
					oddLine=0;
				else
					oddLine=1;
			}
			if (auxSAD + THROUGHPUT_CHOICE >= lastSmallSAD ) { //SAD summation truncation
				return 1;
			}
		break;
		case 8:
			for (i=0; i < CROMABLOCKSIZE; i=i+2) {
				for (j=0; j < CROMABLOCKSIZE; j=j+4) {
					auxSAD += ABS(gsl_matrix_uchar_get(candBlock, i, j) - gsl_matrix_uchar_get(targetBlock, i, j));
				}
			}
			if (auxSAD + THROUGHPUT_CHOICE >= lastSmallSAD ) { //SAD summation truncation
				return 1;
			}
		break;
		case 16:
			for (i=0; i < CROMABLOCKSIZE; i=i+2) {
				for (j=oddLine; j < CROMABLOCKSIZE; j=j+8) {
					auxSAD += ABS(gsl_matrix_uchar_get(candBlock, i, j) - gsl_matrix_uchar_get(targetBlock, i, j));
				}
				if(oddLine)
					oddLine=0;
				else
					oddLine=4;
			}
			if (auxSAD + THROUGHPUT_CHOICE >= lastSmallSAD ) { //SAD summation truncation
				return 1;
			}
		break;
	}
	

    *(targetSAD) = auxSAD;
    return 0;
}

void meCROMA(gsl_matrix_uchar *AF, gsl_matrix_uchar *RF, ARG *a, gsl_vector_int * vector) {

	//printf("entrou ME CROMA\n");
    MEPARM *parm;

    if ((parm = malloc(sizeof (MEPARM))) == NULL) {
        printf("erro ao alocar parm em meInt");
        exit(1);
    }

    parm->RF = RF;
    parm->PEL = a->PEL;
    parm->targetBlock = gsl_matrix_uchar_alloc(CROMABLOCKSIZE, CROMABLOCKSIZE); //testar para identificar se sucesso na alocaÃ§Ã£o

    gsl_matrix_uchar_view tempV;
	
    unsigned int i, hmax = (a->height)/2; //
    unsigned int j, wmax = (a->width)/2; //
    unsigned int h = 0;
	int count_block=0;

	total_iter= LENGHT_LIST_BLOCK * N_iter_block;

	//printf("HMAX: %d, WMAX: %d \n", hmax, wmax);

    //CHECAR
    for (i = 0; i < hmax; i = i + CROMABLOCKSIZE) { //lacos pra percorrer todos blocos BLOCKSIZExBLOCKSIZE de AF
		//printf("i:%d j:%d",i,j);
		//getchar();
        for (j = 0; j < wmax; j = j + CROMABLOCKSIZE) {
            //setar parm adequadamente
            parm->ch = i;
            parm->cw = j;
            parm->vh = 0;
            parm->vw = 0;
			//parm->randons = a->sorteios;

			//printf("PASSOU 1 ME\n");
            tempV.matrix = gsl_matrix_uchar_submatrix(AF, i, j, CROMABLOCKSIZE, CROMABLOCKSIZE).matrix; //CHECAR se alterar a view altera a matriz, se sim (provavel), entao fuuuu ( vai ter que ser feito um set=get a mÃ£o [parm.targetBlock deve ser uma cÃ³pia?]) //memory leak aqui?
			//printf("PASSOU 2 ME\n");
            parm->targetBlock = &tempV.matrix;
			//printf("PASSOU 3 ME\n");

			count_block++;				//contador de blocos increm.
            switch (a->meMode) {
                case DS:
                    a->operations += diamondSearchCROMA(parm, a->meRange);
                    break;
                 case FS:
                    a->operations += fullSearch(parm, a->meRange);
                    break;
                case MPDS:
                    a->operations += multiPointDiamondSearch(parm, a->distance, a->meRange);
                    break;
				case VMPDS:
                    a->operations += vectorMultiPointDiamondSearch(parm, a->distance, a->meRange);
                    break;
                case STCDS:
                    a->operations += sectorThreeStepSearchwithCenterDiamondSearch(parm, a->distance, a->meRange);
                    break;
                case NSS:
                    a->operations += nStepSearch(parm, a->reserved);
                    break;
                case RS:
                    a->operations += uniformRandSearch(parm, a->meRange);
                    break;
                case RS2:
                    a->operations += uniformRandSearchLPDS(parm, a->meRange);
                    break;
                case RS3:
                    a->operations += LPDSuniformRandSearchLPDS(parm, a->meRange);
                    break;
                case RS4:
                    a->operations += LPDSuniformRandSearch_i_LPDS(parm, a->meRange);
                    break;
				case RS5:
                    a->operations += random5(parm, a->meRange);
                    break;
				case RS6:
                    a->operations += random6(parm, a->meRange);
                    break;
				case RS7:
                    a->operations += random7(parm, a->meRange);
                    break;
				case RS8:
                    a->operations += random8(parm, a->meRange);
					break;
                case ARS:
                    a->operations += adaptativeRandSearch(parm, a->meRange);
                    break;
                case LRSF:
					a->operations += longRefinedStepSearch(parm, a->meRange);
					break;
				case OTS:
					a->operations += oneAtATimeSearch(parm);
					break;
				case DMPDS:
					a->operations += dynamicMultiPointDiamondSearch(parm, a->meRange);
					break;					
                case NO_ME:
					a->operations=0;
					parm->vh=0;
					parm->vw=0;
					break;
				case QSDS_DIC:

				case SDS_DIC:
			    	if (count_block == LENGHT_LIST_BLOCK)
					{
						total_iter= LENGHT_LIST_BLOCK * N_iter_block;
						count_block=0;
					}
					a->operations += SDS_dic(parm);
					break;
				case QSDS_FIX:
				case SDS_FIX:
					a->operations += SDS_fix(parm);
					break;
                default:
					printf("\nexiting...\nInvalid algorithm code.");
                    exit(32);

            }
            //printf("PASSOU 4 ME\n");
            //printf("h: %d vh: %d \n", h, parm->vh);
            gsl_vector_int_set(vector, h, parm->vh);
            //printf("PASSOU 6 ME\n");
            gsl_vector_int_set(vector, h + 1, parm->vw);
            //printf("PASSOU 7 ME\n");
            h += 2;
            
        }
    }

    free(parm);
    //printf("SAIU ME CROMA\n");
    return;
}


