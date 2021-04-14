//kmeans_starter.c

#include <stdio.h>
#include <unistd.h>
#include <mpi.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>



#define KMEANSITERS 10

#define VERBOSE 0

//compile
//mpicc kmeans_act1_fhe2.c -lm -o kmeans

//run example with 2 means
//mpirun -np 4 -hostfile myhostfile.txt ./kmeans 5159737 2 2 iono_57min_5.16Mpts_2D.txt

//function prototypes
double distCalc(int dim, double * point1, double * point2);
int importDataset(char * fname, int DIM, int N, double ** dataset);

int main(int argc, char **argv) {

  int my_rank, nprocs;

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  MPI_Comm_size(MPI_COMM_WORLD,&nprocs);


  //Process command-line arguments
  int N;
  int DIM;
  int KMEANS;
  char inputFname[500];


  if (argc != 5) {
    fprintf(stderr,"Please provide the following on the command line: N (number of lines in the file), dimensionality (number of coordinates per point/feature vector), K (number of means), dataset filename. Your input: %s\n",argv[0]);
    MPI_Finalize();
    exit(0);
  }

  sscanf(argv[1],"%d",&N);
  sscanf(argv[2],"%d",&DIM);
  sscanf(argv[3],"%d",&KMEANS);
  strcpy(inputFname,argv[4]);
  
  //pointer to entire dataset
  double ** dataset;

  

  if (N<1 || DIM <1 || KMEANS < 1)
  {
    printf("\nOne of the following are invalid: N, DIM, K(MEANS)\n");
    MPI_Finalize();
    exit(0);
  }
  //All ranks import dataset
  else
  {
   
    if (my_rank==0)
    {
    printf("\nNumber of lines (N): %d, Dimensionality: %d, KMEANS: %d, Filename: %s\n", N, DIM, KMEANS, inputFname);
    }



    //allocate memory for dataset
    dataset=(double**)malloc(sizeof(double*)*N);
    for (int i=0; i<N; i++)
    {
      dataset[i]=(double*)malloc(sizeof(double)*DIM);
    }

    int ret=importDataset(inputFname, DIM, N, dataset);

    if (ret==1)
    {
      MPI_Finalize();
      return 0;
    }    
  }

  MPI_Barrier(MPI_COMM_WORLD);

  
//###########################################################################
  //Write code here
  // Initialize variables used
    // Timing total time
  double tStart = MPI_Wtime();
  double tDist = 0;
  double tCent = 0;
  int localN = N/nprocs;
  int myStart = my_rank * localN;
  if(my_rank == nprocs-1){
    localN += N%nprocs;
  }
  int ksN = KMEANS*DIM;
  double * ks;
  double * secondToLastKS;
  int * clusterID;
  int * clusterSize;

  //allocate memory for arrays used
  ks = (double*)malloc(sizeof(double)*ksN);
  secondToLastKS = (double*)malloc(sizeof(double)*ksN);
  clusterID = (int*)calloc(sizeof(int),localN);
  clusterSize = (int*)malloc(sizeof(int)*KMEANS);

  

  //init starting k means
  if(VERBOSE){
    if(my_rank == 0){
      printf("Dataset: ");
    }
  }
  for(int i = 0; i < KMEANS; i++){
    for(int j = 0; j < DIM; j++){
      ks[i*DIM+j] = dataset[i][j];
      if(VERBOSE){
        if(my_rank == 0){
          printf("%f, ", dataset[i][j]);
        }
      }
    }
  }
  if(VERBOSE){
    if(my_rank == 0){
      printf("\n");
      printf("k Means Start: ");
      for(int i = 0; i < ksN; i++){
        printf("%f, ", ks[i]);
      }
      printf("\n");
    }
  }
  
  //big loop
  for(int i = 0; i < KMEANSITERS; i++){
    //Timing updating Centriod
    double tCentStart = MPI_Wtime();
    // setting up clusterSize to be reused;
    for(int j = 0; j < KMEANS; j++){
      clusterSize[j] = 0;
    }
    //Timing updating Centriod
    double tCentEnd = MPI_Wtime();
    tCent += tCentEnd-tCentStart;

    // Timing Dist calc
    double tDistStart=MPI_Wtime();
    //finding closest k
    for(int j = 0; j < localN; j++){
      double dist = 0;
      double lowest_dist = 0;
      int closeK = 0;
      for(int k = 0; k < KMEANS; k++){
        dist = distCalc(DIM, dataset[j+myStart], &ks[k*DIM]);
        if(k == 0){
          lowest_dist = dist;
          closeK = k;
        }else if(dist < lowest_dist){
          lowest_dist = dist;
          closeK = k;
        }
      }
      clusterID[j] = closeK;
    }
    //Timing Dist calc
    double tDistEnd=MPI_Wtime();
    tDist += tDistEnd-tDistStart;
    if(VERBOSE){
      if(my_rank == 0){
        printf("\n");
        printf("cluster ID in finding closest K: ");
        for(int i = 0; i < localN; i++){
          printf("%d, ", clusterID[i]);
        }
        printf("\n");
      }
    }


    //Timing updating Centriod
    tCentStart = MPI_Wtime();
    //getting sizes of clusters
    for(int j = 0; j<localN; j++){
      clusterSize[clusterID[j]]++;
    }
    int * globalClusterSize;
    globalClusterSize = (int*)malloc(sizeof(int)*KMEANS);
    // MPI_Barrier(MPI_COMM_WORLD);
    MPI_Allreduce(clusterSize, globalClusterSize, KMEANS, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    if(VERBOSE){
      if(my_rank == 0){
        printf("\n");
        printf("local cluster size: ");
        for(int i = 0; i < KMEANS; i++){
          printf("%d, ", clusterSize[i]);
        }
        printf("\n");
      }
    }

    if(i == KMEANSITERS-1){
      for(int j =0; j < ksN; j++){
        secondToLastKS[j] = ks[j];
      }
    }

    // updating clusterSize; setting up ks to be reused
    for(int j = 0; j < KMEANS; j++){
      clusterSize[j] = globalClusterSize[j];
      for(int k = 0; k < DIM; k++){
        ks[j*DIM+k] = 0;
      }
    }
    if(VERBOSE){
      if(my_rank == 0){
        printf("\n");
        printf("Global cluster size: ");
        for(int i = 0; i < KMEANS; i++){
          printf("%d, ", clusterSize[i]);
        }
        printf("\n");
      }
    }
    if(VERBOSE){
      if(my_rank == 0){
        printf("\n");
        printf("k Means after Reset: ");
        for(int i = 0; i < ksN; i++){
          printf("%f, ", ks[i]);
        }
        printf("\n");
      }
    }

    //summing all local N
    for(int j = 0; j < localN; j++){
      for(int k = 0; k < DIM; k++){
        ks[clusterID[j]*DIM+k] += dataset[j+myStart][k];
      }
    }
    if(VERBOSE){
      if(my_rank == 0){
        printf("\n");
        printf("k Means after local sum: ");
        for(int i = 0; i < ksN; i++){
          printf("%f, ", ks[i]);
        }
        printf("\n");
      }
    }

    //finding weighted mean
    for(int j = 0; j < KMEANS; j++){     
      for(int k = 0; k < DIM; k++){
        if(clusterSize[j] > 0){
          ks[j*DIM+k] = ks[j*DIM+k]/clusterSize[j];
        }else{
          ks[j*DIM+k] = 0;
        }
      }
    }
    if(VERBOSE){
      if(my_rank == 0){
        printf("\n");
        printf("k Means after weighted: ");
        for(int i = 0; i < ksN; i++){
          printf("%f, ", ks[i]);
        }
        printf("\n");
      }
    }

    //getting total mean
    double * globalKS;
    globalKS = (double*)malloc(sizeof(double)*ksN);
    // MPI_Barrier(MPI_COMM_WORLD);
    MPI_Allreduce(ks, globalKS, ksN, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    if(VERBOSE){
      if(my_rank == 0){
        printf("\n");
        printf("Global k Means sum: ");
        for(int i = 0; i < ksN; i++){
          printf("%f, ", globalKS[i]);
        }
        printf("\n");
      }
    }


    // updating ks
    // MPI_Barrier(MPI_COMM_WORLD);
    for(int j = 0; j < ksN; j++){
      ks[j] = globalKS[j];
    }
    if(VERBOSE){
      if(my_rank == 0){
        printf("\n");
        printf("k Means after update to global : ");
        for(int i = 0; i < ksN; i++){
          printf("%f ", globalKS[i]);
        }
        printf("\n");
      }
    }
    free(globalClusterSize);
    free(globalKS);

    //Timing updating Centriod
    tCentEnd = MPI_Wtime();
    tCent += tCentEnd-tCentStart;
  }
  // Timing total time
  double tEnd = MPI_Wtime();
  double tTime = tEnd-tStart;




  // printing final results
  if(VERBOSE){
    if(my_rank == 0){
      printf("\n");
      printf("Final Global Cluster Size: ");
      for(int i = 0; i < KMEANS; i++){
        printf("%d ", clusterSize[i]);
      }
      printf("\n");
    }
  }

  // if(VERBOSE){
    if(my_rank == 0){
      printf("\n");
      printf("Final (Second to Last) Global k Means : \n");
      for(int i = 0; i < KMEANS; i++){
        for(int j = 0; j < DIM; j++){
          if(j%2 == 0){
            printf("%f ", secondToLastKS[i*DIM+j]);
          }else{
            printf("%f \n", secondToLastKS[i*DIM+j]);
          }
        }

      }
      printf("\n");
    }
  // }

  // Getting global max times
  double maxTotalTime;
  double maxDistTime;
  double maxCentTime;
  MPI_Reduce(&tTime, &maxTotalTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
  MPI_Reduce(&tDist, &maxDistTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
  MPI_Reduce(&tCent, &maxCentTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
  if(my_rank == 0){
    printf("Max Total Time: %f \nMax Dist Calc Time: %f \nMax Update Centriod Time: %f \n\n", maxTotalTime,maxDistTime,maxCentTime);  
  }


  free(ks);
  free(secondToLastKS);
  free(clusterID);
  free(clusterSize);


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



int importDataset(char * fname, int DIM, int N, double ** dataset)
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