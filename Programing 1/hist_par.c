#include <stdio.h>
#include <stdlib.h>



void pa1_hist_par(int nelts, int nbins, int *__restrict__ data, int *__restrict__ hist) 
{
for(int i=0;i<nbins;i++)
{
  hist[i]=0;
}
#pragma omp parallel //private()
 {
   int result[nbins]; 
  {
   for (int i=0;i<nbins;i++)
   {
      result[i]=0;
   } 

    #pragma omp for
    for (int i = 0; i < nelts; i++)
    {
      //int value = data[i];
      result[data[i]] +=1;

    } 

    for (int i=0;i<nbins;i++)
   {
    #pragma omp critical
    {
      hist[i]+=result[i];

    }

   } 
  }
 }

}

  /* //Manual loop
  int t_id = omp_get_thread_num();
  int nthrds = omp_get_num_threads();
  int rem = nelts % nthrds;
  #pragma omp single nowait
  for (int i = 0; i < rem; i++)
    {
      result[data[i]] += 1;
    }

  for(int i = rem + t_id;i<nelts;i+=nthrds)
  {
    result[data[i]] += 1;
    printf("%d\n", i);
  }  */

/*



 /*  int* result;// = malloc(nbins * sizeof(int));

#pragma omp parallel private(result)
 {
//#pragma omp master
  {
/** master Copy 
  const int t_id = omp_get_thread_num();
  const int nthrds = omp_get_num_threads();
  result = malloc(nbins * sizeof(int));
  
  #pragma omp parallel for
   for (int i=0;i<nbins;i++)
   {
     hist[i]=0;
     //r_Private[i]
     result[i]=0;
    //  for(int j =0;j<nthrds;j++)
    //  {
    //    //r_Private[i+(j*nbins)] =0;
    //    //result[i+(j*nbins)] =0;
    //  }
   }

   #pragma omp parallel for
   for (int i = 0; i < nelts; i++) 
   {
     int loc = data[i];//+(t_id* nbins);
     //printf("loc: %d \n", loc); 
     result[loc] += 1;
     //r_Private[data[i]] +=1;
   }

   #pragma omp critical
  for(int i=0;i<nbins;i++)
  {
    hist[i]+=result[i];//r_Private[i];

    // //#pragma omp for
    // for(int j=0;j<nthrds;j++)
    // {
    //   //#pragma attomic
    //   hist[i]+=result[i];
    // }
  }
 
  }
 } */

