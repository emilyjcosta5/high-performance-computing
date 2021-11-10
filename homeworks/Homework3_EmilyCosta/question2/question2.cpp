#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <semaphore.h>
#include <cmath>
#include <chrono>
using namespace std::chrono;
using namespace std;

#define N_THREADS 2
#define N 10000
pthread_t threads[N_THREADS];
sem_t mutex;
bool is_div[N] = {0};
int block_size = floor((N/N_THREADS));
int COUNT = 0;
int thread_no = 1;

void* thread_work(void* arg){
    //int* n_pos = (int *)thread_no;
    //int n = *n_pos + 1;
    sem_wait(&mutex);
    int n = thread_no++;
    sem_post(&mutex);
    int start = (n-1)*block_size;
    int end = (n*block_size)-1;
    //cout << "End: " << end << endl;
    int val = 0;
    if(n==N_THREADS) end = N-1;
    for(int i=start;i<end;i++){
        if((i+1)%3==0 || (i+1)%7==0){
            sem_wait(&mutex);
            is_div[i] = 1;
            COUNT++;
            sem_post(&mutex);
        }
    }
    return NULL;
}

int main(){
    auto t1 = high_resolution_clock::now();
    sem_init(&mutex, 0, 1);
    for (int i = 0; i < N_THREADS; i++){
        pthread_create(&threads[i], NULL, thread_work, (void *)NULL);
    }
    for (int i = 0; i<N_THREADS; i++)
        pthread_join(threads[i], NULL);
    sem_destroy(&mutex);
    auto t2 = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(t2 - t1);
    // print the numbers
    cout << "Numbers divisible by 3 or 7: " << endl;
    for (int i=0; i<N; i++){
        if(is_div[i]==1){
            cout << i+1 << " ";
            if((i+1)%3!=0 && (i+1)%7!=0){
                cout << "\nERROR: " << i+1 << " is incorrectly classified." << endl;
                return 0;
            }
        }
    }
    cout << "\nCount of numbers divisible by 3 or 7: " << COUNT << endl;
    cout << "Microseconds taken to complete: " << duration.count() << endl;
    return 0;
}