#include <bits/stdc++.h>
#include <omp.h>
#include <chrono>
#include <iostream>
#include <pthread.h>
using namespace std;
using namespace std::chrono;

struct sieve_args{
   int start;
   int end;
} *args;

void print_array(bool arr[], int size){  
    cout << "Printing array elements:"<< endl;  
    for (int i=0; i<size; i++) cout<<arr[i]<<" ";
    cout << endl;
}  

void print_int_array(int arr[], int size){
    cout << "Printing array elements:"<< endl;
    for (int i=0; i<size; i++) cout<<arr[i]<<" ";
    cout << endl;
}

void *sieve_algorithm(void* args){
	int *in = (int *) args;
	int start = in[0];
	int end = in[1];
	bool is_prime[end-start];
	memset(is_prime, true, sizeof(is_prime));
	int loop_start = 0;
	for(int i=2; i*i<=end; i++){
		loop_start = start;
		while(loop_start%i!=0) loop_start++;
		while(loop_start<=end){
			is_prime[loop_start-start] = false;
			loop_start += i;
		}
	}
	if(start==1 && end-start>2){
		is_prime[1] = 1;
		is_prime[2] = 1;
	}
	//cout << "Array of start,end: " << start << ", " << end << endl;
	//print_array(is_prime, end-start);
	return NULL;
}

void sieve_main(int n, int no_threads){
	int block_size[no_threads] = {0};
	// Establish how the numbers will be scattered for the threads
	if(n%no_threads==0){
		for(int i=0; i<no_threads; i++)
	       		block_size[i] = n/no_threads;
	}
	else{
		for(int i=0; i<n; i++)
			block_size[i%no_threads]++;
	}
	print_int_array(block_size, no_threads);
	int thread_part = 0;
	int start = 0;
	int end = 0;
	pthread_t threads[no_threads];
	int args[2] = {0};
	// Create threads to run the function of the designated partition of data
	while(thread_part<no_threads){
		start = end + 1;
		end = start + block_size[thread_part++] - 1;
		args[0] = start;
		args[1] = end;
		pthread_create(&threads[thread_part-1], NULL, sieve_algorithm, (void*)args);
	}
	// Destroy the thread!!!
	for (int i=0; i<no_threads; i++)
                pthread_join(threads[i], NULL);
}

void sieve_main_omp(int n){
	omp_set_dynamic(0);     // Explicitly disable dynamic teams
	//omp_set_num_threads(4);
	//
	#pragma omp parallel 
	{
	int thread_num = omp_get_thread_num();
	int num_threads = omp_get_num_threads();
	int args[2] = {};
	args[0] = n*thread_num/num_threads;
	args[1] = n*(thread_num+1)/num_threads;
	sieve_algorithm(args);
	}	
}

