#include <math.h>
#include <time.h>
#include "PSO.h"

typedef struct particula Particula;
typedef struct velocidade Velocidade;

struct velocidade {
    int **swaps, tamanho;
};

struct particula {
    int *posicao, *pb;
    Velocidade velo;
    float fitnessAtual, fitnessPb;
};

//Procedimentos auxiliares
void inicializaParticula(Particula *p, int numDimensoes);
void liberaParticula(Particula *p, int numDimensoes);

void inicializaVelocidade(Velocidade *v);
void liberaVetorSwaps(Velocidade *v);
void insereSwap(Velocidade *v, int *swap);

void liberaVetor(int **vetor, int n);
void copiaVetor(int *v1, int *v2, int n);

void inicializaPopulacao(Particula **particula, int numParticulas, int **gb,
float *fitnessGb, float **grafo, float c1, float c2, int numDimensoes);
float calculaFitness(int *posicao, float **grafo, int numDimensoes);


//Operadores
void alteraPosicao(int *posicao, Velocidade velo);
void multiplicaVelocidade(Velocidade *velo, int escalar);
void subtraiPosicoes(int *p1, int *p2, int numDimensoes, Velocidade *velo);
void somaVelocidades(Velocidade *velo1, Velocidade velo2);


int* pso(float w, float c1, float c2,
          int numParticulas, int maxItera,
          int numDimensoes, float **grafo, float *fitness) {

    srand((unsigned)time(NULL));

    int i, j;
    int *gb;
    Particula *particula;
    float r1, r2, fitnessGb = -1;
    Velocidade veloCognitiva, veloSocial;

    inicializaPopulacao(&particula, numParticulas, &gb, &fitnessGb, grafo, c1, c2, numDimensoes);

    for(i = 0; i < maxItera; ++i)
    {
        for(j = 0; j < numParticulas; ++j)
        {
            r1 = ((float) (rand()%1000))/1000;
            r2 = ((float) (rand()%1000))/1000;

            //Atualiza posição e fitness
            alteraPosicao(particula[j].posicao, particula[j].velo);
            particula[j].fitnessAtual = calculaFitness(particula[j].posicao, grafo, numDimensoes);

            //Atualiza pb se necessário
            if (particula[j].fitnessAtual < particula[j].fitnessPb) {
                copiaVetor(particula[j].pb, particula[j].posicao, numDimensoes);
                particula[j].fitnessPb = particula[j].fitnessAtual;
            }

            //Atualiza gb se necessário
            if (particula[j].fitnessPb < fitnessGb) {
                copiaVetor(gb, particula[j].posicao, numDimensoes);
                fitnessGb = particula[j].fitnessPb;
            }

            //Atualiza velocidade

            multiplicaVelocidade(&(particula[j]).velo, w);

            inicializaVelocidade(&veloCognitiva);
            inicializaVelocidade(&veloSocial);

            subtraiPosicoes(particula[j].pb, particula[j].posicao, numDimensoes, &veloCognitiva);
            subtraiPosicoes(gb, particula[j].posicao, numDimensoes, &veloSocial);

            multiplicaVelocidade(&veloCognitiva, c1*r1);
            multiplicaVelocidade(&veloSocial, c2*r2);

            somaVelocidades(&(particula[j].velo), veloCognitiva);
            somaVelocidades(&(particula[j].velo), veloSocial);

            liberaVetorSwaps(&veloCognitiva);
            liberaVetorSwaps(&veloSocial);


        }
    }

    for (i = 0; i < numParticulas; ++i) {
        liberaParticula(&(particula[i]), numDimensoes);
    }

    *fitness = fitnessGb;
    return gb;
}

void inicializaPopulacao(Particula **particula, int numParticulas,
                         int **gb, float *fitnessGb, float **grafo,
                         float c1, float c2, int numDimensoes) {

    float r2;
    int i, j, k;

    *gb = (int*) malloc(sizeof(int) * numDimensoes);
    *particula = (Particula*) malloc(numParticulas * sizeof(Particula));

    //Inicializa posição e pb de cada partícula e define o gb
    for(i = 0; i < numParticulas; ++i)
    {
        inicializaParticula( &( (*particula)[i] ), numDimensoes);

        //Randomiza posição
        for (j = 0; j < numDimensoes; ++j) {

            int flag = 0;

            while (!flag) {

                (*particula)[i].posicao[j] = rand()%numDimensoes;
                flag = 1;

                for(k = 0; k < numDimensoes; ++k) {
                    if((*particula)[i].posicao[j] == (*particula)[i].posicao[k] &&  j != k) {
                        flag = 0;
                    }
                }
            }
        }

        //Define fitness da posição atual e pb
        (*particula)[i].fitnessAtual = calculaFitness((*particula)[i].posicao, grafo, numDimensoes);
        (*particula)[i].pb = (int*) malloc(sizeof(int) * numDimensoes);
        (*particula)[i].fitnessPb = (*particula)[i].fitnessAtual;
        copiaVetor((*particula)[i].pb, (*particula)[i].posicao, numDimensoes);

        //Checa se pb da particula é o gb
        if ((*particula)[i].fitnessPb < *fitnessGb || *fitnessGb == -1) {
            copiaVetor(*gb, (*particula)[i].posicao, numDimensoes);
            *fitnessGb = (*particula)[i].fitnessPb;
        }
    }

    //Inicializa velocidade de cada partícula
    for(i = 0; i < numParticulas; ++i)
    {
        r2 = ((float) (rand()%1000))/1000;
        inicializaVelocidade(&((*particula)[i].velo));
        subtraiPosicoes(*gb, (*particula)[i].posicao, numDimensoes, &((*particula)[i].velo));
        multiplicaVelocidade(&((*particula)[i].velo), c2*r2);

    }
}


