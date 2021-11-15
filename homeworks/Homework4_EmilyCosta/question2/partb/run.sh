#!/bin/bash
#SBATCH -N 4
#SBATCH --job-name q2b
#SBATCH --partition=express
#SBATCH --time 0:00:10
#SBATCH --output q2b.out

module purge

module load gcc
module load openmpi

rm -f q2b
mpiCC question2b.cpp -o q2b
mpirun -np 4 ./q2b
