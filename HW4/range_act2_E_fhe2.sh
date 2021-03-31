#!/bin/bash

#SBATCH --job-name=RangeAct2_E


#SBATCH --output=/home/fhe2/CS599/HW4/act2_E.out

#SBATCH --error=/home/fhe2/CS599/HW4/act2_E.err

#SBATCH --time=3:00       ## for all ranks

#SBATCH --mem=40000           ## 40G
#SBATCH -C sl
#SBATCH --nodes=1
#SBATCH --account=cs599-spr21

#SBATCH --cpus-per-task=1
#SBATCH --exclusive

module load openmpi

# mpicc -O3 range_act2_fhe2.c -lm -o range_act2_fhe2


srun --ntasks-per-socket=1 -n1 range_act2_fhe2 2000000 100000
srun --ntasks-per-socket=2 -n4 range_act2_fhe2 2000000 100000
srun --ntasks-per-socket=4 -n8 range_act2_fhe2 2000000 100000
srun --ntasks-per-socket=6 -n12 range_act2_fhe2 2000000 100000
srun --ntasks-per-socket=8 -n16 range_act2_fhe2 2000000 100000
srun --ntasks-per-socket=10 -n20 range_act2_fhe2 2000000 100000
# srun --ntasks-per-node=1 --ntasks-per-socket=1 -n1 range_act2_fhe2 2000000 100000
# srun --ntasks-per-node=1 --ntasks-per-socket=1 -n4 range_act2_fhe2 2000000 100000
# srun --ntasks-per-node=1 --ntasks-per-socket=1 -n8 range_act2_fhe2 2000000 100000
# srun --ntasks-per-node=6 --ntasks-per-socket=1 -n12 range_act2_fhe2 2000000 100000
# srun --ntasks-per-node=8 --ntasks-per-socket=1 -n16 range_act2_fhe2 2000000 100000
# srun --ntasks-per-node=10 --ntasks-per-socket=1 -n20 range_act2_fhe2 2000000 100000