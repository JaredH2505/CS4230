

void mmtu_par(int n, double *__restrict__ a, double *__restrict__ b,
              double *__restrict__ c)
{
  int i, j, k;
//10 Gflops
#pragma omp parallel private(i, j, k)
  {


// #pragma omp master
#pragma omp for schedule(auto)
    for (i = 0; i < n; i++)
    {

      for (j = i; j < n; j++)
      {
        double r =0;
        for (k = i; k <= j; k++)
        {
          r += a[k * n + j] * b[k * n + i];
        }
        c[i * n + j] = r;
        // c[i * n + j] = c[i * n + j] + a[j * n + j] * b[j * n + i];
      }

    }
  }
}

