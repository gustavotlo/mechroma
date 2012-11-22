#include "../../inc/types.h"
#include "../../inc/ME.h"
#include "../../inc/coder.h"

extern int DMPDS_d;

int dynamicMultiPointDiamondSearch(MEPARM * meparm,unsigned int meRange){
	
	return multiPointDiamondSearch(meparm,DMPDS_d, meRange);
}

