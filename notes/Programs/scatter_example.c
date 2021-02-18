
//mpicc scatter_example.c -o scatter_example
//mpirun -n 2 --hostfile myhostfile.txt ./scatter_example

#include <unistd.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 20

int main(int argc,char **argv) {
  
  
  int my_rank;
  int nprocs;
  int * a;

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
  int *recvbuffer=(int*)malloc(sizeof(int)*(N/nprocs));
  // printf("My rank: %d\n",my_rank);
  // printf("nprocs: %d\n",nprocs);
  
  if (my_rank == 0) { //Master
    a=(int*)malloc(sizeof(int)*N);
    //initialize a
    int i=0;
    for(i=0; i<N; i++){
      a[i]=i;
    }
  }

  //only the master (id 0) can scatter (don't need if statement for ranks)
  //original array, chunk to send, data type, buffer to receive, size of the buffer 
  //to receive, data type, sender (0-master), communicator
  MPI_Scatter(a, N/nprocs, MPI_INT, recvbuffer, N/nprocs, MPI_INT, 0, MPI_COMM_WORLD); 

  int j=0;
  for (j=0; j<N/nprocs; j++){
  printf("Rank: %d, val: %d\n",my_rank, recvbuffer[j]);
  }
  
  MPI_Finalize();
  return 0;
  
}




