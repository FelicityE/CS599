#include <stdio.h>
#include <unistd.h>
#include <mpi.h>
int main(int argc, char **argv) {
  // IMPORTANT: each process has its own i, my_rank, nprocs, x
  int i, my_rank, nprocs, x[4];

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

  if (my_rank == 0) { // Master
    x[0]=42; x[1]=43; x[2]=44; x[3]=45;
    MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
    for (i=1;i<nprocs;i++)
    {
      printf("Master: sending 4 ints to process: %d\n",i);
      // send 4 ints at address x to process i in default communicator with tag 0
      MPI_Send(x,4,MPI_INT,i,0,MPI_COMM_WORLD);
    }
  } else { // Worker
    MPI_Status status;
    // receive 4 ints at address x from process 0 in default communicator with tag 0
    MPI_Recv(x,4,MPI_INT,0,0,MPI_COMM_WORLD,&status);
    printf("Rank: %d, x[0]: %d, x[1]: %d, x[2]: %d, x[3]: %d\n",my_rank, x[0], x[1], x[2], x[3]);
  }

  MPI_Finalize();
  return 0;
}