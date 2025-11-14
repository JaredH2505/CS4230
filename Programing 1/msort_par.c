#include "omp.h"
#include <stdio.h>
#include <math.h>
#include "stdbool.h"

void msort_seq(int a[], int b[], int lo, int hi);
void merge(int a[], int b[], int lo, int mid, int hi);

void p(int a[], int size)
{
  for (int i = 0; i < size / 100; i++)
  {
    for (int j = 0; j < 100; j++)
    {
      if(j < size)
      printf(" %d", a[i * 100 + j]);
    }
    printf("\n");
  }
}

void msort_par(int a[], int b[], int lo, int hi)
{
  // printf("\n Starting new run \n");
  /* psudo Code
  get the number of threads NThreads
  devide the work into nThred parts
  Call seq code on those parts
  Merge all parts together
  return
  */
  int itter = 0;
  int DoneMerging[64];
  for(int i=0;i<64;i++)
  {
    DoneMerging[i]=0;
  }

#pragma omp parallel private(itter)
  {
    // printf("Starting theads");
    int Nthrds = omp_get_num_threads();
    int AllThreads = Nthrds;
    int t_ID = omp_get_thread_num();
    int size = (hi - lo) / Nthrds;
    int rem = (hi - lo) % Nthrds;
    // #pragma omp single
    // printf("size:%d \n",size);
    // printf("NT: %d\n",Nthrds);
    int start = size * t_ID;
    int end = size * (t_ID + 1);
    if (t_ID != 0)
      start++;
    if (t_ID == Nthrds - 1)
      end += rem;

    msort_seq(a, b, start, end);
    //DoneMerging[t_ID] =end;
    //printf("\nSeq mosrt on [%d:%d]\n",start,end);

    // Must do parallel for merging
    // Start with Nthrds parts and merver half
    // repeate untill all there is only one part left

// #pragma omp master
//     p(a, hi - lo);
// printf("Before Barrier");
#pragma omp barrier
// #pragma omp single
{
  // printf("\n\nAfter SEQ\n\n");
// p(a,hi-lo);
// printf("\n\n");
}
// printf("Passed barrier");
    bool done = false;
    bool working = true;
    itter = 1;



    while (itter < AllThreads)
    {
    // printf("Passed while");
      
      if(done)
      {

      }
      else
      {
        // printf("Passed else");
        if(t_ID < Nthrds/(2*itter))
        {
          // printf("T:%d I:%d N/I: %d\n", t_ID,itter,Nthrds/(2*itter));
          start = (t_ID * (2*itter)) * size;
          end = ((t_ID+1) * (2*itter)) *size;
          if(t_ID !=0) start++;
          if(end>hi) end=hi;
          if(t_ID >=( Nthrds/(2*itter))-1) end = hi;
          int mid= (start+end)/2;
          merge(a, b, start, mid, end);
        }
      
      }
#pragma omp barrier

      if(3 <= Nthrds/(2*itter) && t_ID == 0)
          {
            //Do 3 way merge
            // printf("Doing 3-Way merge\n");
            // printf("\n Pre Final Sorting\n");
              // p(a, hi - lo);
            // printf("T:%d I:%d N/I: %d\n", t_ID,itter,Nthrds/(2*itter));
          start = (t_ID * (2*itter)) * size;
          end = ((t_ID+1) * (4*itter)) *size;
          if(t_ID !=0) start++;
          if(end>hi) end=hi;
          //if(t_ID >=( Nthrds/(2*itter))-1) end = hi;
          int mid= (start+end)/2;
          //mid;

          // printf("Merge on thread:%d S:%d M:%d E:%d\n", t_ID, start, mid, end);
          merge(a, b, start, mid, end);
          // printf("\n\nAfter M1:\n\n");
          // p(a,hi-lo);
          // printf("T:%d I:%d N/I: %d\n", t_ID,itter,Nthrds/(2*itter));
          start = (t_ID * (2*itter)) * size;
          mid= end;
          end = hi;
          if(t_ID !=0) start++;
          if(end>hi) end=hi;
          //if(t_ID >=( Nthrds/(2*itter))-1) end = hi;

          // printf("Merge on thread:%d S:%d M:%d E:%d\n", t_ID, start, mid, end);
          merge(a, b, start, mid, end);
          // printf("\n\nAfter M2:\n\n");
          // p(a,hi-lo);
          done = true;

          start = 0;
          end = (hi-lo)/2;
          mid= (end/2)+10;
          if(t_ID !=0) start++;
          if(end>hi) end=hi;
          //if(t_ID >=( Nthrds/(2*itter))-1) end = hi;

          // printf("Merge on thread:%d S:%d M:%d E:%d\n", t_ID, start, mid, end);
          merge(a, b, start, mid, end);
          // printf("\n\nAfter M3:\n\n");
          // p(a,hi-lo);

          }

      itter++;
    }
    


    /* while (true)
    {

      printf("Starting loop with %d threads\n", Nthrds);
      size = (hi - lo) / Nthrds;
      rem = (hi - lo) % Nthrds;
      //#pragma omp single
      // printf("size:%d \n",size);
      start = size * t_ID;
      end = size * (t_ID + 1);
      if (t_ID != 0)
        start++;
      if (t_ID == Nthrds - 1)
        end += rem;
      int mid = (start + end) / 2;
      printf("Merge on thread:%d S:%d M:%d E:%d\n", t_ID, start, mid, end);
      merge(a, b, start, mid, end);
      double newThred = Nthrds / 2;
      // printf("NTheads: %d on thread:%d\n",Nthrds,t_ID);
      if (t_ID >= newThred || newThred <= 1)
      {
        printf("thread %d marked as done\n", t_ID);
        done = true;
      }
      else
      {
        Nthrds = ceil(newThred);
      }
    } */
    // printf("Ending while on thread %d\n",t_ID);
    // #pragma omp barrier
    // printf("waiting at the barrier thread %d\n",t_ID);
  }
  int mid = (lo + hi) / 2;
  //merge(a, b, lo, mid, hi);
  // printf("\nFinal Sorting\n");
  // p(a, hi - lo);
  // printf("Finished run\n");
}

