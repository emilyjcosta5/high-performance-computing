#include<cmath>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

#define NUM_BINS 10
#define GRID_SIZE 32 /* should be a multiple of 32 */
#define N 1000

__global__ void kernel_histogram(unsigned char* numbers, long size, unsigned int* histogram, int num_bins){
    int tid = blockIdx.x*blockDim.x+threadIdx.x;
    if(tid>=size) return;
    unsigned char number = numbers[tid];
    int bin = number % num_bins;
    atomicAdd(&histogram[bin], 1);
}


