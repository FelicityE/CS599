//mpicc mergesort_mpi.c -o mergesort_mpi
//mpirun -n 10 --hostfile myhostfile.txt ./mergesort_mpi

//Keep the functions but remove the stuff in main for an in-class exercise
//Remove the check if sorted function
//Remove the Merge function (if time)

#include <stdio.h>
#include <unistd.h>
#include <mpi.h>
#include <stdlib.h>

#define N 200000000
#define SEED 72


/////////////////////////////////////

int compare_function(const void *a, const void *b) {
double *x = (double *) a;
double *y = (double *) b;
// return *x - *y; 
if (*x < *y) return -1;
else if (*x > *y) return 1; return 0;
}

void generateRandomData(double * data)
{
  srand(SEED);
  for (int i=0; i<N; i++){
      data[i]=1000.0*((double)(rand()) / RAND_MAX);
  }
}


//A- input array
//a- |A|
//B- input array
//b- |B|
//output: storage for the output of size |A|+|B|
void mergeOLD (double *A, int a, double *B, int b, double *output) 
{
  int i,j,k;
  i = 0; 
  j = 0;
  k = 0;
  while (i < a && j < b) {
    if (A[i] <= B[j]) {
    //copy A[i] to output[k] 
    output[k] = A[i];
    i++;
    k++;
    }
    else {
      // copy B[j] to output[k]
      output[k] = B[j];
      j++;
      k++;
    }
  }
  // remaining elements in A to output
  while (i < a) {
    output[k]= A[i];
    i++;
    k++;
  }
  // remaining elements in B to output
  while (j < b)  {
    output[k]= B[j];
    j++;
    k++;
  }

  // printf("Output:\n");
  // for (int i=0; i<a+b; i++)
  // {
  //   printf("%f\n",output[i]);
  // }

}  

//A- input array
//a- beginning index
//b- middle index
//c- end index
//modifies A as output, uses temp storage
void merge_same_Arr(double *A, int a, double b, double c) 
{

  //create copy of the input
  double * tmp=(double*)malloc(sizeof(double)*N);
  int x=0;
  for (x=0; x<c; x++)
  {
    tmp[x]=A[x];
  }

  int i,j,k;
  i = 0; 
  j = b;
  k = 0; //counter of output array
  while (i < b && j < c) {
    if (tmp[i] <= tmp[j]) {
    //copy A[i] to output[k] 
    A[k] = tmp[i];
    i++;
    k++;
    }
    else {
      // copy B[j] to output[k]
      A[k] = tmp[j];
      j++;
      k++;
    }
  }
  // remaining elements in A to output
  while (i < b) {
    A[k]= tmp[i];
    i++;
    k++;
  }
  // remaining elements in B to output
  while (j < c)  {
    A[k]= tmp[j];
    j++;
    k++;
  }

  // printf("Output:\n");
  // for (int i=0; i<c; i++)
  // {
  //   printf("%f\n",A[i]);
  // }

  free(tmp);

} 


void checkIfSorted(double * data)
{
  int i=0;
  char flag=1;
  
  for (i=0; i<N-1; i++)
  {
    if (data[i]>data[i+1]){
      flag=0;
    }
    // printf("%f\n",data[i]);
  }

  if (flag==0)
  {
    printf("Array not sorted!!\n");
  }
  else
  {
    printf("Array sorted :)\n");
  }
}



/////////////////////////////////////



int main(int argc, char **argv) {
  // IMPORTANT: each process has its own size, i, my_rank, nprocs, x
  int i, j, my_rank, nprocs;
  

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  MPI_Comm_size(MPI_COMM_WORLD,&nprocs);

  unsigned int datasize=N/nprocs;
  if (N%datasize!=0)
  {
      printf("Exiting, total size does not divide evenly into the size sent to each process\n");
      return 0;
  }



  if (my_rank == 0) { // Master

    printf("Sorting a total of %f GiB\n",(N*1.0*8.0)/(1024*1024*1024.0));



    double * inputArr;
    inputArr=(double *)malloc(N*sizeof(double)); 

    generateRandomData(inputArr);

    for (i=1;i<nprocs;i++){
      printf("Master: sending %d doubles to process: %d\n", datasize, i);
      // send x ints at address x to process i in default communicator with tag 0
      MPI_Send(inputArr+(i*datasize),datasize,MPI_DOUBLE,i,0,MPI_COMM_WORLD);
    }

    //master sorts its chunk
    qsort(inputArr, datasize, sizeof(double), compare_function);

    //receive the chunks
    MPI_Status status;
    for (i=1; i<nprocs; i++)
    {
      MPI_Recv(inputArr+(i*datasize),datasize,MPI_DOUBLE,i,0,MPI_COMM_WORLD,&status);  
      printf("Master received sorted list from rank %d\n", i);
    }

    // printf("Before: \n");
    // for (i=0; i<N; i++)
    // {
    //   printf("%f\n",inputArr[i]);
    // }
    
    // merge_same_Arr(inputArr,0,datasize,datasize*2);
    // merge_same_Arr(inputArr,0,datasize*2,datasize*3);
    // merge_same_Arr(inputArr,0,datasize*3,datasize*4);

    for (i=1; i<nprocs; i++){
        merge_same_Arr(inputArr,0,datasize*i,datasize*(i+1));
    }

    // printf("After: \n");
    // for (i=0; i<N; i++)
    // {
    //   printf("%f\n",inputArr[i]);
    // }

    //check if sorted:
    checkIfSorted(inputArr);

  } else { // Worker

    printf("Worker: %d\n",my_rank);

    double * subList;
    subList=(double*)malloc(sizeof(double)*datasize);

    MPI_Status status;
    MPI_Recv(subList,datasize,MPI_DOUBLE,0,0,MPI_COMM_WORLD,&status);
      
    qsort(subList, datasize, sizeof(double), compare_function);
    
    //send the sorted list to the master
    MPI_Send(subList,datasize,MPI_DOUBLE,0,0,MPI_COMM_WORLD);


  }

  MPI_Finalize();
  return 0;
}

