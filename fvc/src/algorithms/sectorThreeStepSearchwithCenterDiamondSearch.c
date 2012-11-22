#include "../../inc/types.h"
#include "../../inc/ME.h"
#include "../../inc/coder.h"


int sectorThreeStepSearchwithCenterDiamondSearch(MEPARM * meparm, unsigned int dist,unsigned int meRange) {
	int half = meRange / 2;
    int t;
    int operations [5];
    int bestSAD = MAX;
    int sector = 524;

    MEPARM * localParm[5];
    for (t = 0; t < 5; t++) {
        if ((localParm[t] = (MEPARM*) malloc(sizeof (MEPARM))) == NULL) {
            printf("\nerro ao alocar localParm\n");
            exit(1);
        }
    }

    for (t = 0; t < 5; t++) {	//inicializa todas as meparm ( 5 instancias )
        localParm[t]->RF = meparm->RF;
        localParm[t]->targetBlock = meparm->targetBlock;
        localParm[t]->ch = meparm->ch;
        localParm[t]->cw = meparm->cw;
        localParm[t]->vh = 0;
        localParm[t]->vw = 0;
        localParm[t]->SAD = MAX;
        localParm[t]->PEL = meparm->PEL;
        operations[t]=0;
    }

    //setar cada localParm;
    localParm[1]->vh += dist;
	localParm[1]->vw += dist;

	localParm[2]->vh -= dist;
	localParm[2]->vw += dist;

	localParm[3]->vh -= dist;
	localParm[3]->vw -= dist;

	localParm[4]->vh += dist;
	localParm[4]->vw -= dist;

	operations[0] = diamondSearch(localParm[0], meRange);//chamada do DS no centro
    
	for (t = 1; t < 5; t++){			//executa o 3ss nos quatro quadrantes
        if (
        ((localParm[t]->ch - localParm[t]->vh) >= 0) 					&&
        ((localParm[t]->ch - localParm[t]->vh) <= meparm->RF->size1- BLOCKSIZE)	&&
        ((localParm[t]->cw + localParm[t]->vw) >= 0) 					&&
        ((localParm[t]->cw + localParm[t]->vw) <= meparm->RF->size2 - BLOCKSIZE) && (localParm[t]->vh <= half) && (localParm[t]->vw <= half)
        ){
			operations[t] = nStepSearch(localParm[t],3);//chamads do 3SS
		}
        
	}	
    for (t = 0; t < 5; t++) {	// busca pelo  melhor resultado de sad
		if (localParm[t]->SAD < bestSAD) {
            meparm->vh = localParm[t]->vh;
            meparm->vw = localParm[t]->vw;
            bestSAD = localParm[t]->SAD;
			meparm->SAD = localParm[t]->SAD;
            sector = t;
        }
    }
	
	//getchar();
    for (t = 0; t < 5; t++)	//libera cada meparm
        free(localParm[t]);
   
    return (operations[0]+operations[1]+operations[2]+operations[3]+operations[4]);
}
