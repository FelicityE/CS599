#!/bin/bash

#SBATCH --job-name=Kmeann20


#SBATCH --output=/home/fhe2/CS599/HW5/kmeann20.out

#SBATCH --error=/home/fhe2/CS599/HW5/kMeann20.err

#SBATCH --time=3:00       ## for all ranks

#SBATCH --mem=20000          ## 20G
#SBATCH -C sl
#SBATCH --nodes=1
#SBATCH --account=cs599-spr21

#SBATCH --cpus-per-task=1
#SBATCH --exclusive

module load openmpi

mpicc -O3 kmeans_act1_fhe2.c -lm -o kmeans_act1_fhe2

# srun --ntasks=1 kmeans_act1_fhe2 5159737 2 2 iono_57min_5.16Mpts_2D.txt
# srun --ntasks=1 kmeans_act1_fhe2 5159737 2 25 iono_57min_5.16Mpts_2D.txt
# srun --ntasks=1 kmeans_act1_fhe2 5159737 2 50 iono_57min_5.16Mpts_2D.txt
# srun --ntasks=1 kmeans_act1_fhe2 5159737 2 100 iono_57min_5.16Mpts_2D.txt

# srun --ntasks=4 kmeans_act1_fhe2 5159737 2 2 iono_57min_5.16Mpts_2D.txt
# srun --ntasks=4 kmeans_act1_fhe2 5159737 2 25 iono_57min_5.16Mpts_2D.txt
# srun --ntasks=4 kmeans_act1_fhe2 5159737 2 50 iono_57min_5.16Mpts_2D.txt
# srun --ntasks=4 kmeans_act1_fhe2 5159737 2 100 iono_57min_5.16Mpts_2D.txt

# srun --ntasks=8 kmeans_act1_fhe2 5159737 2 2 iono_57min_5.16Mpts_2D.txt
# srun --ntasks=8 kmeans_act1_fhe2 5159737 2 25 iono_57min_5.16Mpts_2D.txt
# srun --ntasks=8 kmeans_act1_fhe2 5159737 2 50 iono_57min_5.16Mpts_2D.txt
# srun --ntasks=8 kmeans_act1_fhe2 5159737 2 100 iono_57min_5.16Mpts_2D.txt

# srun --ntasks=12 kmeans_act1_fhe2 5159737 2 2 iono_57min_5.16Mpts_2D.txt
# srun --ntasks=12 kmeans_act1_fhe2 5159737 2 25 iono_57min_5.16Mpts_2D.txt
# srun --ntasks=12 kmeans_act1_fhe2 5159737 2 50 iono_57min_5.16Mpts_2D.txt
# srun --ntasks=12 kmeans_act1_fhe2 5159737 2 100 iono_57min_5.16Mpts_2D.txt

# srun --ntasks=16 kmeans_act1_fhe2 5159737 2 2 iono_57min_5.16Mpts_2D.txt
# srun --ntasks=16 kmeans_act1_fhe2 5159737 2 25 iono_57min_5.16Mpts_2D.txt
# srun --ntasks=16 kmeans_act1_fhe2 5159737 2 50 iono_57min_5.16Mpts_2D.txt
# srun --ntasks=16 kmeans_act1_fhe2 5159737 2 100 iono_57min_5.16Mpts_2D.txt

srun --ntasks=20 kmeans_act1_fhe2 5159737 2 2 iono_57min_5.16Mpts_2D.txt
srun --ntasks=20 kmeans_act1_fhe2 5159737 2 25 iono_57min_5.16Mpts_2D.txt
srun --ntasks=20 kmeans_act1_fhe2 5159737 2 50 iono_57min_5.16Mpts_2D.txt
srun --ntasks=20 kmeans_act1_fhe2 5159737 2 100 iono_57min_5.16Mpts_2D.txt