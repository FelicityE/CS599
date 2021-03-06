#!/bin/bash
#SBATCH --job-name=ring3         
#SBATCH --output=/home/fhe2/CS599/act3.out	
#SBATCH --error=/home/fhe2/CS599/act3.err
#SBATCH --time=02:00				# 2 min
#SBATCH --mem=2000 
#SBATCH --nodes=1
#SBATCH --ntasks=6
#SBATCH --cpus-per-task=1

module load openmpi

mpicc ~/CS599/ring_act3_fhe2.c -lm -o ~/CS599/ring_act3_fhe2

srun ~/CS599/ring_act3_fhe2