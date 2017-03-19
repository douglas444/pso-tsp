#include <stdio.h>
#include <stdlib.h>


int* pso(double w, double c1, double c2,
          int numParticulas, int maxItera,
          int numDimensoes, double **grafo, double *fitness);
