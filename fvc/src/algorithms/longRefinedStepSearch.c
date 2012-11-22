#include "../../inc/types.h"
#include "../../inc/ME.h"
#include "../../inc/coder.h"

int longRefinedStepSearch(MEPARM *meparm,unsigned int meRange){
	
	int operations[2]={0,0};
	int t;
	//int bestSAD = MAX;
	
	MEPARM * localParm[2];
	
	for (t = 0; t < 5; t++) {
        if ((localParm[t] = (MEPARM*) malloc(sizeof (MEPARM))) == NULL) {
            printf("\nerro ao alocar localParm\n");
            exit(1);
        }
    }
	
	for (t = 0; t < 2; t++) {
        localParm[t]->RF = meparm->RF;
        localParm[t]->targetBlock = meparm->targetBlock;
        localParm[t]->ch = meparm->ch;
        localParm[t]->cw = meparm->cw;
        localParm[t]->vh = 0;
        localParm[t]->vw = 0;
        localParm[t]->SAD = MAX;
        operations[t]=0;

    }
	
	operations[0] = diamondSearch(localParm[0], meRange);
	operations[1] = nStepSearch(localParm[1], 4);
	
	
	if (localParm[0]->SAD <= localParm[1]->SAD) {
		meparm->vh = localParm[0]->vh;
		meparm->vw = localParm[0]->vw;
    }else{
		meparm->vh = localParm[1]->vh;
		meparm->vw = localParm[1]->vw;
	}	
    printf("\n(%d,%d,%d)",localParm[0]->SAD,localParm[1]->SAD,meparm->SAD);
    getchar();
    
    for (t = 0; t < 2; t++)
        free(localParm[t]);
        
    return (operations[0]+operations[1]);
}
