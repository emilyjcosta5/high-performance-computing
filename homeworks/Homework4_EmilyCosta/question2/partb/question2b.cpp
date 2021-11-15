#include <iostream>
#include <cmath>
#include <chrono>
#include <mpi.h>
#include <stdlib.h>
using namespace std::chrono;
using namespace std;

#define N 1000000

int main(int argc, char** argv){
	int elements[N];
	for(int i=0; i<N; i++)
		elements[i] = (rand()%1000)+1;
	auto start = high_resolution_clock::now();
	MPI_Init(&argc, &argv);
        int proc_rank, num_procs;
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
        MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
	int NUM_CLASSES = num_procs;
        int local_bin = 0;
	int bin_size = floor(1000/NUM_CLASSES);
	for(int i=0; i<N; i++){
		if(floor(elements[i]/bin_size)==proc_rank)
			local_bin++;
	}
	cout << "rank " << proc_rank << " local bin: " << local_bin << endl;
	int *global_bins;
	if(proc_rank==0)
		global_bins = (int *)malloc(sizeof(int)*NUM_CLASSES);
	MPI_Gather(&local_bin, 1, MPI_INT, global_bins, 1, MPI_INT, 0, MPI_COMM_WORLD);
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
