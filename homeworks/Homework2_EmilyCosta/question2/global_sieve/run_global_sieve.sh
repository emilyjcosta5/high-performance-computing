g++ -pthread -fopenmp global_sieve.cpp -o sieve
export OMP_NUM_THREADS=1
./sieve
./sieve
./sieve
rm ./sieve
