#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <cmath>
#include <chrono>
using namespace std::chrono;
using namespace std;

#define NUM_DARTS_PER_RANK 10000

int main(int argc, char** argv){
	auto start = high_resolution_clock::now();
	MPI_Init(&argc, &argv);
	int proc_rank, num_procs;
	double global_sum;
	//MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    	MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
	//cout << "Proc: " << proc_rank << endl;
	double local_pi;
	int circle=0,square=0;
	double rand_x,rand_y,origin_dist;
	srand(proc_rank);
	for(int i=0;i<NUM_DARTS_PER_RANK;i++){
		rand_x = double(rand())/double(RAND_MAX);
		rand_y = double(rand())/double(RAND_MAX);
		origin_dist = rand_x*rand_x+rand_y*rand_y;
		if(origin_dist<=1)
			circle++;
		square++;
	}
	local_pi = double(4*circle)/square;
	cout << "Pi at Rank " << proc_rank << "= " << local_pi << endl;
	MPI_Reduce(&local_pi, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	if(proc_rank==0){
		auto stop = high_resolution_clock::now();
        	auto duration = duration_cast<microseconds>(stop - start);
		cout << "pi= " << global_sum/num_procs << endl;
		cout << "approx. accuracy= ~" << (1.-abs(M_PI-(global_sum/num_procs))/M_PI)*100 << endl;
		//cout << "approx. accuracy= ~" << abs(M_PI/(global_sum/num_procs))*100 << endl;
		cout << "number of ranks: " << num_procs << endl;
		cout << "darts thrown per rank: " << NUM_DARTS_PER_RANK << endl;
		cout << "execution time (microseconds): " << duration.count() << endl;
	}
	MPI_Finalize();
	return 0;
}
