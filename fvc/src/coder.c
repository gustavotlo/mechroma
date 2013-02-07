#include "../inc/types.h"
#include "../inc/coder.h"
#include "../inc/ME.h"
#include "../inc/MC.h"
#include <math.h>

int DMPDS_d=10;
extern float q0,q1,q2,q3,q4;//referente ao multiPointDiamondSearch.c para saber os quadrantes 
extern float qv0,qv1,qv2,qv3,qv4;//referente ao vectorMultiPointDiamondSearch.c
extern float iteracoes;//referente ao diamondSearch.c para saber a quantidade de iteracoes
extern float vetores;//referente ao diamondSearch.c

double PSNR_calc(ARG *a, const gsl_matrix_uchar *RF, const gsl_matrix_uchar *RecF) {

    int h, w;

    double aux;
    double SAD_FRAME = 0;
    double MSE_FRAME = 0;
    double PSNR_FRAME = 0;

    for (h = 0; h < RF->size1; h++) {
        for (w = 0; w < RF->size2; w++) {
            aux = (double) (gsl_matrix_uchar_get(RF, h, w) - gsl_matrix_uchar_get(RecF, h, w));
            SAD_FRAME += ABS(aux);
            MSE_FRAME += aux * aux;
        }
    }
    MSE_FRAME = MSE_FRAME / (RF->size1 * RF->size2);
    PSNR_FRAME = 10 * log10(65025 / MSE_FRAME);

    a->PSNR_Total += PSNR_FRAME;
    a->MSE_Total += MSE_FRAME;
    a->SAD_Total += SAD_FRAME;

    return PSNR_FRAME;
}

double PSNRcb_calc(ARG *a, const gsl_matrix_uchar *RF, const gsl_matrix_uchar *RecF) {

    int h, w;

    double aux;
    double SAD_FRAME = 0;
    double MSE_FRAME = 0;
    double PSNR_FRAME = 0;

    for (h = 0; h < RF->size1; h++) {
        for (w = 0; w < RF->size2; w++) {
            aux = (double) (gsl_matrix_uchar_get(RF, h, w) - gsl_matrix_uchar_get(RecF, h, w));
            SAD_FRAME += ABS(aux);
            MSE_FRAME += aux * aux;
        }
    }
    MSE_FRAME = MSE_FRAME / (RF->size1 * RF->size2);
    PSNR_FRAME = 10 * log10(65025 / MSE_FRAME);

    a->PSNRcb_Total += PSNR_FRAME;
    a->MSEcb_Total += MSE_FRAME;
    a->SADcb_Total += SAD_FRAME;

    return PSNR_FRAME;
}

double PSNRcr_calc(ARG *a, const gsl_matrix_uchar *RF, const gsl_matrix_uchar *RecF) {

    int h, w;

    double aux;
    double SAD_FRAME = 0;
    double MSE_FRAME = 0;
    double PSNR_FRAME = 0;

    for (h = 0; h < RF->size1; h++) {
        for (w = 0; w < RF->size2; w++) {
            aux = (double) (gsl_matrix_uchar_get(RF, h, w) - gsl_matrix_uchar_get(RecF, h, w));
            SAD_FRAME += ABS(aux);
            MSE_FRAME += aux * aux;
        }
    }
    MSE_FRAME = MSE_FRAME / (RF->size1 * RF->size2);
    PSNR_FRAME = 10 * log10(65025 / MSE_FRAME);

    a->PSNRcr_Total += PSNR_FRAME;
    a->MSEcr_Total += MSE_FRAME;
    a->SADcr_Total += SAD_FRAME;

    return PSNR_FRAME;
}

int maximum(double a, double b, double c){ //funcao para encontrar o indice do maior elemento entre os 3
	
	int pos=0;
	double max=a;
	
	if(b>max){
		max=b;
		pos=1;
	}
	if(c>max){
		pos=2;
	}
	return pos;
	
}

void jumpChroma(ARG *a) {
    /*static int ratio; //make it static?
    switch (a->chrSampling) {
        case 0://420
            ratio = (a->width * a->height) / 2;
            break;
        case 2://422
            ratio = (a->width * a->height);
            break;
        case 4://444
            ratio = (a->width * a->height)*2;
            break;
        default:
            printf("\nchrSampling error")
            exit(1);
    }*/
    fseek(a->fp, (((a->width * a->height)/2)), SEEK_CUR); //char é sempre 1 byte...
    return;
}

