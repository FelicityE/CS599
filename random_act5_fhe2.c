#include <stdio.h>
#include <unistd.h>
#include <mpi.h>
#include <stdlib.h>
#include <math.h>

//Example compilation
//mpicc random_act5_fhe2.c -lm -o random_act5_fhe2

//Example execution
//mpirun -np 50 -hostfile myhostfile.txt ./random_act5_fhe2

//Do not change the seed, or your answer will not be correct
#define SEED 72

//Change this if you want, but make sure it is set to 10 when submitting the assignment
#define TOTALITER 10

int generateRandomRank(int max_rank, int my_rank);

int main(int argc, char **argv) {
  int i, my_rank, nprocs;

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  MPI_Comm_size(MPI_COMM_WORLD,&nprocs);

  if (nprocs<3)
  {
    if (my_rank==0)
    printf("\nRun with at least 3 ranks.\n\n");
    MPI_Finalize();
    return 0;
  }

  //seed rng do not modify
  srand(SEED+my_rank);


  //WRITE CODE HERE  
  //buffer to receive data
  int myBuffer;
  int my_counter;
  int current_rank;
  int next_rank;

  MPI_Status status;
  
  current_rank = 0;
  my_counter = 0;

  for(i = 0; i < TOTALITER; i++){
    if(my_rank == current_rank){
      next_rank = generateRandomRank(nprocs-1, my_rank);
      printf("My rank: %d, next to recv: %d\n\n", my_rank, next_rank);
      MPI_Send(&my_counter, 1, MPI_INT, next_rank, 0, MPI_COMM_WORLD);
    }
    MPI_Bcast(&next_rank, 1, MPI_INT, current_rank, MPI_COMM_WORLD);
    if(my_rank == next_rank){
      MPI_Recv(&myBuffer, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
      my_counter = myBuffer;
      printf("My rank: %d, old counter: %d\n\n",my_rank, my_counter);
      my_counter += my_rank;
      printf("My rank: %d, new counter: %d\n\n",my_rank, my_counter);
      current_rank = my_rank;
    }
    MPI_Bcast(&current_rank, 1, MPI_INT, next_rank, MPI_COMM_WORLD);
  }


  MPI_Finalize();
  return 0;
}


//Do not modify the rank generator or you will get the wrong answer
//returns a rank between 1 and max_rank, but does not return itself
//does not generate rank 0
int generateRandomRank(int max_rank, int my_rank)
{
  
  int tmp=round(max_rank*((double)(rand()) / RAND_MAX));
  while(tmp==my_rank || tmp==0)
  {  
  tmp=round(max_rank*((double)(rand()) / RAND_MAX)); 
  }

  return tmp;
}