void inicializaParticula(Particula *p, int numDimensoes) {
    p->pb = (int*) malloc(sizeof(int) * numDimensoes);
    p->posicao = (int*) malloc(sizeof(int) * numDimensoes);
    inicializaVelocidade(&(p->velo));
}

void liberaParticula(Particula *p, int numDimensoes) {
    free(p->pb);
    p->pb = NULL;

    free(p->posicao);
    liberaVetorSwaps(&(p->velo));
    p->posicao = NULL;
}

void inicializaVelocidade(Velocidade *v) {
    v->swaps = (int**) malloc(sizeof(int*));
    v->swaps[0] = (int*) malloc(sizeof(int));
    v->swaps[0][0] = -1;
    v->swaps[0][1] = -1;
    v->tamanho = 0;
}

void liberaVetorSwaps(Velocidade *v) {
    int i;
    for (i = 0; i < v->tamanho; ++i) {
        free(v->swaps[i]);
    }
    free(v->swaps);
    v->swaps = NULL;
    v->tamanho = 0;
}

void insereSwap(Velocidade *v, int *swap) {
    int i, **newSwaps, tamanhoVelocidade;

    newSwaps = (int**) malloc(sizeof(int*) * (2 + v->tamanho));
    for (i = 0; i < v->tamanho; ++i) {
        newSwaps[i] = (int*) malloc(sizeof(int) * 2);
        copiaVetor(newSwaps[i], v->swaps[i], 2);
    }

    tamanhoVelocidade = v->tamanho;
    liberaVetorSwaps(v);

    newSwaps[i] = (int*) malloc(sizeof(int) * 2);
    newSwaps[i][0] = swap[0];
    newSwaps[i][1] = swap[1];

    newSwaps[i+1] = (int*) malloc(sizeof(int) * 2);
    newSwaps[i+1][0] = -1;
    newSwaps[i+1][1] = -1;

    v->swaps = newSwaps;
    v->tamanho = tamanhoVelocidade + 1;
}

void copiaVetor(int *v1, int *v2, int n) {
    int i;
    for(i = 0; i < n; ++i) {
        v1[i] = v2[i];
    }
}

float calculaFitness(int *posicao, float **grafo, int numDimensoes) {
    int i;
    float custo = 0;

    for (i = 1; i < numDimensoes; ++i) {
        custo += grafo[posicao[i-1]][posicao[i]];
    }
    return custo;
}

void alteraPosicao(int *posicao, Velocidade velo) {

    int swap[2], i, aux;

    for(i = 0; i < velo.tamanho; ++i) {
        copiaVetor(swap, velo.swaps[i], 2);
        aux = posicao[swap[0]];
        posicao[swap[0]] = posicao[swap[1]];
        posicao[swap[1]] = aux;
    }
}

void multiplicaVelocidade(Velocidade *velo, int escalar) {

    int i;
    Velocidade *novaVelo, veloTruncada;

    novaVelo = (Velocidade*) malloc(sizeof(Velocidade));

    inicializaVelocidade(novaVelo);


    for (i = 0; i < (int)escalar; ++i) {
        somaVelocidades(novaVelo, *velo);
    }

    if (escalar - (int)escalar > 0) {
        inicializaVelocidade(&veloTruncada);
        for (i = 0; i < (int)(velo->tamanho/(escalar - (int)escalar)); ++i) {
            insereSwap(&veloTruncada, velo->swaps[i]);
        }
        somaVelocidades(novaVelo, veloTruncada);
    }

    liberaVetorSwaps(velo);
    velo->swaps = novaVelo->swaps;
    velo->tamanho = novaVelo->tamanho;
}

void subtraiPosicoes(int *p1, int *p2, int numDimensoes, Velocidade *velo) {

    int i, j, swap[2];


    liberaVetorSwaps(velo);
    inicializaVelocidade(velo);

    for (i = 0; i < numDimensoes; ++i) {
        for(j = 0; j < numDimensoes; ++j) {
            if (p1[i] == p2[j] && i != j) {
                swap[0] = i;
                swap[1] = j;
                insereSwap(velo, swap);
            }
        }
    }
}

void somaVelocidades(Velocidade *velo1, Velocidade velo2) {
    int i, tamanhoNovaVelo, **swaps;

    tamanhoNovaVelo = velo1->tamanho + velo2.tamanho;
    swaps = (int**) malloc(sizeof(int*) * (1 + tamanhoNovaVelo));

    for (i = 0; i < velo1->tamanho; ++i) {
        swaps[i] = (int*) malloc(sizeof(int) * 2);
        copiaVetor(swaps[i], velo1->swaps[i], 2);
    }
    if (velo1->tamanho > 0) {
        for (i = velo1->tamanho; i < tamanhoNovaVelo; ++i) {
            swaps[i] = (int*) malloc(sizeof(int) * 2);
            copiaVetor(swaps[i], velo2.swaps[i - velo1->tamanho], 2);
        }
    } else {
        for (i = 0; i < velo2.tamanho; ++i) {
            swaps[i] = (int*) malloc(sizeof(int) * 2);
            copiaVetor(swaps[i], velo2.swaps[i], 2);
        }
    }

    swaps[i] = (int*) malloc(sizeof(int) * 2);

    swaps[i][0] = -1;
    swaps[i][1] = -1;

    liberaVetorSwaps(velo1);
    velo1->swaps = swaps;
    velo1->tamanho = tamanhoNovaVelo;
}
