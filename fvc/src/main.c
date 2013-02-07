#include "../inc/types.h"
#include "../inc/ME.h"
#include "../inc/coder.h"

int main(int argc, char **argv) {

    clock_t start = 0, end = 0;
    double totalTime = 0;
    
    FILE *pw;
    ARG arg1;    
    int a;

    pw = fopen(argv[1], "rb");	//abre o arquivo yuv
    if (pw == NULL) {
        printf(" Erro ao abrir arquivo...\n");
        return 2;
    }

    strcpy(arg1.FileName, argv[1]); //nome arquivo que foi passado por linha de comando
    arg1.width = 1920;		//largura
    arg1.height = 1088;		//altura
    arg1.meRange = atoi(argv[2]);		//aréa de pesquisa NxN
    arg1.frames = 5;		//número de frames a serem processados
    arg1.meMode = atoi(argv[3]);    	//código do algoritmo ( em inc/types.h estão todos os códigos )
    arg1.fp = pw;
    arg1.distance = 10;		//parâmetro d para o MPDS

    arg1.chrSampling = 0;   //chroma sampling
    arg1.PEL = 0;			//amostragem de pixel no calculo do sad
    arg1.reserved = 3; 		//numero de steps do NSS
    arg1.sorteios = 96;


    if (arg1.meMode == 14 || arg1.meMode ==16)
    {	
            arg1.PEL=2;
    }
    if (arg1.meMode == 15 || arg1.meMode ==17)
    {	
            arg1.PEL=4;
    }

    start = clock();	//dispara o relógio
    a = code(&arg1);	//começa o processamento
    end = clock();		//finaliza o relógio

    totalTime = difftime(end, start); //calcula o tempo de processamento

    printf("\nTotal time: %.3f s\n", totalTime / CLOCKS_PER_SEC);		//imprime quantos segundos demorou

    fclose(pw);			//fecha o arquivo yuv

    return 0;
}
