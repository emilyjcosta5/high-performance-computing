#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <mutex>          // std::mutex
#include <bits/stdc++.h>
#include <chrono>
#include <pthread.h>
#include <omp.h>

using namespace std;
using namespace std::chrono;
std::mutex mtx;

#define ARRAY_SIZE 2*1000000
#define N_THREADS 2
bool is_prime[ARRAY_SIZE];
//memset(is_prime, true, sizeof(is_prime));

void print_array(bool arr[], int size){
    cout << "Printing array elements:"<< endl;
    for (int i=0; i<size; i++) cout<<arr[i]<<" ";
    cout << endl;
}

void *thread_work(void* p){
        int prime = (intptr_t) p;
	//if(is_prime[prime-1]!=true)
	//	return NULL;
	mtx.lock();
        for(int i=prime*prime; i<=ARRAY_SIZE; i+=prime){
                //mtx.lock();
		is_prime[i-1]=false;
		//mtx.unlock();
	}
        mtx.unlock();
	return NULL;
}

void sieve_pthreads(){ 
        pthread_t threads[N_THREADS];
	int thread_count=0;
	memset(is_prime, true, sizeof(is_prime));
        for(int i=2; i*i<=ARRAY_SIZE; i++){
		if(is_prime[i-1]==true)
			pthread_create(&threads[thread_count++], NULL, thread_work, (void*)(intptr_t)i);
		if(thread_count%N_THREADS==0)
			thread_count=0;	
        }
	for (int i=0; i<N_THREADS; i++)
                pthread_join(threads[i], NULL);
}

void sieve_omp(){
        memset(is_prime, true, sizeof(is_prime));
	int size = ARRAY_SIZE;
	int k = size/2;
	#pragma omp parallel for shared(is_prime)
	for(int i=2; i<k; i++){
		if(is_prime[i-1]==true){
			for(int j=i*i; j<=size; j+=i)
				is_prime[i-1]=false;
		}
		k = size/(i+1);
        }
}

int main(){
        auto start = high_resolution_clock::now();
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end-start);
	
	omp_set_num_threads(N_THREADS);
        start = high_resolution_clock::now();
        //sieve_pthreads();
	sieve_omp();
        end = high_resolution_clock::now();
        duration = duration_cast<microseconds>(end-start);
	cout << "Time taken: "
         << duration.count() << " microseconds" << endl;

	//print_array(is_prime, ARRAY_SIZE);
}

