
#include <stdio.h>
#include <unistd.h>
#include <mpi.h>

int main(int argc, char **argv) {
  int my_rank, n;
  char hostname[128];
  int namelen;
  MPI_Init(&argc,&argv);	// Called once
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  MPI_Comm_size(MPI_COMM_WORLD,&n);
  MPI_Get_processor_name(hostname,&namelen);
  if (my_rank == 0) { // Master
    printf("I am the master: %s\n",hostname);
  } else { // Worker
    printf("I am a worker: %s (rank=%d/%d) at time %lf\n",
           hostname,my_rank,n-1,MPI_Wtime());
  }
  MPI_Finalize();  // Call once
  return 0;
}
