#!/bin/bash
#SBATCH --job-name=random4         
#SBATCH --output=/home/fhe2/CS599/act4.out	
#SBATCH --error=/home/fhe2/CS599/act4.err
#SBATCH --time=00:30				# 2 min
#SBATCH --mem=1000 
#SBATCH --nodes=1
#SBATCH --ntasks=50         # could not run on monsoon due to high number of tasks, increasing number of nodes made the take too long.
#SBATCH --cpus-per-task=1

module load openmpi

mpicc ~/CS599/random_act4_fhe2.c -lm -o ~/CS599/random_act4_fhe2

srun ~/CS599/random_act4_fhe2