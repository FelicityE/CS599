#!/bin/bash

#SBATCH --job-name=RangeAct2_4N


#SBATCH --output=/home/fhe2/CS599/HW4/act2_4N.out

#SBATCH --error=/home/fhe2/CS599/HW4/act2_4N.err

#SBATCH --time=3:00       ## for all ranks

#SBATCH --mem=40000           ## 40G
#SBATCH -C sl
#SBATCH --nodes=4
#SBATCH --account=cs599-spr21

#SBATCH --cpus-per-task=1
#SBATCH --exclusive

module load openmpi

# mpicc -O3 range_act2_fhe2.c -lm -o range_act2_fhe2

srun --ntasks-per-node=1 -n1 range_act2_fhe2 2000000 100000
srun --ntasks-per-node=1 -n4 range_act2_fhe2 2000000 100000
srun --ntasks-per-node=2 -n8 range_act2_fhe2 2000000 100000
srun --ntasks-per-node=3 -n12 range_act2_fhe2 2000000 100000
srun --ntasks-per-node=4 -n16 range_act2_fhe2 2000000 100000
srun --ntasks-per-node=5 -n20 range_act2_fhe2 2000000 100000