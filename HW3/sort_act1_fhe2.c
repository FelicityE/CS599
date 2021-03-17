#include <stdio.h>
#include <unistd.h>
#include <mpi.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//Example compilation
//mpicc -O3 sort_act1_fhe2.c -lm -o sort_act1_fhe2

//Example execution
//mpirun -np 3 -hostfile myhostfile.txt ./sort_act1_fhe2

void generateData(int * data, int SIZE);


int compfn (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}

#define VERBOSE 0

//Do not change the seed
#define SEED 72
#define MAXVAL 1000000
// #define MAXVAL 1000

//Total input size is N, divided by nprocs
//Doesn't matter if N doesn't evenly divide nprocs
#define N 1000000000
// #define N 1000000

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
  if(my_rank == 0){
    printf("Act 1: Uniform, Number of Ranks: %d\n\n",nprocs);
  }
  // Checking sum before
  unsigned long int localsum = 0;
  for(int i = 0; i < localN; i++){
    localsum += data[i];
  }
  unsigned long int globalSum = 0;
  MPI_Reduce(&localsum, &globalSum, 1, MPI_UNSIGNED_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
  if(my_rank == 0){
    printf("Sum Before: %ld\n\n", globalSum);
  }
  
  unsigned long int localsum_cnt = 0;
  unsigned long int globalSum_cnt = 0;
  if(VERBOSE){
    localsum_cnt =localN;
    MPI_Reduce(&localsum_cnt, &globalSum_cnt, 1, MPI_UNSIGNED_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    if(my_rank == 0){
      printf("N: %ld\n\n", globalSum_cnt);
    }
  }

  // Timing
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
  
  // Testing ****************************************************************
  int total_sendLen = 0;
  int * send_lenPR = (int*)malloc(sizeof(int)*nprocs);
  int ** sendDataSetBuffer_PR=(int**)malloc(sizeof(int*)*nprocs); // perRank
  if(VERBOSE){
    if(my_rank == 0){
      printf("Allocating sendDataSetBuffer before for loop\n");
    }  
  }

  for(int i = 0; i < nprocs; i++){
    send_lenPR[i] = 0;
    sendDataSetBuffer_PR[i] =(int*)malloc(sizeof(int)*localN);
  }
  
  if(VERBOSE){
    if(my_rank == 0){
      printf("Allocating sendDataSetBuffer first for loop complete \n");
    }  
  }
  // ****************************************************************


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
          total_sendLen += send_len;
        }else{
          //new_temp_data adds data not sent
          newData[newData_len] = oldData[j];
          // update new_temp_data_pos
          newData_len++;
        }
      }

      oldData_len = newData_len;
      
      // move new_temp_data to old_temp_data
      for(int j = 0; j < oldData_len; j++){
        oldData[j] = newData[j];
      }

      send_lenPR[i] = send_len;
      for(int j = 0; j < send_len; j++){
        sendDataSetBuffer_PR[i][j] = sendDataSetBuffer[j];
      }
    }
  }

  // add old_temp_data into myDataSet;
  int data_len = 0;
  for(int i = 0; i < newData_len; i++){
    myDataSet[i] = oldData[i];
    data_len++;
  }

  for (int i = 0; i < nprocs; i++){
    if(i != my_rank){
      MPI_Isend(&send_lenPR[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, &request);
      if(VERBOSE){
        printf("My rank: %d, Sending to rank: %d, sent: %d\n", my_rank, i, send_lenPR[i]);
      }
      MPI_Isend(sendDataSetBuffer_PR[i], send_lenPR[i], MPI_INT, i, 1, MPI_COMM_WORLD, &request);
    }
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
  
  if(nprocs == 1){
    data_len = localN;
    for(int i = 0; i < data_len; i++){
      myDataSet[i] = data[i];
    }   
  }

  if(VERBOSE){
    localsum_cnt = 0;
    localsum_cnt =data_len;
    globalSum_cnt = 0;
    MPI_Reduce(&localsum_cnt, &globalSum_cnt, 1, MPI_UNSIGNED_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    if(my_rank == 0){
      printf("N: %ld\n\n", globalSum_cnt);
    }
  }

  // Timing reduction
  double tend=MPI_Wtime();
  double globalMax_distTime;
  double dist_time = tend - tstart;
  MPI_Reduce(&dist_time, &globalMax_distTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
  if(my_rank == 0){
    printf("Time to Distribute (s): %f\n", globalMax_distTime);  
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
  double tSort_start=MPI_Wtime();
  //sort
  qsort(myDataSet, data_len, sizeof(int), compfn);
  double tSort_end=MPI_Wtime();
  
  double globalMax_sortTime;
  double sort_time = tSort_end - tSort_start;
  MPI_Reduce(&sort_time, &globalMax_sortTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
  //total time
  double globalMax_totalTime;
  double totalTime = dist_time+sort_time;
  MPI_Reduce(&totalTime, &globalMax_totalTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
  if(my_rank == 0){
    printf("Time to Sort (s): %f\n", globalMax_sortTime);  
    printf("Total Time (s): %f\n\n", globalMax_totalTime);
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

  if(VERBOSE){
    localsum_cnt =data_len;
    globalSum_cnt = 0;
    MPI_Reduce(&localsum_cnt, &globalSum_cnt, 1, MPI_UNSIGNED_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    if(my_rank == 0){
      printf("N: %ld\n\n", globalSum_cnt);
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
    printf("Sum After: %ld\n\n", globalSum);
  }
  
  //free
  free(newData);
  free(oldData);
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////
  free(data); 
  free(sendDataSetBuffer); 
  free(recvDatasetBuffer); 
  free(myDataSet);

  MPI_Finalize();
  return 0;
}


//generates data [0,MAXVAL)
void generateData(int * data, int SIZE)
{
  for (int i=0; i<SIZE; i++)
  {
  
  data[i]=rand()%MAXVAL;
  }
}