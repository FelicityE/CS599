//monte carlo calculate pi
//mpicc reduce_pi.c -lm -o reduce_pi
//mpirun -n 2 --hostfile myhostfile.txt ./reduce_pi

//Each processes generates N values to calculate PI
//Uses reduce to calculate the total sum (instead of gather)


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
  double sums=0; //for master only
  
  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
  
  //Multiple the rank by the seed so 
  //that each process gets a different random sequence
  srand(SEED*my_rank);

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

  double local_estimate_pi=((cnt*1.0)/(N*1.0))*4.0;

  printf("Rank %d, Estimate pi: %f\n",my_rank,local_estimate_pi);

  //Reduce the sums of all of the subvalues of PI
  MPI_Reduce(&local_estimate_pi, &sums, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  //master divides by the number of procs
  if (my_rank == 0) { //Master
    printf("Averaged PI estimate: %f\n",sums/nprocs);
  }
  
  MPI_Finalize();
  return 0;
  
}

