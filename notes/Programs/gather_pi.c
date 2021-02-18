//monte carlo calculate pi
//mpicc gather_pi.c -lm -o gather_pi
//mpirun -n 2 --hostfile myhostfile.txt ./gather_pi
//Each processes generates N values to calculate PI
//Uses gather to send them to the master node and average the values


#include <unistd.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 100000
#define SEED 57

int main(int argc,char **argv) {
  int my_rank;
  int nprocs;
  int * a;
  double *recvbuffer; //for master only
  
  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
  
  //Multiple the rank by the seed so 
  //that each process gets a different random sequence
  srand(SEED*my_rank);

  //each proc sends a single estimate of pi to the master   
  if (my_rank == 0) { //Master
    recvbuffer=(double*)malloc(sizeof(double)*nprocs);
  }

  //all processes calculate pi
  //generate their own random numbers
  double x=0;
  double y=0;
  unsigned int cnt=0;

  for (int i=0; i<N; i++){
      x=((double)(rand()) / RAND_MAX);
      y=((double)(rand()) / RAND_MAX);

      if(sqrt((x*x)+(y*y))<1.0){
        cnt++;
      }
  }

  double estimate_pi=((cnt*1.0)/(N*1.0))*4.0;

  printf("Rank %d, Estimate pi: %f\n",my_rank,estimate_pi);

  //buffer for each to send, size, data type, recvbuffer (only rank=0 needs to alloc this)
  //size of each array to receive, data type, receiving rank, communicator
  MPI_Gather(&estimate_pi, 1, MPI_DOUBLE, recvbuffer, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  if (my_rank == 0) { //Master
    int j=0;
    double sum_pi=0;
    for (j=0; j<nprocs; j++)
    {
      sum_pi+=recvbuffer[j];
    }

    printf("Averaged PI estimate: %f\n",sum_pi/(nprocs*1.0));
  }
  
  MPI_Finalize();
  return 0;
  
}