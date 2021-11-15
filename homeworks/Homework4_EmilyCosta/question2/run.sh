#!/bin/bash
#SBATCH -N 2
#SBATCH --job-name q2a
#SBATCH --partition=express
#SBATCH --time 0:01:00
#SBATCH --output q2a.out

module purge

module load gcc
module load openmpi

rm -f q2a
mpiCC question2a.cpp -o q2a
mpirun -np 2 ./q2a

rm -f q2b
mpiCC question2b.cpp -o q2b
mpirun -np 2 ./q2b
