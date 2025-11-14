
#include <stdio.h>

void trimm_par(int n, float *__restrict__ a, float *__restrict__ b,
               float *__restrict__ c)
{
     int i, j, k;
     const int size = n * n;
     const int GROUPSIZE = 1;

#pragma omp parallel private(k, i, j)
     {
          //  float r[size];
          //   #pragma omp for schedule(auto)
          for (k = 0; k < n; k++)
          {
#pragma omp for schedule(auto)// collapse(2)
               for (i = k; i < n; i++)
               {
                    for (j = 0; j <= k; j++)
                    {
                         //    c[i][j] = c[i][j] + a[i][k]*b[k][j];
                         c[i * n + j] = c[i * n + j] + a[i * n + k] * b[k * n + j];
                    }
               }
               //     #pragma omp for schedule(dynamic)
          }
     }
}
