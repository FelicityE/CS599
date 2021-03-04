#!/bin/bash

#SBATCH --job-name=distanceAct21rank
##SBATCH --job-name=distanceAct24rank  
##SBATCH --job-name=distanceAct28rank  
##SBATCH --job-name=distanceAct212rank  
##SBATCH --job-name=distanceAct216rank  
##SBATCH --job-name=distanceAct220rank           


#SBATCH --output=/home/fhe2/CS599/HW2/act2_1rank.out
##SBATCH --output=/home/fhe2/CS599/HW2/act2_4rank.out
##SBATCH --output=/home/fhe2/CS599/HW2/act2_8rank.out
##SBATCH --output=/home/fhe2/CS599/HW2/act2_12rank.out
##SBATCH --output=/home/fhe2/CS599/HW2/act2_16rank.out
##SBATCH --output=/home/fhe2/CS599/HW2/act2_20rank.out	

#SBATCH --error=/home/fhe2/CS599/HW2/act2_ranks.err

#SBATCH --time=01:30:00				# for 1 rank
##SBATCH --time=16:30				# for 4 ranks
##SBATCH --time=08:00				# for 8 ranks
##SBATCH --time=06:00				# for 12 ranks
##SBATCH --time=05:00				# for 16 ranks
##SBATCH --time=04:30				# for 20 ranks

#SBATCH --mem=85000
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

mpicc -O3 distance_act2_fhe2.c -lm -o distance_act2_fhe2

srun distance_act2_fhe2 100000 90 100 MSD_year_prediction_normalize_0_1_100k.txt
srun distance_act2_fhe2 100000 90 100 MSD_year_prediction_normalize_0_1_100k.txt
srun distance_act2_fhe2 100000 90 100 MSD_year_prediction_normalize_0_1_100k.txt