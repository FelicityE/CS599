#!/bin/bash
#SBATCH --job-name=ping_pong           
#SBATCH --output=/home/fhe2/CS599/act1.out	
#SBATCH --error=/home/fhe2/CS599/act1.err
#SBATCH --time=02:00				# 2 min
#SBATCH --mem=2000 
#SBATCH --nodes=1
#SBATCH --ntasks=10 
#SBATCH --cpus-per-task=1

module load openmpi

mpicc ~/CS599/pingpong_act1_fhe2.c -lm -o ~/CS599/pingpong_act1_fhe2

srun ~/CS599/pingpong_act1_fhe2