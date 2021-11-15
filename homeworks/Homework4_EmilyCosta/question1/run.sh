#!/bin/bash
#SBATCH -N 1
#SBATCH --job-name parallel_pi
#SBATCH --partition=express
#SBATCH --time 0:01:00
#SBATCH --output parallel_pi.out

module purge

module load gcc
module load openmpi

rm -f monte_carlo_pi_mpi
mpiCC question1.cpp -o monte_carlo_pi_mpi
mpirun -np 1 ./monte_carlo_pi_mpi
