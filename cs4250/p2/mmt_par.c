#define CHUNK (3)

void mmt_par(int n, double *__restrict__ a, double *__restrict__ b,
             double *__restrict__ c)
{
   int i, j, k;

#pragma omp parallel private(i, j, k)
   {
      // #pragma omp for schedule(auto)
      int rem = n%CHUNK;
      for(k=0;k<rem;k++)
      {
      #pragma omp for schedule(auto)
         for (i = 0; i < n; i++)
         {
            for (j = 0; j < n; j++)
            {
               // i,j
               c[i * n + j] = c[i * n + j] + a[k * n + j] * b[k * n + i];
            }
         }
      }

      for (k = rem; k < n; k+=CHUNK)
      {

#pragma omp for schedule(auto)
         for (i = 0; i < n; i++)
         {
            for (j = 0; j < n; j++)
            {
               // i,j
               c[i * n + j] = c[i * n + j] + a[k * n + j] * b[k * n + i];
               c[i * n + j] = c[i * n + j] + a[(k+1) * n + j] * b[(k+1) * n + i];
               c[i * n + j] = c[i * n + j] + a[(k+2) * n + j] * b[(k+2) * n + i];
            }
         }

      }

   }
}