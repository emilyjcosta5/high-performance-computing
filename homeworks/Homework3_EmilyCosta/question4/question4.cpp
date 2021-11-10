#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <mutex>
#include <omp.h>
#include <thread>
#include <string.h>
#include <stdio.h>
using namespace std;

#define N 512
#define N_THREADS 4

void run(int matrix[][N], int vector[]){
    int* result = (int*)malloc(N*sizeof(int));
    cout << "Available processors: " << omp_get_num_procs() << endl;
    cout << "Number of processors used by program: " << N_THREADS << endl;
    cout << "Thread IDs: ";
    #pragma omp parallel num_threads(N_THREADS)
    {
        int* dot_product = (int*)malloc(N*sizeof(int));
        //cout << omp_get_thread_num() << ",";
        printf("%d ",omp_get_thread_num());
        for(int i=0; i<N ; i++) {
            #pragma omp for
            for(int j=0; j<N; j++) {
                dot_product[i] += vector[j]*matrix[j][i];   
            }
        }
        #pragma omp critical
        {
            for(int i=0; i<N; i++) result[i] += dot_product[i];
        }
        free(dot_product);
    }
    cout << "\nThe resulting vector: " << endl;
    for(int i=0; i<N; i++)
        cout << result[i] << " ";
    cout << endl;
}

void run_with_reduction(int matrix[][N], int vector[]){
    int* result = (int*)malloc(N*sizeof(int));
    int sum = 0;
    int i,j;
    cout << "Available processors: " << omp_get_num_procs() << endl;
    cout << "Number of processors used by program: " << N_THREADS << endl;
    cout << "Thread IDs: ";
    #pragma omp parallel num_threads(N_THREADS) private(i)
    {
        printf("%d ",omp_get_thread_num());
        for(i=0; i<N ; i++) {
            #pragma omp for reduction(+:result[:N])
            for(j=0; j<N; j++) {
                result[i] += vector[j]*matrix[j][i];
            }
        }
    }
    cout << "\nThe resulting vector: " << endl;
    for(int i=0; i<N; i++)
        cout << result[i] << " ";
    cout << endl;
}

int main(){
    int matrix[N][N];
    for (int x=0; x<N; x++)
		for (int y=0; y<N; y++)
			matrix[x][y] = rand()%20;
    int vector[N];
    for (int x=0; x<N; x++)
        vector[x] = rand()%10;
    run(matrix,vector);
    run_with_reduction(matrix,vector);
}