// int mid = (lo+hi)/2;
//   merge(a,b,lo,mid,hi);
// int size = (hi-lo)/numParts;
//}

/*

while (itter < AllThreads)
    {
      
      if(done)
      {
      }else{
      double newThred = Nthrds / 2;
      printf("NTheads: %d on thread:%d\n",newThred, t_ID);
      if (t_ID >= newThred)
      {
        printf("thread %d marked as done\n", t_ID);
        done = true;
        if(t_ID ==0) merge(a,b,lo,end,hi);
        continue;
      }
      else
      {
        Nthrds = ceil(newThred);
      }
        printf("Starting loop with %d threads\n", Nthrds);
        size = (hi - lo) / Nthrds;
        rem = (hi - lo) % Nthrds;
        if(t_ID ==0) start =0;
        else start = DoneMerging[t_ID] +1; 
        
        end = size * (t_ID + 2);
        end*=2;
        if (t_ID != 0)
          start++;
        if (t_ID == Nthrds - 1)
          end += rem;
        if(end > hi) end=hi;
        int mid = (start + end) / 2;
        
        printf("Merge on thread:%d S:%d M:%d E:%d\n", t_ID, start, mid, end);
        merge(a, b, start, mid, end);

      
      //#pragma omp single
      // printf("size:%d \n",size);
      
      }
      itter++;

#pragma omp barrier
    }
















#pragma omp for schedule(static)
    for (int stage = 1; stage <Nthrds ; stage++)
    {
      printf("Stage: %d",stage);
        int wThrds =(Nthrds+1)/itter;
        printf("Running with T:%d WT:%d\n", t_ID,wThrds);
      if(t_ID < wThrds)
      {
        printf("T:%d made it past if onWT:%d\n", t_ID,wThrds);
      size = (hi - lo) / (Nthrds);
      rem = (hi - lo) % (Nthrds);
      // printf("S:%d R:%d\n", size,rem);
      size*=itter;

      start = size * t_ID;
      end = size * (t_ID + stage);
      if (t_ID != 0)
        start++;
      if (t_ID == Nthrds - 1)
        end += rem;
      if(end > hi) end=hi;
      if(start>hi) printf("Error in start");
      int mid = (start + end) / 2;
      printf("Merge on thread:%d S:%d M:%d E:%d\n", t_ID, start, mid, end);
      merge(a, b, start, mid, end);
      }
      itter *=2;


    }













      printf("T: %d starting on itter:%d\n",t_ID,itter);
      int size = (hi - lo) / Nthrds;
      int rem = (hi - lo) % Nthrds;
      //#pragma omp single
      int multi = p(2,itter);
      printf("size: %d rem: %d multi:%d \n", size,rem,multi);
      int start = (multi * size) * t_ID;
      int end = (multi *size) * (t_ID + itter+1);

      if (t_ID == Nthrds - 1)
        end += rem;
      int mid = (start + end) / 2;
      printf("M on S:%d m:%d e:%d onT:%d\n",start,mid,end,t_ID);
      merge(a, b, start, mid, end);
      itter++;
      if(t_ID >= Nthrds/itter || Nthrds/(2*itter) <= 1)
      {
      done = true;
      printf("T: %d is marked as done \n", t_ID);



//#pragma omp master
  {
   int temp,mid;
   if (lo < hi)

   {
     if (hi == lo+1)
     {
      #pragma omp critical
       if (a[hi]<a[lo]) {
         temp=a[hi];
         a[hi]=a[lo];
         a[lo]=temp;
       }
    }
     else
     {
      mid = (lo+hi)/2;

       #pragma omp parallel sections
     {
       #pragma omp section
       msort_seq(a,b,lo,mid);
      #pragma omp section
       msort_seq(a,b,mid+1,hi);
     }


     #pragma omp critical
       merge(a,b,lo,mid,hi);
     }

     */
