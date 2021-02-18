#!/bin/bash
#SBATCH --job-name=random5         
#SBATCH --output=/home/fhe2/CS599/act5.out	
#SBATCH --error=/home/fhe2/CS599/act5.err
#SBATCH --time=02:00				# 2 min
#SBATCH --mem=2000 
#SBATCH --nodes=1
#SBATCH --ntasks=50
#SBATCH --cpus-per-task=1

module load openmpi

mpicc ~/CS599/random_act5_fhe2.c -lm -o ~/CS599/random_act5_fhe2

srun ~/CS599/random_act5_fhe2