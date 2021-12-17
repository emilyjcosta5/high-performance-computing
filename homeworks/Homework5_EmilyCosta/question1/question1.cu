#include <cmath>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <cuda.h>

#define BIN_COUNT 128
#define BLOCK_SIZE 512
#define N 33554432
#define RANGE 1000000
#define PRINT_RESULT 0
#define PRINT_RUNTIME 1

__global__ void kernel(const int *numbers, int *histogram, const int *bin_count){
     /*__shared__ unsigned int local[NUM_THREADS];
     local[threadIdx.x] = 0;
     __syncthreads();
     int offset = blockDim.x * gridDim.x; // to move to next block
     //int bin_size = (int)(RANGE/NUM_THREADS);
     int bin = 0;
     for(int i=threadIdx.x+blockIdx.x*blockDim.x; i<size; i+=offset){
         bin = (int)(numbers[i]/bin_size);     
         atomicAdd(&local[bin], 1);
     }
     __syncthreads();
    // since we inferred each thread has one bin
    atomicAdd(&(histogram[threadIdx.x]), local[threadIdx.x]);*/
    int i = threadIdx.x + blockDim.x * blockIdx.x;
    int num = numbers[i];
    int bin = num % *bin_count;
    atomicAdd(&(histogram[bin]), 1);
}

int* histogram_gpu(int* numbers){
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    int num_blocks = N/BLOCK_SIZE;
    if(N%BLOCK_SIZE!=0) num_blocks++;
    // allocate memory for histogram
    int* dev_histogram;
    cudaError_t cudaStatus;
    cudaStatus = cudaMalloc((void**)&dev_histogram, (BIN_COUNT+1)*sizeof(int));
    if(cudaStatus != cudaSuccess)
        printf("dev_histogram not allocated.\n");
    // allocate memory for numbers
    int* dev_numbers;
    cudaStatus = cudaMalloc((void**)&dev_numbers, (N+1)*sizeof(int));
    if(cudaStatus != cudaSuccess)
        printf("dev_numbers not allocated.\n");
    // set histogram values into the device
    cudaStatus = cudaMemset(dev_histogram, 0, (BIN_COUNT+1) * sizeof(int));
    if(cudaStatus != cudaSuccess)
        printf("dev_histogram not memset.\n");
    // copy numbers into the device
    cudaStatus = cudaMemcpy(dev_numbers, numbers, (N+1)*sizeof(int), cudaMemcpyHostToDevice);
    if(cudaStatus != cudaSuccess)
        printf("dev_numbers not copied.\n");
    int * host_numbers = new int[N+1];
    cudaMemcpy(host_numbers, dev_numbers, (N+1)*sizeof(int), cudaMemcpyDeviceToHost);
    if(host_numbers[0]!=numbers[0]) printf("Numbers not copied correctly.\n");
    int* dev_bin_count;
    cudaStatus = cudaMalloc((void**)&dev_bin_count, sizeof(int));
    if(cudaStatus != cudaSuccess)
        printf("dev_bin_count not allocated.\n");
    int bin_count = BIN_COUNT;
    cudaStatus = cudaMemcpy(dev_bin_count, &bin_count, sizeof(int), cudaMemcpyHostToDevice);
    if(cudaStatus != cudaSuccess)
        printf("dev_bin_count not copied.\n");
    // finally, run on gpu
    cudaEventRecord(start);
    kernel <<< num_blocks , BLOCK_SIZE >>> (dev_numbers, dev_histogram, dev_bin_count);
    cudaEventRecord(stop);
    // copy result back to host
    int* histogram = new int[BLOCK_SIZE+1];
    cudaStatus = cudaMemcpy(histogram, dev_histogram, (BIN_COUNT+1)*sizeof(int), cudaMemcpyDeviceToHost);
    if(cudaStatus != cudaSuccess)
        printf("histogram not returned to host.\n");
    cudaEventSynchronize(stop);
    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, stop);
    if(PRINT_RUNTIME==1) printf("Total gpu runtime (milliseconds): %f\n", milliseconds);
    cudaFree(dev_histogram); cudaFree(dev_numbers);
    return histogram;
}

int main(int argc, char* argv[]){
    int numbers[N];
    for(int i=0; i<N; i++) numbers[i]=(rand()%RANGE)+1;
    int* histogram;
    histogram = histogram_gpu(numbers);
    if(PRINT_RESULT==1)
        for (int i=0; i<BIN_COUNT; i++)
            printf("%d : %d\n", i+1, histogram[i]); 
    delete [] histogram;
    return 0;
}
