#include <iostream>
#include <cmath>
#include <chrono>
#include <mpi.h>
#include <stdlib.h>
using namespace std::chrono;
using namespace std;

#define NUM_CLASSES 100
#define N 1000000

int main(int argc, char** argv){
	int elements[N];
	for(int i=0; i<N; i++)
		elements[i] = (rand()%1000)+1;
	auto start = high_resolution_clock::now();
	MPI_Init(&argc, &argv);
        int proc_rank, num_procs;
        int local_bins[NUM_CLASSES] = {0};
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
        MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
	int block_size = floor(N/num_procs);
	int bin_size = floor(1000/NUM_CLASSES);
	int bin;
	for(int i=(proc_rank)*(block_size); i<((proc_rank+1)*(block_size))-1; i++){
		bin = floor(elements[i]/bin_size);
		local_bins[bin] = local_bins[bin] + 1;
	}
	int global_bins[NUM_CLASSES] = {0};
	for(int i=0; i<NUM_CLASSES; i++)
		MPI_Reduce(&local_bins[i], &global_bins[i], 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	if(proc_rank==0){
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(stop - start);
                cout << "bins= " << endl;
		for (int i=0; i<NUM_CLASSES; i++)
			cout << "bin " << i+1 << ": " << global_bins[i] << endl;
		cout << "number of ranks: " << num_procs << endl;
                cout << "execution time (microseconds): " << duration.count() << endl;
        }
        MPI_Finalize();
        return 0;
}
