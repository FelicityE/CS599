#!/bin/bash

#SBATCH --job-name=SortAct1


#SBATCH --output=/home/fhe2/CS599/HW3/act1.out

#SBATCH --error=/home/fhe2/CS599/HW3/act1.err

#SBATCH --time=15:00       ## for all ranks

#SBATCH --mem=50000           ## 50G
#SBATCH -C sl
#SBATCH --nodes=1
#SBATCH --account=cs599-spr21

#SBATCH --cpus-per-task=1
#SBATCH --exclusive

module load openmpi

mpicc -O3 sort_act1_fhe2.c -lm -o sort_act1_fhe2

srun --ntasks=1 sort_act1_fhe2
srun --ntasks=2 sort_act1_fhe2
srun --ntasks=4 sort_act1_fhe2
srun --ntasks=8 sort_act1_fhe2
srun --ntasks=12 sort_act1_fhe2
srun --ntasks=16 sort_act1_fhe2
srun --ntasks=20 sort_act1_fhe2