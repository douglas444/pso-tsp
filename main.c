#include "PSO.h"
#include "tspReader.h"

int menu();
void pause();


int main() {

    double w, c1, c2, fitness, melhorFitness;
    double **grafo;
    int numParticulas, maxItera, numDimensoes, *melhorResultado,
        *resultado, i, numExecucoes, somatorioFitness, opcao;
    char nomeArquivo[100];
    FILE *f;
    TspInfo *tspInfo;

    tspInfo = NULL;
    resultado = NULL;
    grafo = NULL;
    opcao = -1;

    w = 1;
    c1 = 2;//cognitivo
    c2 = 2;//social

    printf("Informe o nome do arquivo de instancia (max 100): ");
    scanf("%s", nomeArquivo);
    printf("Informe o numero de particulas: ");
    scanf("%d", &numParticulas);
    printf("Informe o numero maximo de iteracoes: ");
    scanf("%d", &maxItera);

    system("tput reset");

    f = fopen(nomeArquivo, "r");
    tspInfo = read(f);
    fclose(f);

    grafo = tspInfo->distances;
    numDimensoes = tspInfo->dimension;

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
            system("tput reset");

            break;

        case 1:

            printf("Informe o numero de execucoes: ");
            scanf("%d", &numExecucoes);

            melhorFitness = -1;
            somatorioFitness = 0;

            for (i = 0; i < numExecucoes; ++i) {

                resultado = pso(w, c1, c2, numParticulas, maxItera, numDimensoes, grafo, &fitness);
                somatorioFitness += fitness;

                if(fitness < melhorFitness || melhorFitness == -1){
                    melhorFitness = fitness;
                    melhorResultado = resultado;
                }else{
                    free(resultado);
                    resultado = NULL;
                }
            }

            printf("Fitness da melhor solucao: %f\n\nRota da melhor solucao:\n\n", melhorFitness);
            for (i = 0; i < numDimensoes; ++i) {
                printf("%d\n", melhorResultado[i]);
            }

            printf("Media dos fitness de todas solucoes: %d\n", somatorioFitness/numExecucoes);

            free(melhorResultado);

            pause();
            system("tput reset");

            break;

        case 2:

            printf("Informe o nome do novo arquivo de instancia (max 100): ");
            scanf("%s", nomeArquivo);

            freeTspInfo(tspInfo);

            f = fopen(nomeArquivo, "r");
            tspInfo = read(f);

            grafo = tspInfo->distances;
            numDimensoes = tspInfo->dimension;

            system("tput reset");

            break;

        case 3:

            printf("Numero de iteracoes atual: %d\n", maxItera);
            printf("Numero de particulas atual: %d\n\n", numParticulas);

            printf("Informe o numero de particulas: ");
            scanf("%d", &numParticulas);

            printf("Informe o numero maximo de iteracoes: ");
            scanf("%d", &maxItera);

            system("tput reset");

            break;

        default:
            break;

        }

    }

    freeTspInfo(tspInfo);
    free(tspInfo);


    return 0;

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

    system("tput reset");

    return opcao;

}


void pause() {

    printf("\nPressione qualquer tecla para continuar");
    setbuf(stdin, NULL);
    getchar();

}