gsl_vector_int *  scaleVector(gsl_vector_int *vector, ARG *a){
    gsl_vector_int * novoVetor;
    novoVetor = gsl_vector_int_calloc(((((a->height/2) * (a->width/2)) / (CHROMABLOCKSIZE*CHROMABLOCKSIZE))*2)); //Aloca novo Vetor
    
    //printf("entrou scaleVector\n");
    //getchar();
    int i=0;
    int h,w;
    int n = ((((a->height/2) * (a->width/2)) / (CHROMABLOCKSIZE*CHROMABLOCKSIZE))*2);

    while(i < n){//Para cada elemento do vetor escalona
        //printf("\ni: %d, n: %d\n",i,n);
        h = gsl_vector_int_get(vector, i);              //Pega a componente horizontal
        w = gsl_vector_int_get(vector, i + 1);          //Pega a componente vertical
        
        //printf("h: %d, w: %d\n",h,w);
        if(h != 0){                                     //Se o elemento for diferente de zero
            h = h/2;
            //printf("novo H: %d\n", h);
            //getchar();
            gsl_vector_int_set(novoVetor, i, h);        //Escalona o elemento e coloca no novovetor
            
        }else{   
            gsl_vector_int_set(novoVetor, i, 0);
            //printf("Coloca 0 no vetor\n");                //Senao coloca zero no novoVetor
        }
        
        if(w != 0){                                     //Se o elemento for diferente de zero
            w = w/2;
            //printf("novo W: %d\n", w);
            //getchar();
            gsl_vector_int_set(novoVetor, i+1, w);      //Escalona o elemento e coloca no novovetor
        }else{
           gsl_vector_int_set(novoVetor, i+1, 0);       //Senao coloca zero no novoVetor
           //printf("Coloca 0 no vetor\n");
        }       
        //getchar();
        i+=2;
    }
    
    return novoVetor;
}

