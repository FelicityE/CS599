#!/bin/bash

#SBATCH --job-name=distanceAct2B       


#SBATCH --output=/home/bwd29/Fel/CS599/HW2/act2_b.out

#SBATCH --error=/home/bwd29/Fel/CS599/HW2/act2_b.err
#SBATCH --time=05:00				
#SBATCH --mem=85000 #
#SBATCH -C hw
#SBATCH --nodes=1

#SBATCH --ntasks=20


#SBATCH --cpus-per-task=1
#SBATCH --exclusive

module load openmpi

mpicc -O3 distance_act2_fhe2.c -lm -o distance_act2_fhe2

srun ./distance_act2_fhe2 100000 90 5 MSD_year_prediction_normalize_0_1_100k.txt
srun ./distance_act2_fhe2 100000 90 100 MSD_year_prediction_normalize_0_1_100k.txt
srun ./distance_act2_fhe2 100000 90 500 MSD_year_prediction_normalize_0_1_100k.txt
srun ./distance_act2_fhe2 100000 90 1000 MSD_year_prediction_normalize_0_1_100k.txt
srun ./distance_act2_fhe2 100000 90 2000 MSD_year_prediction_normalize_0_1_100k.txt
srun ./distance_act2_fhe2 100000 90 3000 MSD_year_prediction_normalize_0_1_100k.txt
srun ./distance_act2_fhe2 100000 90 4000 MSD_year_prediction_normalize_0_1_100k.txt
srun ./distance_act2_fhe2 100000 90 5000 MSD_year_prediction_normalize_0_1_100k.txt