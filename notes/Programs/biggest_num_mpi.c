//compute the biggest number 1 master, 3 workers
//master just creates the data and sends to processes
//receives the data and compares to find the biggest value

//mpicc biggest_num_mpi.c -o biggest_num_mpi
//mpirun -n 4 --hostfile myhostfile.txt ./biggest_num_mpi

#include <stdio.h>
#include <unistd.h>
#include <mpi.h>
int main(int argc, char **argv) {
  // IMPORTANT: each process has its own size, i, my_rank, nprocs, x
  unsigned int size=1000;
  int i, j, my_rank, nprocs, x[size];

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

  if (my_rank == 0) { // Master

    int biggest_arr[nprocs-1];

    MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
    
    for (i=1;i<nprocs;i++)
    {
      for (j=0; j<size; j++){
         x[j]=j+(i*size);
      }
       printf("Master: sending %d ints to process: %d\n",size,i);
      // send x ints at address x to process i in default communicator with tag 0
      MPI_Send(x,size,MPI_INT,i,0,MPI_COMM_WORLD);
    }

    //receive the biggest numbers and compare:
    MPI_Status status;
    for (i=1; i<nprocs; i++)
    {
      MPI_Recv(biggest_arr+i,1,MPI_INT,i,0,MPI_COMM_WORLD,&status);  
      printf("Master received: %d from rank %d\n", biggest_arr[i], i);
    }

    //print the biggest number:
    int biggest_master=-1;
    for (i=1; i<nprocs; i++)
    {
        if (biggest_arr[i]>biggest_master)
        {
          biggest_master=biggest_arr[i];
        }
    }

    printf("Biggest found by master: %d\n", biggest_master);




  } else { // Worker
    MPI_Status status;
    MPI_Recv(x,size,MPI_INT,0,0,MPI_COMM_WORLD,&status);
      
    int biggest=-1;  
    //each process finds biggest number
    for (i=0; i<size; i++)
    {
      if (biggest<x[i])
      {
        biggest=x[i];
      }
    }

    printf("Rank: %d, Biggest number found: %d\n",my_rank, biggest);  

    //send the biggest number to the master
    MPI_Send(&biggest,1,MPI_INT,0,0,MPI_COMM_WORLD);


  }

  MPI_Finalize();
  return 0;
}