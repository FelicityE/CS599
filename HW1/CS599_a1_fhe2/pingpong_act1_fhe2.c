#include <stdio.h>
#include <unistd.h>
#include <mpi.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


//Example compilation
//mpicc pingpong_act1_fhe2.c -lm -o pingpong_act1_fhe2

//Example execution
//mpirun -np 8 -hostfile myhostfile.txt ./pingpong_act1_fhe2



int main(int argc, char **argv) {

  int my_rank, nprocs;

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  MPI_Comm_size(MPI_COMM_WORLD,&nprocs);

  //make sure even number of procs
  if ((nprocs%2)!=0)
  {
    if (my_rank==0)  
    {
    printf("\nYou must enter an even number of process ranks"); 
    }
    MPI_Finalize();
    return 0;
  }

  //Write code here

  //buffer with my value
  int myValue;

  //buffer to receive data
  int myBuffer;

  //counter to add data
  int mycounter;

  //Init values
  myValue = my_rank;

  MPI_Status status;
  for(int i = 0; i < 5; i++){
    if(my_rank%2==0)
    {
      MPI_Send(&myValue, 1, MPI_INT, my_rank+1, 0, MPI_COMM_WORLD);
      MPI_Recv(&myBuffer, 1, MPI_INT, my_rank+1, 0, MPI_COMM_WORLD, &status);
      mycounter += myBuffer;
    } 
    else
    {
      MPI_Recv(&myBuffer, 1, MPI_INT, my_rank-1, 0, MPI_COMM_WORLD, &status);
      MPI_Send(&myValue, 1, MPI_INT, my_rank-1, 0, MPI_COMM_WORLD);
      mycounter += myBuffer;
    }
  }

  printf("\nRank: %d, my buffer: %d, my counter: %d\n\n", my_rank, myBuffer, mycounter); 

  MPI_Finalize();
  return 0;
}
