#!/bin/bash
#SBATCH --nodes=1
#SBATCH --time=0:01:00
#SBATCH --job-name=hw3q1
#SBATCH --partition=reservation
#SBATCH --constraint=cascadelake 

module add gcc/10.1.0

gcc -march=native question1.cpp -o q1 
./q1
rm ./q1