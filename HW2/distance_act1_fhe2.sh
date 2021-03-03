#!/bin/bash

#SBATCH --job-name=distance_act1_1rank
##SBATCH --job-name=distance_act1_4rank  
##SBATCH --job-name=distance_act1_8rank  
##SBATCH --job-name=distance_act1_12rank  
##SBATCH --job-name=distance_act1_16rank  
##SBATCH --job-name=distance_act1_20rank           


#SBATCH --output=/home/fhe2/CS599/act1_1rank.out
## SBATCH --output=/home/fhe2/CS599/act1_4rank.out
## SBATCH --output=/home/fhe2/CS599/act1_8rank.out
## SBATCH --output=/home/fhe2/CS599/act1_12rank.out
## SBATCH --output=/home/fhe2/CS599/act1_16rank.out
## SBATCH --output=/home/fhe2/CS599/act1_20rank.out	

#SBATCH --error=/home/fhe2/CS599/act1.err
#SBATCH --time=01:00				
#SBATCH --mem=2000 
#SBATCH -C hw
#SBATCH --nodes=1

#SBATCH --ntasks=1
##SBATCH --ntasks=4
##SBATCH --ntasks=8
##SBATCH --ntasks=12
##SBATCH --ntasks=16
##SBATCH --ntasks=20


#SBATCH --cpus-per-task=1
#SBATCH --exclusive

module load openmpi

mpicc -03 ~/CS599/distance_act1_fhe2.c -lm -o ~/CS599/distance_act1_fhe2

srun ~/CS599/distance_act1_fhe2 100000 90 1 ~/CS599/MSD_year_prediction_normalize_0_1_100k.txt