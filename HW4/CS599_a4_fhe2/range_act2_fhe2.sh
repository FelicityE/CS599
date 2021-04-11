#!/bin/bash

#SBATCH --job-name=RangeAct2


#SBATCH --output=/home/fhe2/CS599/HW4/act23.out

#SBATCH --error=/home/fhe2/CS599/HW4/act23.err

#SBATCH --time=1:00       ## for all ranks

#SBATCH --mem=40000           ## 40G
#SBATCH -C sl
#SBATCH --nodes=1
#SBATCH --account=cs599-spr21

#SBATCH --cpus-per-task=1
#SBATCH --exclusive

module load openmpi

# mpicc -O3 range_act2_fhe2.c -lm -o range_act2_fhe2

# srun --ntasks=1 range_act2_fhe2 2000000 100000
# srun --ntasks=4 range_act2_fhe2 2000000 100000
# srun --ntasks=8 range_act2_fhe2 2000000 100000
# srun --ntasks=12 range_act2_fhe2 2000000 100000
srun --ntasks=16 range_act2_fhe2 2000000 100000
srun --ntasks=20 range_act2_fhe2 2000000 100000