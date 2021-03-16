#!/bin/bash

#SBATCH --job-name=SortAct1


#SBATCH --output=/home/fhe2/CS599/HW3/act1.out

#SBATCH --error=/home/fhe2/CS599/HW3/act1.err

#SBATCH --time=01:30:00       ## for all ranks

#SBATCH --mem=50000           ## 50G
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

srun --ntasks=1 distance_act1_fhe2 100000 90 1 MSD_year_prediction_normalize_0_1_100k.txt
srun distance_act1_fhe2 100000 90 1 MSD_year_prediction_normalize_0_1_100k.txt
srun distance_act1_fhe2 100000 90 1 MSD_year_prediction_normalize_0_1_100k.txt