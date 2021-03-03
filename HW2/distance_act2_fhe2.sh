#!/bin/bash

#SBATCH --job-name=distance_act2_b       


#SBATCH --output=/home/fhe2/CS599/act2_b.out

#SBATCH --error=/home/fhe2/CS599/act2_b.err
#SBATCH --time=01:00				
#SBATCH --mem=2000 
#SBATCH -C hw
#SBATCH --nodes=1

#SBATCH --ntasks=20


#SBATCH --cpus-per-task=1
#SBATCH --exclusive

module load openmpi

mpicc -03 ~/CS599/distance_act2_fhe2.c -lm -o ~/CS599/distance_act2_fhe2

srun ~/CS599/distance_act2_fhe2 100000 90 5 ~/CS599/MSD_year_prediction_normalize_0_1_100k.txt
srun ~/CS599/distance_act2_fhe2 100000 90 100 ~/CS599/MSD_year_prediction_normalize_0_1_100k.txt
srun ~/CS599/distance_act2_fhe2 100000 90 500 ~/CS599/MSD_year_prediction_normalize_0_1_100k.txt
srun ~/CS599/distance_act2_fhe2 100000 90 1000 ~/CS599/MSD_year_prediction_normalize_0_1_100k.txt
srun ~/CS599/distance_act2_fhe2 100000 90 2000 ~/CS599/MSD_year_prediction_normalize_0_1_100k.txt
srun ~/CS599/distance_act2_fhe2 100000 90 3000 ~/CS599/MSD_year_prediction_normalize_0_1_100k.txt
srun ~/CS599/distance_act2_fhe2 100000 90 4000 ~/CS599/MSD_year_prediction_normalize_0_1_100k.txt
srun ~/CS599/distance_act2_fhe2 100000 90 5000 ~/CS599/MSD_year_prediction_normalize_0_1_100k.txt