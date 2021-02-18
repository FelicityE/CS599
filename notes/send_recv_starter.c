#include <stdio.h>
#include <unistd.h>
#include <mpi.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


//Example compile
//mpicc send_recv_starter.c -lm -o send_recv_starter

//Example execution
//mpirun -np 1 --hostfile myhostfile.txt send_recv_starter


int main(int argc, char **argv) {

  int my_rank, nprocs;

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  MPI_Comm_size(MPI_COMM_WORLD,&nprocs);

  if (nprocs!=2)
  {
    if (my_rank==0)
    fprintf(stderr,"Please enter 2 process ranks\n");      
    
    MPI_Finalize();
    exit(0);
  }

  
  //buffer with my value
  int myValue;

  //buffer to receive data
  int myBuffer;

  //Initialize values at each rank
  if(my_rank==0)
  {
    myValue=100;
  }
  else
  {
    myValue=200;
  }


  // //Write code here
  // MPI_Status status;
  // if(my_rank==0)
  // {
  //   MPI_Send(&myValue, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
  //   MPI_Recv(&myBuffer, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
  // } 
  // else
  // {
  //   MPI_Recv(&myBuffer, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
  //   MPI_Send(&myValue, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  // }

  //Write code here
  MPI_Status status;
  MPI_Request request;
  if(my_rank==0)
  {
    MPI_Isend(&myValue, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
    MPI_Recv(&myBuffer, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
  } 
  else
  {
    MPI_Isend(&myValue, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
    MPI_Recv(&myBuffer, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
  }


  printf("\n\nRank: %d, my buffer: %d", my_rank, myBuffer);  

  MPI_Finalize();

  return 0;
}