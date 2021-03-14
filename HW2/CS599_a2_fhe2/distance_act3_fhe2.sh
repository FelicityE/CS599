#!/bin/bash

##SBATCH --job-name=distance_Act1_1rank
##SBATCH --job-name=distance_act3_4rank  
##SBATCH --job-name=distance_act3_8rank  
#SBATCH --job-name=distance_act3_12rank  
##SBATCH --job-name=distance_act3_16rank  
##SBATCH --job-name=distance_act3_20rank           


##SBATCH --output=/home/fhe2/CS599/HW2/act3_1rank.out
##SBATCH --output=/home/fhe2/CS599/HW2/act3_4rank.out
##SBATCH --output=/home/fhe2/CS599/HW2/act3_8rank.out
#SBATCH --output=/home/fhe2/CS599/HW2/act3_12rank.out
##SBATCH --output=/home/fhe2/CS599/HW2/act3_16rank.out
##SBATCH --output=/home/fhe2/CS599/HW2/act3_20rank.out	

##SBATCH --error=/home/fhe2/CS599/HW2/act3_1rank.err
##SBATCH --error=/home/fhe2/CS599/HW2/act3_4rank.err
##SBATCH --error=/home/fhe2/CS599/HW2/act3_8rank.err
#SBATCH --error=/home/fhe2/CS599/HW2/act3_12rank.err
##SBATCH --error=/home/fhe2/CS599/HW2/act3_16rank.err
##SBATCH --error=/home/fhe2/CS599/HW2/act3_20rank.err

##SBATCH --time=01:30:00				# for 1 rank
##SBATCH --time=16:30				# for 4 ranks
##SBATCH --time=08:00				# for 8 ranks
#SBATCH --time=06:00				# for 12 ranks
##SBATCH --time=05:00				# for 16 ranks
##SBATCH --time=04:30				# for 20 ranks

#SBATCH --mem=85000 ## 85G I hope
#SBATCH -C hw
#SBATCH --nodes=1
#SBATCH --account=cs599-spr21

##SBATCH --ntasks=1
##SBATCH --ntasks=4
##SBATCH --ntasks=8
#SBATCH --ntasks=12
##SBATCH --ntasks=16
##SBATCH --ntasks=20

#SBATCH --cpus-per-task=1
#SBATCH --exclusive

module load openmpi

mpicc -O3 distance_act1_fhe2.c -lm -o distance_act1_fhe2
mpicc -O3 distance_act2_fhe2.c -lm -o distance_act2_fhe2


/usr/bin/perf stat -B -e cache-references,cache-misses srun distance_act1_fhe2 100000 90 1 MSD_year_prediction_normalize_0_1_100k.txt
/usr/bin/perf stat -B -e cache-references,cache-misses srun distance_act2_fhe2 100000 90 100 MSD_year_prediction_normalize_0_1_100k.txt