#include "PSO.h"


int menu();
void pause();
void carregaInstanciaTSP(float ***grafo, int *qtdCidades, char *nomeArquivo);


int main() {

    float w, c1, c2, **grafo, fitness;

    int numParticulas, maxItera, numDimensoes,
        *resultado, i, numExecucoes, somatorioFitness, opcao;

    char nomeArquivo[100];

    resultado = NULL;
    grafo = NULL;
    opcao = -1;

    w = 0.5;
    c1 = 2.1;//cognitivo
    c2 = 2.1;//social

    printf("Informe o nome do arquivo de instancia (max 100): ");
    scanf("%s", nomeArquivo);
    printf("Informe o numero de particulas: ");
    scanf("%d", &numParticulas);
    printf("Informe o numero maximo de iteracoes: ");
    scanf("%d", &maxItera);

    system("clear");

    carregaInstanciaTSP(&grafo, &numDimensoes, nomeArquivo);

    while (opcao != 4 || opcao == -1) {

        opcao = menu();

        switch (opcao) {

        case 0:

            resultado = pso(w, c1, c2, numParticulas, maxItera, numDimensoes, grafo, &fitness);

            printf("Fitness: %f\n\nRota:\n\n", fitness);
            for (i = 0; i < numDimensoes; ++i) {
                printf("%d\n", resultado[i]);
            }

            free(resultado);
            resultado = NULL;

            pause();
            system("clear");

            break;

        case 1:

            printf("Informe o numero de execucoes: ");
            scanf("%d", &numExecucoes);

            somatorioFitness = 0;
            for (i = 0; i < numExecucoes; ++i) {
                pso(w, c1, c2, numParticulas, maxItera, numDimensoes, grafo, &fitness);
                somatorioFitness += fitness;
            }

            printf("Media fitness: %d\n", somatorioFitness/numExecucoes);
            pause();
            system("clear");

            break;

        case 2:

            for (i = 0; i < numDimensoes; ++i) {
                free(grafo[i]);
            }
            free(grafo);
            grafo = NULL;

            printf("Informe o nome do novo arquivo de instancia (max 100): ");
            scanf("%s", nomeArquivo);

            carregaInstanciaTSP(&grafo, &numDimensoes, nomeArquivo);

            system("clear");

            break;

        case 3:

            printf("Numero de iteracoes atual: %d\n", maxItera);
            printf("Numero de particulas atual: %d\n\n", numParticulas);

            printf("Informe o numero maximo de iteracoes: ");
            scanf("%d", &maxItera);

            printf("Informe o numero de particulas: ");
            scanf("%d", &numParticulas);

            system("clear");

            break;

        default:
            break;

        }

    }

    if (grafo != NULL) {

        for (i = 0; i < numDimensoes; ++i) {
            free(grafo[i]);
        }

    }
    free(grafo);

    return 0;

}


void carregaInstanciaTSP(float ***grafo, int *qtdCidades, char *nomeArquivo)
{
    int i, j;
    FILE *f;

    f = fopen(nomeArquivo, "r");

    if(f == NULL) {
        exit(1);
    }

    fscanf(f, "%d\n", qtdCidades);

    *grafo = (float**) malloc((*qtdCidades) * sizeof(float*));
    if(*grafo == NULL) {
        exit(1);
    }

    for(i = 0; i < *qtdCidades; ++i) {

        (*grafo)[i] = (float*) malloc((*qtdCidades) * sizeof(float));

        if((*grafo)[i] == NULL) {
            exit(1);
        }

    }


    for(i = 0; i < *qtdCidades - 1; ++i) {

        for(j = i + 1; j < *qtdCidades; ++j) {

            fscanf(f, "%f\n", &(*grafo)[i][j]);
            (*grafo)[j][i] = (*grafo)[i][j];

        }

    }

    fclose(f);
}


int menu() {

    int opcao;

    printf("Selecione uma das opcoes abaixo:\n");
    printf(" 0 - Executar;\n");
    printf(" 1 - Realizar execucao multipla;\n");
    printf(" 2 - Alterar instancia tsp;\n");
    printf(" 3 - Alterar nr. de iteracoes e nr. de particulas;\n");
    printf(" 4 - Sair;\n");
    printf("Opcao escolhida: ");
    scanf("%d", &opcao);

    system("clear");

    return opcao;

}


void pause() {

    printf("\nPressione qualquer tecla para continuar");
    setbuf(stdin, NULL);
    getchar();

}
