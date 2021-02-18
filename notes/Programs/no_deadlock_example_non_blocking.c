

//mpicc no_deadlock_example_non_blocking.c -o no_deadlock_example_non_blocking
//mpirun -n 2 --hostfile myhostfile.txt ./no_deadlock_example_non_blocking

#include <unistd.h>
#include <mpi.h>
#include <stdio.h>
int main(int argc, char **argv) {
  int i, my_rank, x, y;
  MPI_Status status;
  MPI_Request request=MPI_REQUEST_NULL;

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

  if (my_rank == 0) { // P0
    x=42;
    MPI_Isend(&x,1,MPI_INT,1,0,MPI_COMM_WORLD,&request);
    MPI_Recv(&y,1,MPI_INT,1,0,MPI_COMM_WORLD,&status);
    MPI_Wait(&request,&status); 
    printf("\nRank %d, received: %d",my_rank,y);
 } else if (my_rank == 1) { // P1
    y=41;
    MPI_Isend(&y,1,MPI_INT,0,0,MPI_COMM_WORLD,&request);
    MPI_Recv(&x,1,MPI_INT,0,0,MPI_COMM_WORLD,&status);
    MPI_Wait(&request,&status); 
    printf("\nRank %d, received: %d",my_rank,x);
  }

  MPI_Finalize(); 
  return(0);
}