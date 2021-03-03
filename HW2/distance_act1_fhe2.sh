#!/bin/bash

#SBATCH --job-name=distanceAct11rank
##SBATCH --job-name=distanceAct14rank  
##SBATCH --job-name=distanceAct18rank  
##SBATCH --job-name=distanceAct112rank  
##SBATCH --job-name=distanceAct116rank  
##SBATCH --job-name=distanceAct120rank           


#SBATCH --output=/home/fhe2/CS599/HW2/act1_1rank.out
## SBATCH --output=/home/fhe2/CS599/HW2/act1_4rank.out
## SBATCH --output=/home/fhe2/CS599/HW2/act1_8rank.out
## SBATCH --output=/home/fhe2/CS599/HW2/act1_12rank.out
## SBATCH --output=/home/fhe2/CS599/HW2/act1_16rank.out
## SBATCH --output=/home/fhe2/CS599/HW2/act1_20rank.out	

#SBATCH --error=/home/fhe2/CS599/HW2/act1.err
#SBATCH --time=2:00				
#SBATCH --mem=85000 ## 85G I hope
#SBATCH -C hw
#SBATCH --nodes=1
#SBATCH --account=cs599-spr21

#SBATCH --ntasks=1
##SBATCH --ntasks=4
##SBATCH --ntasks=8
##SBATCH --ntasks=12
##SBATCH --ntasks=16
##SBATCH --ntasks=20

#SBATCH --cpus-per-task=1
#SBATCH --exclusive

module load openmpi

mpicc -O3 distance_act1_fhe2.c -lm -o distance_act1_fhe2

srun distance_act1_fhe2 100000 90 1 bMSD_year_prediction_normalize_0_1_100k.txt