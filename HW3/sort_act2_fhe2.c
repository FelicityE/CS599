#include <stdio.h>
#include <unistd.h>
#include <mpi.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//Example compilation
//mpicc -O3 sort_act2_fhe2.c -lm -o sort_act2_fhe2

//Example execution
//mpirun -np 3 -hostfile myhostfile.txt ./sort_act2_fhe2

void generateData(int * data, int SIZE);


int compfn (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}

#define VERBOSE 0

//Do not change the seed
#define SEED 72
// #define MAXVAL 1000000
#define MAXVAL 1000

//Total input size is N, divided by nprocs
//Doesn't matter if N doesn't evenly divide nprocs
// #define N 1000000000
#define N 1000000

int main(int argc, char **argv) {

  int my_rank, nprocs;

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  MPI_Comm_size(MPI_COMM_WORLD,&nprocs);

  //seed rng do not modify
  srand(SEED+my_rank);


  //local input size N/nprocs
  const unsigned int localN=N/nprocs;

  //All ranks generate data
  int * data=(int*)malloc(sizeof(int)*localN);

  generateData(data, localN);

  int * sendDataSetBuffer=(int*)malloc(sizeof(int)*localN); //most that can be sent is localN elements
  int * recvDatasetBuffer=(int*)malloc(sizeof(int)*localN); //most that can be received is localN elements
  int * myDataSet=(int*)malloc(sizeof(int)*N); //upper bound size is N elements for the rank


  //Write code here
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Checking sum before
  unsigned long int localsum = 0;
  for(int i = 0; i < localN; i++){
    localsum += data[i];
  }
  unsigned long int globalSum = 0;
  MPI_Reduce(&localsum, &globalSum, 1, MPI_UNSIGNED_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
  if(my_rank == 0){
    printf("Sum: %ld\n\n", globalSum);
  }


  // Timing
  MPI_Barrier(MPI_COMM_WORLD);
  double tstart=MPI_Wtime();
  
  int inSize = 0; // incoming size/recv size buffer, assume 0 incase no data recieved from rank x; 
  int all_inSize[nprocs];
  
  int ror = MAXVAL/nprocs; //range over ranks
  //everyone
  int mystart = my_rank*ror;
  int myend = mystart + ror;
  // unless my rank is last rank
  if(my_rank == nprocs-1){
    myend += MAXVAL%nprocs;
  }
  
  // mybucket = [mystart, myend)
  if(VERBOSE){
    printf("My rank: %d, My bucket: [%d, %d) \n", my_rank, mystart, myend);
  }

  //allocate old_temp_data = size data (localN)
  int * oldData = (int*)malloc(sizeof(int)*localN);
  // add data to old_temp_data
  for(int i = 0; i < localN; i++){
    oldData[i] = data[i];
  }
  //allocate new_temp_data = size old_temp_data
  int * newData = (int*)malloc(sizeof(int)*localN);

  int oldData_len = localN;

  int newData_len;  
  MPI_Status status;
  MPI_Request request;
  

  // for each rank expect my rank
  for(int i = 0; i < nprocs; i++){
    if(i != my_rank){
      // i bucket
      int istart = i*ror;
      int iend = istart + ror;
      if(i == nprocs-1){
        iend += MAXVAL%nprocs;
      }
      // Number of data moved counter
      int send_len = 0;
      // postion in new data counter
      newData_len = 0;

      // for all old_temp_data
      for(int j = 0; j < oldData_len; j++){
        // if not mine and in i's bucket
        if((oldData[j] < mystart || oldData[j] >= myend) &&
            (oldData[j] >= istart && oldData[j] < iend)){
          // put in send buffer
          sendDataSetBuffer[send_len] = oldData[j];
          //update send position/number of data being moved
          send_len++;
        }else{
          //new_temp_data adds data not sent
          newData[newData_len] = oldData[j];
          // update new_temp_data_pos
          newData_len++;
        }
      }

      oldData_len = newData_len;
      
      // move new_temp_data to old_temp_data
      for(int j = 0; j < newData_len; j++){
        oldData[j] = newData[j];
      }
      
      
      // Isend size to i
      MPI_Isend(&send_len, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &request);
      if(VERBOSE){
        printf("My rank: %d, Sending to rank: %d, sent: %d\n", my_rank, i, send_len);
      }
      MPI_Isend(sendDataSetBuffer, send_len, MPI_INT, i, 1, MPI_COMM_WORLD, &request);
    }
  }

  // add old_temp_data into myDataSet;
  int data_len = 0;
  for(int i = 0; i < newData_len; i++){
    myDataSet[i] = oldData[i];
    data_len++;
  }

  for(int i = 0; i < nprocs; i++){
    if(i != my_rank){
      MPI_Recv(&inSize, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
      all_inSize[i] = inSize;
      if(VERBOSE){
        printf("My rank: %d, Receiving from rank: %d, recieved: %d \n", my_rank, i, inSize);
      }
      MPI_Recv(recvDatasetBuffer, inSize, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
      for(int j = 0; j < inSize; j++){
        myDataSet[data_len+j] = recvDatasetBuffer[j];
      }
      data_len += inSize;
    }
  }

  // Timing end
  double tend=MPI_Wtime();
  if(my_rank == 0){
    printf("Time to Distribute (s): %f\n", tend - tstart);  
  }


  if(VERBOSE){
    if(my_rank == 2){
      printf("My rank: %d, My data: ", my_rank);
      for(int i = 0; i < data_len; i++){
        printf(" %d ", myDataSet[i]);
      }
      printf("\n");
    }
  }

  // Timing Sort
  MPI_Barrier(MPI_COMM_WORLD);
  double tSort_start=MPI_Wtime();
  //sort
  qsort(myDataSet, data_len, sizeof(int), compfn);
  double tSort_end=MPI_Wtime();
  //total time
  double totalTime = (tend-tstart) + (tSort_end - tSort_start);
  if(my_rank == 0){
    printf("Time to Sort (s): %f\n", tSort_end - tSort_start);  
    printf("Total Time (s): %f\n\n", totalTime);
  }

  if(VERBOSE){
    if(my_rank == 2){
      printf("My rank: %d, My data: ", my_rank);
      for(int i = 0; i < data_len; i++){
        printf(" %d ", myDataSet[i]);
      }
      printf("\n");
    }
  }

  // Checking 
  localsum = 0;
  for(int i = 0; i < data_len; i++){
    localsum += myDataSet[i];
  }
  globalSum = 0;
  MPI_Reduce(&localsum, &globalSum, 1, MPI_UNSIGNED_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
  if(my_rank == 0){
    printf("Sum: %ld\n\n", globalSum);
  }
  
  //free
  free(newData);
  free(oldData);
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////

  //free
  free(data); 
  free(sendDataSetBuffer); 
  free(recvDatasetBuffer); 
  free(myDataSet);

  MPI_Finalize();
  return 0;
}


double randomExponential(double lambda){
    double u = rand() / (RAND_MAX + 1.0);
    return -log(1- u) / lambda;
}

//generates data [0,1000000)
void generateData(int * data, int SIZE)
{
  for (int i=0; i<SIZE; i++)
  {
    double tmp=0; 
    
    //generate value between 0-1 using exponential distribution
    do{
    tmp=randomExponential(4.0);
    // printf("\nrnd: %f",tmp);
    }while(tmp>=1.0);
    
    data[i]=tmp*MAXVAL;
    
  }

  
}