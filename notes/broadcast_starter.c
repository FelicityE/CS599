#include <stdio.h>
#include <unistd.h>
#include <mpi.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


//Example compile
//mpicc broadcast_starter.c -lm -o broadcast_starter

//Example execution
//mpirun -np 1 --hostfile myhostfile.txt broadcast_starter


int main(int argc, char **argv) {

  int my_rank, nprocs;

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  MPI_Comm_size(MPI_COMM_WORLD,&nprocs);

  if (nprocs<=1)
  {
    if (my_rank==0)
    fprintf(stderr,"Please enter >1 process ranks\n");      
    
    MPI_Finalize();
    exit(0);
  }


  //buffer to receive data
  //Data from rank 0 will be sent from this buffer
  int buffer;

  //Initialize the value to be broadcast
  if(my_rank==0)
  {
    buffer=100;
  }
  
  
  //Write code here
  //Part 1 -- homemade broadcast
  // int i=0;
  // if (my_rank==0)
  // {
  //   for (i=1; i<nprocs; i++)
  //   {
  //     MPI_Send(&buffer, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
  //   }
  // }
  // else
  // {
  //   MPI_Status status;
  //   MPI_Recv(&buffer, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
  // }
  
  //Part 2
  MPI_Bcast(&buffer, 1, MPI_INT, 0, MPI_COMM_WORLD);
  
  fprintf(stderr,"Rank: %d, my buffer: %d\n\n", my_rank, buffer);  

  MPI_Finalize();

  return 0;
}












