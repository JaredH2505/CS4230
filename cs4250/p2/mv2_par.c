#include <omp.h>
void mv2_par(int n, double *__restrict__ m, double *__restrict__ x, double *__restrict__ y, double *__restrict__ z)
{
  int i, j;
#pragma omp parallel private(i,j)
 {
// #pragma omp master
// #pragma omp for schedule(auto)
  for (i = 0; i < n; i+=4)
  #pragma omp for schedule(auto)
   for (j = 0; j < n; j++) {
//    y[j] = y[j] + m[i][j] * x[i];
//    z[j] = z[j] + m[j][i] * x[i];
//
      y[j] = y[j] + m[i*n+j] * x[i];
      y[j] = y[j] + m[(i+1)*n+j] * x[i+1];
      y[j] = y[j] + m[(i+2)*n+j] * x[i+2];
      y[j] = y[j] + m[(i+3)*n+j] * x[i+3];

      z[j] = z[j] + m[j*n+i] * x[i];
      z[j] = z[j] + m[j*n+(i+1)] * x[i+1];
      z[j] = z[j] + m[j*n+(i+2)] * x[i+2];
      z[j] = z[j] + m[j*n+(i+3)] * x[i+3];
   }
 }
}
