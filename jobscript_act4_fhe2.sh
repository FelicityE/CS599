#!/bin/bash
#SBATCH --job-name=random4         
#SBATCH --output=/home/fhe2/CS599/act4.out	
#SBATCH --error=/home/fhe2/CS599/act4.err
#SBATCH --time=02:00				# 2 min
#SBATCH --mem=2000 
#SBATCH --nodes=2
#SBATCH --ntasks=50
#SBATCH --cpus-per-task=1

module load openmpi

mpicc ~/CS599/random_act4_fhe2.c -lm -o ~/CS599/random_act4_fhe2

srun ~/CS599/random_act4_fhe2