int main(){
	auto start = high_resolution_clock::now();
	auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end-start);

	// pthreads
	cout << "pthreads\n----------------" << endl;

	// Weak scaling
	cout << "WEAK SCALING\n----------------" << endl;
	int n = 1000000;
	
	start = high_resolution_clock::now();
	sieve_main(n,1);
	end = high_resolution_clock::now();
	duration = duration_cast<microseconds>(end-start);
	cout << "Time taken by 1 thread: "
         << duration.count() << " microseconds" << endl;

	start = high_resolution_clock::now();
	sieve_main(n,2);
	end = high_resolution_clock::now();
	duration = duration_cast<microseconds>(end-start);
	cout << "Time taken by 2 threads: "
         << duration.count() << " microseconds" << endl;
	
	start = high_resolution_clock::now();
	sieve_main(n,3);
	end = high_resolution_clock::now();
	duration = duration_cast<microseconds>(end-start);
	cout << "Time taken by 3 threads: "
         << duration.count() << " microseconds" << endl;
	
	start = high_resolution_clock::now();
	sieve_main(n,4);
	end = high_resolution_clock::now();
	duration = duration_cast<microseconds>(end-start);
	cout << "Time taken by 4 threads: "
         << duration.count() << " microseconds" << endl;
	
	start = high_resolution_clock::now();
	sieve_main(n,5);
	end = high_resolution_clock::now();
	duration = duration_cast<microseconds>(end-start);
	cout << "Time taken by 5 threads: "
         << duration.count() << " microseconds" << endl;
	
	start = high_resolution_clock::now();
	sieve_main(n,6);
	end = high_resolution_clock::now();
	duration = duration_cast<microseconds>(end-start);
	cout << "Time taken by 6 threads: "
         << duration.count() << " microseconds" << endl;
	
	start = high_resolution_clock::now();
	sieve_main(n,7);
	end = high_resolution_clock::now();
	duration = duration_cast<microseconds>(end-start);
	cout << "Time taken by 7 threads: "
         << duration.count() << " microseconds" << endl;
	
	start = high_resolution_clock::now();
	sieve_main(n,8);
	end = high_resolution_clock::now();
	duration = duration_cast<microseconds>(end-start);
	cout << "Time taken by 8 threads: "
         << duration.count() << " microseconds" << endl;


	// Strong scaling
	cout << endl;
	cout << "STRONG SCALING\n----------------" << endl;

        start = high_resolution_clock::now();
        sieve_main(n,1);
        end = high_resolution_clock::now();
	duration = duration_cast<microseconds>(end-start);
        cout << "Time taken by 1 thread: "
         << duration.count() << " microseconds" << endl;

        start = high_resolution_clock::now();
        sieve_main(n*2,2);
        end = high_resolution_clock::now();
        duration = duration_cast<microseconds>(end-start);
        cout << "Time taken by 2 threads: "
         << duration.count() << " microseconds" << endl;

        start = high_resolution_clock::now();
        sieve_main(n*3,3);
        end = high_resolution_clock::now();
        duration = duration_cast<microseconds>(end-start);
        cout << "Time taken by 3 threads: "
         << duration.count() << " microseconds" << endl;

        start = high_resolution_clock::now();
        sieve_main(n*4,4);
        end = high_resolution_clock::now();
        duration = duration_cast<microseconds>(end-start);
        cout << "Time taken by 4 threads: "
         << duration.count() << " microseconds" << endl;

        start = high_resolution_clock::now();
        sieve_main(n*5,5);
        end = high_resolution_clock::now();
        duration = duration_cast<microseconds>(end-start);
        cout << "Time taken by 5 threads: "
         << duration.count() << " microseconds" << endl;

        start = high_resolution_clock::now();
        sieve_main(n*6,6);
        end = high_resolution_clock::now();
        duration = duration_cast<microseconds>(end-start);
        cout << "Time taken by 6 threads: "
         << duration.count() << " microseconds" << endl;

        start = high_resolution_clock::now();
        sieve_main(n*7,7);
        end = high_resolution_clock::now();
        duration = duration_cast<microseconds>(end-start);
        cout << "Time taken by 7 threads: "
	 << duration.count() << " microseconds" << endl;

        start = high_resolution_clock::now();
        sieve_main(n*8,8);
        end = high_resolution_clock::now();
        duration = duration_cast<microseconds>(end-start);
        cout << "Time taken by 8 threads: "
         << duration.count() << " microseconds" << endl;
	
	cout << endl << "-------------------" << endl;
	// openMP
        cout << "OMP\n----------------" << endl;
	//int n=1000000;
	
	// Weak scaling
        cout << "WEAK SCALING\n----------------" << endl;

        omp_set_num_threads(1);
        start = high_resolution_clock::now();
        sieve_main_omp(n);
        end = high_resolution_clock::now();
        duration = duration_cast<microseconds>(end-start);
        cout << "Time taken by 1 thread: "
         << duration.count() << " microseconds" << endl;

        int t = 2;
        omp_set_num_threads(t);
        start = high_resolution_clock::now();
        sieve_main_omp(n);
        end = high_resolution_clock::now();
        duration = duration_cast<microseconds>(end-start);
        cout << "Time taken by " << t <<" threads: "
         << duration.count() << " microseconds" << endl;

        t = 3;
        omp_set_num_threads(t);
        start = high_resolution_clock::now();
        sieve_main_omp(n);
        end = high_resolution_clock::now();
        duration = duration_cast<microseconds>(end-start);
        cout << "Time taken by " << t <<" threads: "
         << duration.count() << " microseconds" << endl;

        t = 4;
        omp_set_num_threads(t);
        start = high_resolution_clock::now();
        sieve_main_omp(n);
        end = high_resolution_clock::now();
        duration = duration_cast<microseconds>(end-start);
        cout << "Time taken by " << t <<" threads: "
         << duration.count() << " microseconds" << endl;

        t = 5;
        omp_set_num_threads(t);
        start = high_resolution_clock::now();
        sieve_main_omp(n);
        end = high_resolution_clock::now();
        duration = duration_cast<microseconds>(end-start);
        cout << "Time taken by " << t <<" threads: "
         << duration.count() << " microseconds" << endl;

        t = 6;
        omp_set_num_threads(t);
        start = high_resolution_clock::now();
        sieve_main_omp(n);
        end = high_resolution_clock::now();
        duration = duration_cast<microseconds>(end-start);
	
	t = 7;
        omp_set_num_threads(t);
        start = high_resolution_clock::now();
        sieve_main_omp(n);
        end = high_resolution_clock::now();
        duration = duration_cast<microseconds>(end-start);
        cout << "Time taken by " << t <<" threads: "
         << duration.count() << " microseconds" << endl;

        t = 8;
        omp_set_num_threads(t);
        start = high_resolution_clock::now();
        sieve_main_omp(n);
        end = high_resolution_clock::now();
        duration = duration_cast<microseconds>(end-start);
        cout << "Time taken by " << t <<" threads: "
         << duration.count() << " microseconds" << endl;

        // Strong scaling
        cout << "STRONG SCALING\n----------------" << endl;
	
	omp_set_num_threads(1);
        start = high_resolution_clock::now();
        sieve_main_omp(n);
        end = high_resolution_clock::now();
	duration = duration_cast<microseconds>(end-start);
        cout << "Time taken by 1 thread: "
         << duration.count() << " microseconds" << endl;	
	
	t = 2;
	omp_set_num_threads(t);
        start = high_resolution_clock::now();
        sieve_main_omp(n*t);
        end = high_resolution_clock::now();
        duration = duration_cast<microseconds>(end-start);
        cout << "Time taken by " << t <<" threads: "
         << duration.count() << " microseconds" << endl;
	
	t = 3;
        omp_set_num_threads(t);
        start = high_resolution_clock::now();
        sieve_main_omp(n*t);
        end = high_resolution_clock::now();
        duration = duration_cast<microseconds>(end-start);
        cout << "Time taken by " << t <<" threads: "
         << duration.count() << " microseconds" << endl;
	
	t = 4;
        omp_set_num_threads(t);
        start = high_resolution_clock::now();
        sieve_main_omp(n*t);
        end = high_resolution_clock::now();
        duration = duration_cast<microseconds>(end-start);
        cout << "Time taken by " << t <<" threads: "
         << duration.count() << " microseconds" << endl;

	t = 5;
        omp_set_num_threads(t);
        start = high_resolution_clock::now();
        sieve_main_omp(n*t);
        end = high_resolution_clock::now();
        duration = duration_cast<microseconds>(end-start);
        cout << "Time taken by " << t <<" threads: "
         << duration.count() << " microseconds" << endl;

	t = 6;
        omp_set_num_threads(t);
        start = high_resolution_clock::now();
        sieve_main_omp(n*t);
        end = high_resolution_clock::now();
        duration = duration_cast<microseconds>(end-start);
        cout << "Time taken by " << t <<" threads: "
         << duration.count() << " microseconds" << endl;

	t = 7;
        omp_set_num_threads(t);
        start = high_resolution_clock::now();
        sieve_main_omp(n*t);
        end = high_resolution_clock::now();
        duration = duration_cast<microseconds>(end-start);
        cout << "Time taken by " << t <<" threads: "
         << duration.count() << " microseconds" << endl;

	t = 8;
        omp_set_num_threads(t);
        start = high_resolution_clock::now();
        sieve_main_omp(n*t);
        end = high_resolution_clock::now();
        duration = duration_cast<microseconds>(end-start);
        cout << "Time taken by " << t <<" threads: "
         << duration.count() << " microseconds" << endl;
	return 1;
}


