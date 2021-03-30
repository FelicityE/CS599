#!/bin/bash

#SBATCH --job-name=HW4Test2


#SBATCH --output=/home/fhe2/CS599/HW4/Tact2.out

#SBATCH --error=/home/fhe2/CS599/HW4/Tact2.err

#SBATCH --time=2:00     

#SBATCH --mem=40000           ## 40G
#SBATCH -C sl
#SBATCH --nodes=1
#SBATCH --account=cs599-spr21

#SBATCH --cpus-per-task=1
#SBATCH --exclusive

module load openmpi

# mpic++ -O3 range_act2_fhe2.cpp -lm -o range_act2_fhe2

srun --ntasks=20 range_act2_fhe2 2000000 100000
# srun --ntasks-per-node=10 -n20 range_act2_fhe2 2000000 100000