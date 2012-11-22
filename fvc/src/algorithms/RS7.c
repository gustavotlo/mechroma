#include "../../inc/types.h"
#include "../../inc/ME.h"
#include "../../inc/coder.h"
#include <time.h>

int random7(MEPARM *meparm, unsigned int meRange) {

    srand(time(0));//inicia a funcao rand, NAO esqueca de falar isso em um artigo, usamos a funcao randomica de uma biblioteca stdlib e time do C

    int bestSAD = MAX;

    int truncFLAG = 0;
int t;
	int sector=-524;//variavel apenas para avaliar qual setor teve melhot PSNR
    int h = 0, w = 0;

    int operations [5]; //0 no centro, + 4 quadrantes, com DS em cada um dos melhores resultados sorteados
   
    int half = meRange / 2;//metade da area de pesquisa

    int center[2] = {0, 0}; // coordenadas (h,w) -> (Y,X)

    int SDSP[5][2] = {
        {0, 0},
        {0, 1},
        {1, 0},
        {0, -1},
        {-1, 0}
    };

    gsl_matrix_uchar *candBlock;
    gsl_matrix_uchar *targetBlock = meparm->targetBlock;
    gsl_matrix_uchar_view tempV;

//cria estruturas locais para armazenar os vetores e SAD's do centro e de cada um dos quatro quadrantes
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


//laco que divide os valores randomicos em quatro quadrantes, pega o mehlor ponto de cada quadrante e salva em um localparm
	register int i,j,k;
	int x,y,lIter=0;
	x=1;
	y=1;//variaveis para mudar os sinais, pegando valores em cada um dos quadrantes
	t = 0;//variavel para indexar estruturas temporarias contendo os vetores e sad
	for (i=0 ; i < 2 ; i++ )//i<2 pois inverte apenas 1 vez o y
	{
		for(j=0 ; j < 2 ; j++)//j<2 pois inverte apenas 1 vez o x
		{
			t++;//soma o indexador, a posicao 0 é para o DS no centro, entao ja soma 1 para indexar os pontos randomicos dos quadrantes
			bestSAD = MAX;
			for(k=0 ; k<meparm->randons/4;k++)//k/4 e a divisao por quadrante ex:ITER=16, 1º quadrante = 4 sorteios,2ºquadrante = 4soreteios
			{
				h = rand() % half * x;// a multiplicacao e para mexer com o sinal
				w = rand() % half * y;//ex 1º quadrante x=1 y=1 2º quadrante x=-1 y=1 3º quadrante x=-1 y=-1 4º quadrante x=1 y=-1	
				//printf("%d %d\n",h,w);
				//printf("vetores sorteados x= %d y= %d\n",h,w);//avaliacao para validacao
				if(isInsideFrame(h,w))//ve se o vetor sorteado esta dentro do frame
				{
					extractCandBlock(h,w);//extrai o bloco candidato
					truncFLAG = SADCalc(targetBlock, candBlock, &localParm[t]->SAD, bestSAD, meparm->PEL, &operations[t]);
					operations[t]++;//bloco candidato dentro do frame, somar 1 em oprerations
					if(!truncFLAG)//trunqFLAG retorna verdadeiro (1) caso o SAD do bloco extraido e maior que o bloco anterior
					{
						localParm[t]->vh = h;//atualiza na estrutura temporaria o vetor com melhor SAD 
						localParm[t]->vw = w;
						bestSAD = localParm[t]->SAD;

					}				
				}
				
			}
			x*=-1;		
		}
		y*=-1;
	}




	operations[0] = diamondSearch(localParm[0],meRange);//executa o diamond no centro localParm[0] e nos melhores pontos de de cada um dos quatro quadrantes
/*
* refinamento final nos melhores pontos achados dos quatro quadrantes
*/


for(i=1;i<5;i++)
{
	bestSAD = MAX;
    do {
        center[0] = localParm[i]->vh;
        center[1] = localParm[i]->vw;

        lIter++;

        for (j = 0; j < 5; j++) {
            h = SDSP[j][0] + center[0];
            w = SDSP[j][1] + center[1];

            if (isInsideFrame(h, w)) {
                extractCandBlock(h, w);
                truncFLAG = SADCalc(targetBlock, candBlock, &localParm[i]->SAD, bestSAD, meparm->PEL, &operations[i]);
		//printf("ITERANDO %d %d\n",h,w);		
                if (!truncFLAG) {
                    localParm[i]->vh = h;
                    localParm[i]->vw = w;
                    bestSAD = localParm[i]->SAD;
		//printf("ATUALIZANDO MELHOR RESULTADO\n");
                }
            }
        }
    } while ((localParm[i]->vh != center[0] || localParm[i]->vw != center[1])&& (localParm[i]->vh >= -half && localParm[i]->vh <= half && localParm[i]->vw >= -half && localParm[i]->vw <= half));
}


        
	
	bestSAD = MAX;//reinicializa bestSAD com valor maximo para achar o menor SAD no laco abaixo
	    for (t = 1; t < 5; t++) {	// avalia cada uma das estruturas locais em busca do melhor resultado
		if (localParm[t]->SAD < bestSAD) {
            meparm->vh = localParm[t]->vh;
            meparm->vw = localParm[t]->vw;
            bestSAD = localParm[t]->SAD;
			meparm->SAD = localParm[t]->SAD;
			sector = t;
            
        }
    }


	//printf("melhor setor: %d\n",sector);
	//getchar();
    meparm->SAD = bestSAD;
    return (operations[0]+operations[1]+operations[2]+operations[3]+operations[4]+lIter);
}

