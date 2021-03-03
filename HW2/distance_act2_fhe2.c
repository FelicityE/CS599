#include <stdio.h>
#include <unistd.h>
#include <mpi.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>



//Example compilation
//mpicc -O3 distance_act2_fhe2.c -lm -o distance_act2_fhe2

//Example execution
//mpirun -np 3 -hostfile myhostfile.txt ./distance_act2_fhe2 10 90 100 MSD_year_prediction_normalize_0_1_100k.txt

//Test run
// mpirun -np 4 -hostfile myhostfile.txt ./distance_act2_fhe2 1000 90 3 MSD_year_prediction_normalize_0_1_100k.txt

//function prototypes
int importDataset(char * fname, int N, double ** dataset);
double distCalc(int dim, double * point1, double * point2);

int main(int argc, char **argv) {

  int my_rank, nprocs;

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  MPI_Comm_size(MPI_COMM_WORLD,&nprocs);


  //Process command-line arguments
  int N;
  int DIM;
  int blocksize;
  char inputFname[500];


  if (argc != 5) {
    fprintf(stderr,"Please provide the following on the command line: N (number of lines in the file), dimensionality (number of coordinates per point), block size, dataset filename.\n");
    MPI_Finalize();
    exit(0);
  }

  sscanf(argv[1],"%d",&N);
  sscanf(argv[2],"%d",&DIM);
  sscanf(argv[3],"%d",&blocksize);
  strcpy(inputFname,argv[4]);
  
  //pointer to dataset
  double ** dataset;

  
  if (N<1 || DIM <1)
  {
    printf("\nN is invalid or DIM is invalid\n");
    MPI_Finalize();
    exit(0);
  }
  //All ranks import dataset
  else
  {
   
    if (my_rank==0)
    {
    printf("\nNumber of lines (N): %d, Dimensionality: %d, Block size: %d, Filename: %s\n", N, DIM, blocksize, inputFname);
    }

    //allocate memory for dataset
    dataset=(double**)malloc(sizeof(double*)*N);
    for (int i=0; i<N; i++)
    {
      dataset[i]=(double*)malloc(sizeof(double)*DIM);
    }

    int ret=importDataset(inputFname, N, dataset);

    if (ret==1)
    {
      MPI_Finalize();
      return 0;
    }

  }

  //###########################################################################
  //Write code here
  int nrow = 0;
  // int ncol = N;
  int my_start = 0;
  double localsum = 0;

  int ntiles = 0;
  
  int trow = 0;
  int tcol = 0;

  // tiles per row
  int tpr = ceil(N*1.0/blocksize);

  // Assign N into Ranks
  nrow = N/nprocs;
  my_start = my_rank*nrow;
  if(my_rank == nprocs-1){
    nrow += N%nprocs;
  }

  ntiles = ceil(nrow*1.0/blocksize) * tpr;


  for(int i = 0; i < ntiles; i++){
  }
  
  // // Allocate memory for distance matrix
  double ** distmat;
  distmat=(double**)malloc(sizeof(double*)*nrow);
  for (int i=0; i<nrow; i++){
    distmat[i]=(double*)malloc(sizeof(double)*N);
  }


  MPI_Barrier(MPI_COMM_WORLD);
  double tstart=MPI_Wtime();
  int count = 0;
  /*for each block*/
  for(int i = 0; i < ntiles; i++){
    trow = blocksize;
    if(nrow%blocksize != 0){
      if (i/tpr == nrow/blocksize){
        trow = nrow%blocksize;
      }
    }
    tcol = blocksize;
    if(N%blocksize != 0){
      int check_last_col = 0;
      check_last_col = (i+1)%(int)ceil(N*1.0/blocksize);
      if (check_last_col == 0){
        tcol = N%blocksize;
      }
    }
    /*for each row*/
    for(int j=my_start+i/tpr*blocksize; j < my_start+i/tpr*blocksize+trow; j++){
      /*for each col*/
      for(int k = i%tpr*blocksize; k < i%tpr*blocksize+tcol; k++){
        distmat[j-my_start][k] = distCalc(DIM, dataset[j], dataset[k]);
        localsum += distmat[j-my_start][k];
      }
    } 
  }
  double tend=MPI_Wtime();
  if(my_rank == 0){
    printf("Time (s): %f\n", tend - tstart);  
  }

  double globalSum = 0;
  MPI_Reduce(&localsum, &globalSum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  if(my_rank == 0){
    printf("Sum: %f\n\n", globalSum);
  }

  //###########################################################################
      

  //free dataset
  for (int i=0; i<N; i++)
  {
    free(dataset[i]);
  }

  free(dataset);

  MPI_Finalize();

  return 0;
}

//###########################################################################
double distCalc(int dim, double * point1, double * point2){
  // dist = sqrt(sum((x_a - x_b)^2))
  // dist = sqrt((x1-x2)^2+(y1-y2)^2+(z1-z2)^2)
  double sum_count = 0;
  double dist = 0;

  for(int i = 0; i < dim; i++){
    sum_count += pow((point1[i]-point2[i]),2);
  }
  dist = sqrt(sum_count);

  return dist;
}
//###########################################################################

int importDataset(char * fname, int N, double ** dataset)
{

    FILE *fp = fopen(fname, "r");

    if (!fp) {
        printf("Unable to open file\n");
        return(1);
    }

    char buf[4096];
    int rowCnt = 0;
    int colCnt = 0;
    while (fgets(buf, 4096, fp) && rowCnt<N) {
        colCnt = 0;

        char *field = strtok(buf, ",");
        double tmp;
        sscanf(field,"%lf",&tmp);
        dataset[rowCnt][colCnt]=tmp;

        
        while (field) {
          colCnt++;
          field = strtok(NULL, ",");
          
          if (field!=NULL)
          {
          double tmp;
          sscanf(field,"%lf",&tmp);
          dataset[rowCnt][colCnt]=tmp;
          }   

        }
        rowCnt++;
    }

    fclose(fp);

    return 0;


}

