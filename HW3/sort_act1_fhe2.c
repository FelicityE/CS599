#include <stdio.h>
#include <unistd.h>
#include <mpi.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


//Example compilation
//mpicc -O3 sort_act1_fhe2.c -lm -o sort_act1_fhe2

//Example execution
//mpirun -np 2 -hostfile myhostfile.txt ./sort_act1_fhe2


void generateData(int * data, int SIZE);
int cmp(const void * a, const void * b);


int compfn (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}


//Do not change the seed
#define SEED 72
#define MAXVAL 10

//Total input size is N, divided by nprocs
//Doesn't matter if N doesn't evenly divide nprocs
#define N 20

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
  unsigned long int localsum = 0;
  for(int i = 0; i < localN; i++){
    localsum += data[i];
  }
  // //Testing ***************************
  // for(int i = 0; i < nprocs; i++){
  //   if(my_rank == i){
  //     printf("My rank: %d, My data: ", my_rank);
  //     for(int j = 0; j < localN; j++){
  //       printf(" %d ", data[j]);
  //     }
  //     printf("\n");
  //   }
  // }


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
  printf("My rank: %d, My bucket: [%d, %d) \n", my_rank, mystart, myend);
  //allocate old_temp_data = size data (localN)
  int * oldData = (int*)malloc(sizeof(int)*localN);
  // add data to old_temp_data
  for(int i = 0; i < localN; i++){
    oldData[i] = data[i];
  }
  //allocate new_temp_data = size old_temp_data
  int * newData = (int*)malloc(sizeof(int)*localN);

  int size_oldData = localN;
  //Testing ***************************
  // if(my_rank == 0){
  //   printf("Local N: %d \n", localN);
  //   //Testing ***************************
  //   printf("My rank: %d, My data: ", my_rank);
  //   for(int j = 0; j < localN; j++){
  //     printf(" %d ", data[j]);
  //   }
  //   printf("\n");
  // }


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
      for(int j = 0; j < size_oldData; j++){
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

      // free old_temp_data
      // printf("My rank: %d going to free oldData for iteration: %d\n", my_rank,i);
      // free(oldData);
      // printf("My rank: %d freed oldData for iteration: %d\n", my_rank,i);
      // alocate old temp_data = size of new_temp_data - number of moved data
      size_oldData = newData_len;
      // int * oldData = (int*)malloc(sizeof(int)*size_oldData);
      // move new_temp_data to old_temp_data

      for(int j = 0; j < newData_len; j++){
        oldData[j] = newData[j];
      }
      
      // // free new_temp_data
      // printf("My rank: %d going to free newData for iteration: %d\n", my_rank,i);
      // free(newData);
      // printf("My rank: %d freed newData for iteration: %d\n", my_rank,i);
      // int * newData = (int*)malloc(sizeof(int)*size_oldData);
          // Isend size to i
      MPI_Isend(&send_len, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &request);
      printf("My rank: %d, Sending to rank: %d, sent: %d\n", my_rank, i, send_len);
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
      printf("My rank: %d, Receiving from rank: %d, recieved: %d \n", my_rank, i, inSize);
      MPI_Recv(recvDatasetBuffer, inSize, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
      for(int j = 0; j < inSize; j++){
        myDataSet[data_len+j] = recvDatasetBuffer[j];
      }
      data_len += inSize;
    }
  }

  if(my_rank == 2){
    printf("My rank: %d, My data: ", my_rank);
    for(int i = 0; i < data_len; i++){
      printf(" %d ", myDataSet[i]);
    }
    printf("\n");
  }


  //sort
  qsort(myDataSet, data_len, sizeof(int), cmp);

  if(my_rank == 2){
    printf("My rank: %d, My data: ", my_rank);
    for(int i = 0; i < data_len; i++){
      printf(" %d ", myDataSet[i]);
    }
    printf("\n");
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////
  //free
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//compare for qsort
int cmp(const void * a, const void * b){
  return ( *(int*)a - *(int*)b );
}