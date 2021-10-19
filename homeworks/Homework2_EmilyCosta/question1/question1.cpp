#include <pthread.h>
//#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <mutex>
using namespace std;

#define N 5
pthread_t philosophers[N];
pthread_mutex_t forks[N];
int philosopher_no[N];
int is_eating[N] = {0};
std::mutex mtx;
int dinner_time = 1;

void *eat(void * philosopher){
	int* ptr = (int*)philosopher;
	int philosopher_number = *ptr;
	bool last = false;
	while(dinner_time){
		pthread_mutex_lock(&forks[philosopher_number]);
		// corner case: last philosopher needs to take first and last forks
		last = philosopher_number==N;
		if(last)
			pthread_mutex_lock(&forks[0]);
		else
			pthread_mutex_lock(&forks[philosopher_number+1]);
		// print the current table
		// only only one mutex to print at a time so it's not mangled
		mtx.lock();
		//cout << "Philosopher #" << philosopher_number+1 << " started eating." << endl;
		is_eating[philosopher_number] = 1;
		cout << "Philosophers currently eating: ";
		for(int i=0; i<N; i++){
			if(is_eating[i]==1)
				cout << i+1 << " ";
		}
		/*cout << endl;
		cout << "Philosophers not eating: ";
		for(int i=0; i<N; i++){
        	        if(is_eating[i]==0)
                	        cout << i+1 << " ";
	        }*/
		cout << endl;
		mtx.unlock();
		// eat for a random amount of time; each run should vary as we use time
		// as the seed for rand
		srand(time(0));
		sleep(rand()%10);
		//sleep(2);
		// put down fork when done eating
		pthread_mutex_unlock(&forks[philosopher_number]);
		if(last)
        	        pthread_mutex_unlock(&forks[0]);
	        else
        	        pthread_mutex_unlock(&forks[philosopher_number+1]);
		is_eating[philosopher_number] = 0;
		//mtx.lock();
		//cout << "Philosopher #" << philosopher_number+1 << " stopped eating." << endl;
		//mtx.unlock();
	}
	return NULL;
}

void dijkstra(){
	// Populate the philosopher number array
	for(int i=0;i<N;i++)
		philosopher_no[i] = i;
	// create the mutexes
	for(int i=0;i<N;i++)
		pthread_mutex_init(&forks[i], NULL);
	// create threads
	for(int i=0;i<N;i++)
		pthread_create(&philosophers[i], NULL, eat, &philosopher_no[i]);
	sleep(30);
	dinner_time = 0;
	// destroy threads & mutexes	
	for(int i=0;i<N;i++)
		pthread_join(philosophers[i], NULL);
	for(int i=0;i<N;i++)
		pthread_mutex_destroy(&forks[i]);
}

int main(){
	dijkstra();
	return 0;	
}
