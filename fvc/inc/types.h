#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_matrix_uchar.h>
#include <gsl/gsl_matrix_ushort.h>
#include <gsl/gsl_vector_int.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define BLOCKSIZE 8		//Tamanho do bloco LUMA
#define CHROMABLOCKSIZE 4	//Tamanho bloco para CROMA

#define MAX 999999999
#define MIN -999999999
//#define ITER 15
#define N_iter_block 20
#define LENGHT_LIST_BLOCK 16

#define NO_ME 		0		//desliga a ME
#define FS 		1		//Full search
#define DS 		2		//Diamond search
#define MPDS 		3		
#define HEX 		4
#define OTS 		5
#define NSS		6
#define RS 		7
#define RS2 		8
#define RS3 		9
#define RS4	  	10
#define ARS 		11
#define LRSF 		12
#define DMPDS		13
#define	SDS_DIC		14
#define	QSDS_DIC	15
#define SDS_FIX		16
#define QSDS_FIX	17
#define VMPDS		18
#define STCDS		19
#define RS5		20
#define RS6		21
#define RS7		22
#define RS8		23
#define DSCROMA		24		//Diamond search CROMA
 
typedef struct arg{
	FILE *fp;
	unsigned int width;
	unsigned int height;
	unsigned int meRange;
	unsigned int frames;
	int chrSampling;
	int meMode;
    double PSNR_Total;
    double PSNRcb_Total;
    double PSNRcr_Total;
	double SAD_Total;
	double SADcb_Total;
	double SADcr_Total;
	double MSE_Total;
	double MSEcb_Total;
	double MSEcr_Total;
	int PEL;
	int distance;
    int reserved;
    double operations;
	char FileName[64];
	int sorteios;
}ARG;
	
