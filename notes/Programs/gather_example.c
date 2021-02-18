
//mpicc gather_example.c -o gather_example
//mpirun -n 2 --hostfile myhostfile.txt ./gather_example

#include <unistd.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 20

int main(int argc,char **argv) {
  
  
  int my_rank;
  int nprocs;
  int * a;
  int *recvbuffer; //for master only
  
  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  MPI_Comm_size(MPI_COMM_WORLD,&nprocs);

  //each process gets a buffer of size N/nprocs
  int *sendbuffer=(int*)malloc(sizeof(int)*(N/nprocs));
  
  
  if (my_rank == 0) { //Master
    recvbuffer=(int*)malloc(sizeof(int)*N);
  }

  //initialize
  int i=0;
  for(i=0; i<N/nprocs; i++){
      sendbuffer[i]=i+(my_rank*(N/nprocs));
    //sendbuffer[i]=my_rank;
  }

  //buffer for each proc to send, size, data type, recvbuffer (only rank=0 needs to alloc this)
  //size of each array to receive, data type, receiving rank, communicator
  MPI_Gather(sendbuffer, N/nprocs, MPI_INT, recvbuffer, N/nprocs, MPI_INT, 0, MPI_COMM_WORLD);

  if (my_rank == 0) { //Master
    int j=0;
    for (j=0; j<N; j++)
    {
      printf("%d\n",recvbuffer[j]);
    }
  }
  
  MPI_Finalize();
  return 0;
  
}