int code(ARG *a) {


    short temp = 0;
    double PSNR = 0;
   
    //define as matrizes e o vetor
    gsl_matrix_uchar *AF, *RF, *RecF; // ponteiro para FRAME atual, de referência e reconstruido da LUMA.
    gsl_matrix_uchar *cbAF, *crAF, *cbRF, *crRF, *cbRecF, *crRecF; // ponteiro para FRAME atual, de referência e reconstruido da CROMA.
    gsl_matrix_ushort *ResF, *cbResF, *crResF;  // ponteiro para FRAME residuo da LUMA e CROMA.
    gsl_vector_int *vvector, *cbvvector, *crvvector; //ponteiros dos vetores de movimento.

    init_a();	// marcro inicializa variaveis estatisticas (PSNR médio, erro total...) da LUMA E CROMA.

    unsigned int framecounter = 0, i, j;
    FILE *RecYUV, *ResYUV;

    //abre arquivos
    RecYUV = fopen("yuv/rebuild.yuv", "wb"); //testar retorno
    ResYUV = fopen("yuv/residuo.yuv", "wb"); //testar retorno
	
    //variaveis do DMPDS
    double DMPDS_STAGE0_PSNR=0;	//psnr do estágio 0
    double DMPDS_STAGE1_PSNR=0;	//psnr do estágio 1
    double DMPDS_STAGE2_PSNR=0;	//psnr do estágio 2
    int DMPDS_STAGE=0;          //estágio do controle
    int DMPDS_delta=5;		//variação aplicada ao valor d (DMPDS_d)
    int DMPDS_up=0;		//quantas vezes o estágio 2 foi melhor
    int DMPDS_down=0;		//quantas vezes o estágio 1 foi melhor
    int index=0;

    int totalVetores;//usado para saber a quantidade total de vetores do mpds


    if(RecYUV == NULL){//abre o arquivo da pasta yuv, contendo os quadros reconstruidos
        printf("\nerro ao abrir arquivo com frame reconstruído\n");
        exit(1);
    }
    
    if(ResYUV == NULL){//abre o arquivo da pasta yuv, contendo os quadros de resíduo
        printf("\nerro ao abrir arquivo com frame residual\n");
        exit(1);
    }
    
    //aloca na memoria as matrizes de CB
    cbAF = gsl_matrix_uchar_alloc(a->height/2, a->width/2); //chrAF é o frame atual de CROMA 
    cbRF = gsl_matrix_uchar_alloc(a->height/2, a->width/2); // chrRF é o frame de referencia de CROMA
    cbRecF = gsl_matrix_uchar_alloc(a->height/2, a->width/2);// chrRecF é o frame reconstruido de CROMA após passar pela MC.
    cbResF = gsl_matrix_ushort_alloc(a->height/2, a->width/2); //residuo tem 9bits, logo nao cabe em um unsigned char
    cbvvector = gsl_vector_int_calloc(((((a->height/2) * (a->width/2)) / (CHROMABLOCKSIZE*CHROMABLOCKSIZE))*2));//vvector é o vetor de vetores, 2 em duas coordenadas [0]H [1]W , [2]H [3]W, ....

    //aloca na memoria as matrizes de CR
    crAF = gsl_matrix_uchar_alloc(a->height/2, a->width/2); //chrAF é o frame atual CR 
    crRF = gsl_matrix_uchar_alloc(a->height/2, a->width/2); // chrRF é o frame de referencia de CROMA
    crRecF = gsl_matrix_uchar_alloc(a->height/2, a->width/2);// chrRecF é o frame reconstruido de CROMA após passar pela MC.
    crResF = gsl_matrix_ushort_alloc(a->height/2, a->width/2); //residuo tem 9bits, logo nao cabe em um unsigned char
    crvvector = gsl_vector_int_calloc(((((a->height/2) * (a->width/2)) / (CHROMABLOCKSIZE*CHROMABLOCKSIZE))*2));//vvector é o vetor de vetores, 2 em duas coordenadas [0]H [1]W , [2]H [3]W, ....

    //aloca na memoria as matrizes de LUMA
    AF = gsl_matrix_uchar_alloc(a->height, a->width); //pode retornar GSL_ENOMEM ou 0, AF é o frame atual (Actual Frame) 
    RF = gsl_matrix_uchar_alloc(a->height, a->width); // RF é o frame de referencia (Reference Frame)
    RecF = gsl_matrix_uchar_alloc(a->height, a->width);// RecF é o frame reconstruido após passar pela MC.
    ResF = gsl_matrix_ushort_alloc(a->height, a->width); //residuo tem 9bits, logo nao cabe em um unsigned char
    vvector = gsl_vector_int_calloc(((a->height * a->width) / (BLOCKSIZE*BLOCKSIZE))*2);//vvector é o vetor de vetores, 2 em duas coordenadas [0]H [1]W , [2]H [3]W, ....

   
    gsl_matrix_uchar_fread(a->fp, RF); // lê o primeiro quadro de referencia, pode retornar GSL_EFAILED ou 0
    //jumpChroma(a); //pula chroma
    gsl_matrix_uchar_fread(a->fp, cbRF);
    gsl_matrix_uchar_fread(a->fp, crRF);
	
    //intracode(RecYUV)
    //gsl_matrix_uchar_fwrite(RecYUV, RF);

    do {

        gsl_matrix_uchar_fread(a->fp, AF); //lê o quadro atual
        //jumpChroma(a); //pula chroma
        gsl_matrix_uchar_fread(a->fp, cbAF);
        gsl_matrix_uchar_fread(a->fp, crAF);

        meInt(AF, RF, a, vvector); //a ME preenche o vetor de vetores por referencia, esse vetor de vetores é mandado para MC 
        meCROMA(cbAF, cbRF, a, cbvvector);
        meCROMA(crAF, crRF, a, crvvector);
        
        //cbvvector = scaleVector(vvector, a); // Escalona o vetor da luma
        //gsl_vector_int_memcpy(crvvector, cbvvector); //Copia o vetor cb escalonado para cr   
        
        MC(RF, RecF, vvector); //monta um novo quadro usando os vetores e o quadro de referencia
        MCCROMA(cbRF, cbRecF, cbvvector);
        MCCROMA(crRF, crRecF, crvvector);

        gsl_matrix_uchar_fwrite(RecYUV, RecF);	//escreve quadro reconstruído no arquivo rebuild.yuv
        gsl_matrix_uchar_fwrite(RecYUV, cbRecF);
        gsl_matrix_uchar_fwrite(RecYUV, crRecF);

        //calcula o residuo ponto a ponto entre o quadro reconstruido e o quadro atual
        //TODO: transformar isso em uma função
        for (i = 0; i < a->height; i++) {
            for (j = 0; j < a->width; j++) {
                temp = ((gsl_matrix_uchar_get(AF, i, j) - gsl_matrix_uchar_get(RecF, i, j))/2)+126;
                gsl_matrix_ushort_set(ResF, i, j, temp);
            }
        }

        //gsl_matrix_ushort_fwrite(ResYUV, ResF);	//escreve o quadro residual no arquivo de residuo

        //Imprime os resultados de cada frame
        printf("\nFrame %d done!\n", framecounter);

        PSNR = PSNR_calc(a, AF, RecF);
        printf("\t PSNR Lumi: \t%f\n", PSNR);

        PSNR = PSNRcb_calc(a, cbAF, cbRecF);
        printf("\t PSNR CB: \t%f\n", PSNR);

        PSNR = PSNRcr_calc(a, crAF, crRecF);
        printf("\t PSNR CR: \t%f\n", PSNR);

        //DMPDS_Control();

        gsl_matrix_uchar_memcpy(RF, AF); //frame atual passa a ser referencia
        gsl_matrix_uchar_memcpy(cbRF, cbAF);
        gsl_matrix_uchar_memcpy(crRF, crAF);
        //gsl_matrix_uchar_memcpy(RF, RecF+residuo_pos_loop); //frame reconstruido+residuo_pos_loop passa a ser referencia

        framecounter++;
    } while (framecounter < a->frames); //TODO: implementar condição || chegar em fim de arquivo


    printf("\nPSNR Luma medio: \t%.4f", a->PSNR_Total / framecounter);
    printf("\nPSNR Cb medio: \t\t%.4f", a->PSNRcb_Total / framecounter);
    printf("\nPSNR Cr medio: \t\t%.4f", a->PSNRcr_Total / framecounter);
    printf("\n---------------------------------");
    printf("\nErro acumulado: %.0f", a->SAD_Total);
    printf("\nMSE medio: \t%.4f", a->MSE_Total / framecounter);
    printf("\ncomparacoes: \t%.0f\n", a->operations);

    FILE *f1;
	
    f1=fopen("saida.csv","a");
    
    fprintf(f1,"%s;%.4f;%.0f;%.4f;%.0f\n",a->FileName,a->PSNR_Total / framecounter,a->SAD_Total,a->MSE_Total / framecounter,a->operations);
	


    switch(a->meMode)
    {
        case VMPDS:
            totalVetores = qv1+qv2+qv3+qv4+q0;
            printf("Quadrante 1:%.4f\nQuadrante 2:%.4f\nQuadrante 3:%.4f\nQuadrante 4:%.4f\nCentro     :%.4f\nTOTAL Vetores:%d\n",(qv1*100)/totalVetores,(qv2*100)/totalVetores,(qv3*100)/totalVetores,(qv4*100)/totalVetores,(qv0*100)/totalVetores,totalVetores);
            //fprintf(f1,"%d;%.4f;%.4f;%.4f;%.4f;%.4f;%d\n",a->distance,(qv1*100)/totalVetores,(qv2*100)/totalVetores,(qv3*100)/totalVetores,(qv4*100)/totalVetores,(qv0*100)/totalVetores,totalVetores);
            break;
        case DMPDS:
        case MPDS:
            totalVetores = q1+q2+q3+q4+q0;
            printf("Quadrante 1:%.4f\nQuadrante 2:%.4f\nQuadrante 3:%.4f\nQuadrante 4:%.4f\nCentro     :%.4f\nTOTAL Vetores:%d\n",(q1*100)/totalVetores,(q2*100)/totalVetores,(q3*100)/totalVetores,(q4*100)/totalVetores,(q0*100)/totalVetores,totalVetores);
            fprintf(f1,"%d;%.4f;%.4f;%.4f;%.4f;%.4f;%d\n",a->distance,(q1*100)/totalVetores,(q2*100)/totalVetores,(q3*100)/totalVetores,(q4*100)/totalVetores,(q0*100)/totalVetores,totalVetores);
            break;
        case DS:
            printf("Iteracoes: \t%.4f\n",iteracoes/vetores);
            //fprintf(f1,"%.4f;",iteracoes/vetores);
            break;
        default:
            break;
    }

    //fclose(f1);


    //libera memoria
    gsl_matrix_uchar_free(AF);
    gsl_matrix_uchar_free(cbAF);
    gsl_matrix_uchar_free(crAF);

    gsl_matrix_uchar_free(RF);
    gsl_matrix_uchar_free(cbRF);
    gsl_matrix_uchar_free(crRF);

    gsl_matrix_uchar_free(RecF);
    gsl_matrix_uchar_free(cbRecF);
    gsl_matrix_uchar_free(crRecF);

    gsl_matrix_ushort_free(ResF);
    gsl_matrix_ushort_free(cbResF);
    gsl_matrix_ushort_free(crResF);

    gsl_vector_int_free(vvector);
    gsl_vector_int_free(cbvvector);
    gsl_vector_int_free(crvvector);

    fclose(RecYUV);
    fclose(ResYUV);

    return 1;
}
