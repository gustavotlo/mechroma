#include "../../inc/types.h"
#include "../../inc/ME.h"
#include "../../inc/coder.h"

float qv0 = 0; 
float qv1 = 0;
float qv2 = 0;
float qv3 = 0;
float qv4 = 0;

int vectorMultiPointDiamondSearch(MEPARM * meparm, unsigned int dist,unsigned int meRange) {
	int half = meRange / 2;
    int t;
    int operations [5];
    int bestSAD = MAX;
    int sector = 524;
static int ds[5][2]; // matriz estatica que contera os vetores H e Y anteriores para cada uma dos 5 diamond (o centro ds[0][0] e ds[0][1] nao usamos)
int mesmaLinha = meparm->cw;//a cada nova linha meparm->cw recebe 0, logo entra no if apenas quando for na mesma linha

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
//printf("VETORES ESTATICOS:\n");
//printf("ch: %d cw: %d\n",ds[0][0],ds[0][1]);
//printf("1ºh: %d 1ºw: %d\n",ds[1][0],ds[1][1]);
//printf("2ºh: %d 2ºw: %d\n",ds[2][0],ds[2][1]);
//printf("3ºh: %d 3ºw: %d\n",ds[3][0],ds[3][1]);
//printf("4ºh: %d 4ºw: %d\n",ds[4][0],ds[4][1]);
//printf("VETORES ANTERIORES OU REINI\n");

	if(mesmaLinha)//mesma linha, vetores atuais recebem vetores anteriores (estaticos)
	{
		if(ds[1][0] == MAX)//caso o vetor anterior foi setado com MAX, indica que não é valido, fora da area de pesquisa
		{
			localParm[1]->vh += dist;
			localParm[1]->vw += dist;
		}
		else//caso seja um vetor valido, recebe os vetores anteriores
		{
			localParm[1]->vh = ds[1][0];
			localParm[1]->vw = ds[1][1] + BLOCKSIZE;
		}
		if(ds[2][0] == MAX)//caso o vetor anterior foi setado com MAX, indica que não é valido, fora da area de pesquisa
		{
			localParm[2]->vh -= dist;
			localParm[2]->vw += dist;
		}
		else//caso seja um vetor valido, recebe os vetores anteriores
		{
			localParm[2]->vh = ds[2][0] ;
			localParm[2]->vw = ds[2][1] + BLOCKSIZE;
		}
		if(ds[3][0] == MAX)//caso o vetor anterior foi setado com MAX, indica que não é valido, fora da area de pesquisa
		{
			localParm[3]->vh -= dist;
			localParm[3]->vw -= dist;
		}
		else//caso seja um vetor valido, recebe os vetores anteriores
		{
			localParm[3]->vh = ds[3][0] ;
			localParm[3]->vw = ds[3][1] + BLOCKSIZE;
		}
		if(ds[4][0] == MAX)//caso o vetor anterior foi setado com MAX, indica que não é valido, fora da area de pesquisa
		{
			localParm[4]->vh += dist;
			localParm[4]->vw -= dist;
		}
		else//caso seja um vetor valido, recebe os vetores anteriores
		{
			localParm[4]->vh = ds[4][0];
			localParm[4]->vw = ds[4][1] + BLOCKSIZE;
		}
	}
	else//começou nova linha, os vetores são reinicializados
	{
		//setar cada localParm;
	   	 localParm[1]->vh += dist;
		localParm[1]->vw += dist;

		localParm[2]->vh -= dist;
		localParm[2]->vw += dist;

		localParm[3]->vh -= dist;
		localParm[3]->vw -= dist;

		localParm[4]->vh += dist;
		localParm[4]->vw -= dist;
	}
//printf("ch: %d cw: %d\n",localParm[0]->vh,localParm[0]->vw);
//printf("1ºh: %d 1ºw: %d\n",localParm[1]->vh,localParm[1]->vw);
//printf("2ºh: %d 2ºw: %d\n",localParm[2]->vh,localParm[2]->vw);
//printf("3ºh: %d 3ºw: %d\n",localParm[3]->vh,localParm[3]->vw);
//printf("4ºh: %d 4ºw: %d\n",localParm[4]->vh,localParm[4]->vw);
    
printf("VETORES GERADOS\n");

    for (t = 0; t < 5; t++){			//executa as 5 instancias, os vetores serão enviados para o Diamond Search apenas se estiverem dentro do frame e dentro da area de pesquisa
        if (
        ((localParm[t]->ch - localParm[t]->vh) >= 0) 					&&
        ((localParm[t]->ch - localParm[t]->vh) <= meparm->RF->size1- BLOCKSIZE)	&&
        ((localParm[t]->cw + localParm[t]->vw) >= 0) 					&&
        ((localParm[t]->cw + localParm[t]->vw) <= meparm->RF->size2 - BLOCKSIZE) && (localParm[t]->vh <= half) && (localParm[t]->vw <= half)
        ){
			operations[t] = diamondSearch(localParm[t],meRange);
		}
        
	}

//printf("ch: %d cw: %d\n",localParm[0]->vh,localParm[0]->vw);
//printf("1ºh: %d 1ºw: %d\n",localParm[1]->vh,localParm[1]->vw);
//printf("2ºh: %d 2ºw: %d\n",localParm[2]->vh,localParm[2]->vw);
//printf("3ºh: %d 3ºw: %d\n",localParm[3]->vh,localParm[3]->vw);
//printf("4ºh: %d 4ºw: %d\n",localParm[4]->vh,localParm[4]->vw);
//printf("----------------------------------\n");
//getchar();
    for (t = 0; t < 5; t++) {	// busca pelo pelo melhor resultado de sad
		ds[t][0] = localParm[t]->vh;//recebe os vetores gerados para a altura H
		ds[t][1] = localParm[t]->vw;//recebe os vetores gerados para a largura W
		if (localParm[t]->SAD < bestSAD) {
            meparm->vh = localParm[t]->vh;
            meparm->vw = localParm[t]->vw;
            bestSAD = localParm[t]->SAD;
			meparm->SAD = localParm[t]->SAD;
            sector = t;
        }
	else if(localParm[t]->SAD == MAX) // se o sad é maximo entao o vetor nem passou do if para executar o DS, logo não é um vetor valido
	{
		ds[t][0] = MAX;//liga a flag indicando que não é valido
	}	

    }
	/* ajuste de quadrantes de calculo para quadrantes da matematica, geram apenas porcentagens de ocorrencia em cada quadrante
	*   
	* 	3º	2º	=>	2º	1º
	*	4º	1º	=>	3º	4º
	*/	
	switch(sector){
		case 0:
			qv0++;
		break;
		case 1:
			qv4++;
		break;
		case 2:
			qv1++;
		break;
		case 3:
			qv2++;
		break;
		case 4:
			qv3++;
		break;
		default:
			printf("Quadrante inválido");
			exit(1);
		break;
	}
	
	//getchar();
    for (t = 0; t < 5; t++)	//libera cada meparm
        free(localParm[t]);
   
    return (operations[0]+operations[1]+operations[2]+operations[3]+operations[4]);
}
