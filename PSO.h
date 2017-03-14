#include <stdio.h>
#include <stdlib.h>

int* pso(float w, float c1, float c2,
          int numParticulas, int maxItera,
          int numDimensoes, float **grafo, float *fitness);
