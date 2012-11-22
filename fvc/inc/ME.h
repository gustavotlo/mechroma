#define THROUGHPUT_CHOICE 0

#define ABS(my_val) ((my_val) < 0) ? -(my_val) : (my_val)

#define isInsideFrame(coordH,coordW) 									\
	   (meparm->ch - coordH >= 0)										\
	&& (meparm->cw + coordW >= 0)										\
	&& (meparm->ch - coordH <= (meparm->RF->size1 - BLOCKSIZE))			\
	&& (meparm->cw + coordW <= (meparm->RF->size2 - BLOCKSIZE))	 		\

#define extractCandBlock(coordH,coordW) 								\
	tempV.matrix = gsl_matrix_uchar_submatrix(							\
									meparm->RF, 						\
									meparm->ch - coordH,				\
									meparm->cw + coordW,				\
									BLOCKSIZE,							\
									BLOCKSIZE).matrix;					\
    candBlock = &tempV.matrix;											\

#define isInsideFrameCROMA(coordH,coordW) 									\
	   (meparm->ch - coordH >= 0)										\
	&& (meparm->cw + coordW >= 0)										\
	&& (meparm->ch - coordH <= (meparm->RF->size1 - CROMABLOCKSIZE))			\
	&& (meparm->cw + coordW <= (meparm->RF->size2 - CROMABLOCKSIZE))	 		\

#define extractCandBlockCROMA(coordH,coordW) 								\
	tempV.matrix = gsl_matrix_uchar_submatrix(							\
									meparm->RF, 						\
									meparm->ch - coordH,				\
									meparm->cw + coordW,				\
									CROMABLOCKSIZE,							\
									CROMABLOCKSIZE).matrix;					\
    candBlock = &tempV.matrix;											\
    
//#define compareSet(truncFLAG) if(!truncFLAG) {(meparm->vh=h;meparm->vw=w;bestSAD=SAD;}
	


struct meParm {
    gsl_matrix_uchar *RF;
    gsl_matrix_uchar *targetBlock;
    int ch, cw; //coordenadas do bloco no AF

    int vh, vw; //compostas do vetor
    int PEL; //sad resultante
    int SAD;
	int randons;

};


struct MPDS_struct {

    unsigned int dist;
    int stage;
    int pos;
    int up;
    int down;
    float resultM;
    float resultL;
    float resultH;
    float bestResult;
    int delta;

};

typedef struct MPDS_struct MPDS_control;
typedef struct meParm MEPARM;

//ME.c
int SADCalc(const gsl_matrix_uchar *targetBlock, gsl_matrix_uchar *candBlock,
			int *targetSAD , const int lastSmallSAD, const int PEL, int *operations);
void meInt(gsl_matrix_uchar *AF, gsl_matrix_uchar *RF, ARG *a, gsl_vector_int *vector);

void meCROMA(gsl_matrix_uchar *AF, gsl_matrix_uchar *RF, ARG *a, gsl_vector_int * vector);

//diamondSearch.c
int diamondSearch(MEPARM *meparm,unsigned int meRange);

//fullSearch.c
int fullSearch(MEPARM *parm, unsigned int meRange);

//logarithmPropagationSearch.c
int logarithmPropagationSearch(MEPARM *parm, unsigned int meRange);
int calculaCentro(MEPARM * parm);
int calculaGeral(MEPARM *parm, unsigned int meRange, unsigned int dist_centro);

//macroDiamondSearch.c
int macroDiamondSearch(MEPARM * meparm, unsigned int dist);

//multiPointDiamondSearch.c
int multiPointDiamondSearch(MEPARM * meparm, unsigned int dist,unsigned int meRange);

//multiPointoneAtTimeSearch.c
int multiPointOneAtTimeSearch(MEPARM * meparm, unsigned int dist);

//multiPointoneAtTimeSearch_DS.c
int multiPointOneAtTimeSearch_DS(MEPARM * meparm, unsigned int dist);

//multiPointNStepSearch.c
int multiPointNStepSearch(MEPARM * meparm, unsigned int dist, unsigned int STEPS);

//multiPointNStepSearch_DS.c
int multiPointNStepSearch_DS(MEPARM * meparm, unsigned int dist, unsigned int STEPS);

//nStepSearch.c
int nStepSearch(MEPARM *meparm, unsigned int STEPS);

//SDS.c
int SDS_dic(MEPARM *meparm);

//SDS_fix.c
int SDS_fix(MEPARM *meparm);

int uniformRandSearch(MEPARM *meparm, unsigned int meRange);
int uniformRandSearchLPDS(MEPARM *meparm, unsigned int meRange);
int LPDSuniformRandSearchLPDS(MEPARM *meparm, unsigned int meRange);
int LPDSuniformRandSearch_i_LPDS(MEPARM *meparm, unsigned int meRange);
int adaptativeRandSearch(MEPARM *meparm, unsigned int meRange);

int longRefinedStepSearch(MEPARM *meparm,unsigned int meRange);

int oneAtATimeSearch(MEPARM *meparm);

int dynamicMultiPointDiamondSearch(MEPARM * meparm,unsigned int meRange);

int vectorMultiPointDiamondSearch(MEPARM * meparm, unsigned int dist,unsigned int meRange);

int sectorThreeStepSearchwithCenterDiamondSearch(MEPARM * meparm, unsigned int dist,unsigned int meRange);

int random5(MEPARM *meparm, unsigned int meRange);

int random6(MEPARM *meparm, unsigned int meRange);

int random7(MEPARM *meparm, unsigned int meRange);

int random8(MEPARM *meparm, unsigned int meRange);
