#include <cmath>
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <cuda.h>

#define N 64
#define RANGE 10
#define PRINT_RESULT 0 /* Set 1 to print resulting array */
#define PRINT_RUNTIME 1 /* Set 1 to print runtime */

__global__ void tiled(float * a, float * b, int n){
    int i = threadIdx.x + blockIdx.x * blockDim.x;
    int j = threadIdx.y + blockIdx.y * blockDim.y;
    int k = threadIdx.z + blockIdx.z * blockDim.z;
    float result;
    result = 0.8*(b[(i-1)+n*j+n*n*k]+b[(i+1)+n*j+n*n*k]+b[i+n*(j-1)+n*n*k]+b[i+n*(j+1)+n*n*k]+b[i+n*j+n*n*(k-1)]+b[i+n*j+n*n*(k+1)]);
    a[i+n*j+n*n*k]=(float)result;
}

__global__ void tiled_shared_memory(float * a, float * b, int n){
    __shared__ float tmp[8*8*8];
    int i = threadIdx.x + blockIdx.x * blockDim.x;
    int j = threadIdx.y + blockIdx.y * blockDim.y;
    int k = threadIdx.z + blockIdx.z * blockDim.z;
    int block_location = threadIdx.x+threadIdx.y+threadIdx.z;
    float result;
    result = 0.8*(b[(i-1)+n*j+n*n*k]+b[(i+1)+n*j+n*n*k]+b[i+n*(j-1)+n*n*k]+b[i+n*(j+1)+n*n*k]+b[i+n*j+n*n*(k-1)]+b[i+n*j+n*n*(k+1)]);
    tmp[block_location] = (float)result;
    __syncthreads();
    a[i+n*j+n*n*k] = tmp[block_location];
}

__global__ void nontiled(float * a, float * b, int n){
    int i = threadIdx.x + blockIdx.x * blockDim.x;
    float result;
    for(int j=1; j<n-1; j++){
        for(int k=1; k<n-1; k++){
            result = 0.8*(b[(i-1)+n*j+n*n*k]+b[(i+1)+n*j+n*n*k]+b[i+n*(j-1)+n*n*k]+b[i+n*(j+1)+n*n*k]+b[i+n*j+n*n*(k-1)]+b[i+n*j+n*n*(k+1)]);
            a[i+n*j+n*n*k]=(float)result;
        }
    }
}

float* q2_tiled(float * b){
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    float* d_a;
    float* d_b;
    int* d_n;
    cudaError_t cudaStatus;
    // set up a on device
    cudaStatus = cudaMalloc(&d_a, (N*N*N)*sizeof(float));
    if(cudaStatus != cudaSuccess)
        printf("d_a not allocated.\n");
    cudaStatus = cudaMemset(d_a, 0, (N*N*N)*sizeof(float));
    if(cudaStatus != cudaSuccess)
        printf("d_a not memset.\n");
    // set up b on device
    cudaStatus = cudaMalloc(&d_b, (N*N*N)*sizeof(float));
    if(cudaStatus != cudaSuccess)
        printf("d_b not allocated.\n");
    cudaStatus = cudaMemcpy(d_b, b, (N*N*N)*sizeof(float), cudaMemcpyHostToDevice);
    if(cudaStatus != cudaSuccess)
        printf("d_b not copied.\n");
    // set up n on device
    cudaStatus = cudaMalloc((void**)&d_n, sizeof(int));
    if(cudaStatus != cudaSuccess)
        printf("d_n not allocated.\n");
    int n = N;
    cudaStatus = cudaMemcpy(d_n, &n, sizeof(int), cudaMemcpyHostToDevice);
    if(cudaStatus != cudaSuccess)
        printf("d_n not copied.\n");
    // finally, run on gpu
    int d = (int)sqrt(N);
    dim3 grid(d,d,d);
    dim3 block(d,d,d);
    cudaEventRecord(start);
    tiled <<< grid, block >>> (d_a, d_b, N);
    cudaEventRecord(stop);
    // copy result back to host
    float * a = new float[N*N*N+1];
    cudaStatus = cudaMemcpy(a, d_a, (N*N*N)*sizeof(float), cudaMemcpyDeviceToHost);
    cudaEventSynchronize(stop);
    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, stop);
    if(PRINT_RUNTIME==1) printf("Tiled with shared memory took %f milliseconds.\n",milliseconds);
    if(cudaStatus != cudaSuccess)
        printf("a not returned to host.\n");
    cudaFree(d_a); cudaFree(d_b);
    return a;
}

float* q2_tiled_shared_memory(float * b){
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    float* d_a;
    float* d_b;
    int* d_n;
    cudaError_t cudaStatus;
    // set up a on device
    cudaStatus = cudaMalloc(&d_a, (N*N*N)*sizeof(float));
    if(cudaStatus != cudaSuccess)
        printf("d_a not allocated.\n");
    cudaStatus = cudaMemset(d_a, 0, (N*N*N)*sizeof(float));
    if(cudaStatus != cudaSuccess)
        printf("d_a not memset.\n");
    // set up b on device
    cudaStatus = cudaMalloc(&d_b, (N*N*N)*sizeof(float));
    if(cudaStatus != cudaSuccess)
        printf("d_b not allocated.\n");
    cudaStatus = cudaMemcpy(d_b, b, (N*N*N)*sizeof(float), cudaMemcpyHostToDevice);
    if(cudaStatus != cudaSuccess)
        printf("d_b not copied.\n");
    // set up n on device
    cudaStatus = cudaMalloc((void**)&d_n, sizeof(int));
    if(cudaStatus != cudaSuccess)
        printf("d_n not allocated.\n");
    int n = N;
    cudaStatus = cudaMemcpy(d_n, &n, sizeof(int), cudaMemcpyHostToDevice);
    if(cudaStatus != cudaSuccess)
        printf("d_n not copied.\n");
    // finally, run on gpu
    int d = (int)sqrt(N);
    dim3 grid(d,d,d);
    dim3 block(d,d,d);
    cudaEventRecord(start);
    tiled_shared_memory <<< grid, block >>> (d_a, d_b, N);
    cudaEventRecord(stop);
    // copy result back to host
    float * a = new float[N*N*N+1];
    cudaStatus = cudaMemcpy(a, d_a, (N*N*N)*sizeof(float), cudaMemcpyDeviceToHost);
    cudaEventSynchronize(stop);
    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, stop);
    if(PRINT_RUNTIME==1) printf("Tiled took %f milliseconds.\n",milliseconds);
    if(cudaStatus != cudaSuccess)
        printf("a not returned to host.\n");
    cudaFree(d_a); cudaFree(d_b);
    return a;
}

float* q2_nontiled(float * b){
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    float* d_a;
    float* d_b;
    int* d_n;
    cudaError_t cudaStatus;
    // set up a on device
    cudaStatus = cudaMalloc(&d_a, (N*N*N)*sizeof(float));
    if(cudaStatus != cudaSuccess)
        printf("d_a not allocated.\n");
    cudaStatus = cudaMemset(d_a, 0, (N*N*N)*sizeof(float));
    if(cudaStatus != cudaSuccess)
        printf("d_a not memset.\n");
    // set up b on device
    cudaStatus = cudaMalloc(&d_b, (N*N*N)*sizeof(float));
    if(cudaStatus != cudaSuccess)
        printf("d_b not allocated.\n");
    cudaStatus = cudaMemcpy(d_b, b, (N*N*N)*sizeof(float), cudaMemcpyHostToDevice);
    if(cudaStatus != cudaSuccess)
        printf("d_b not copied.\n");
    // set up n on device
    cudaStatus = cudaMalloc((void**)&d_n, sizeof(int));
    if(cudaStatus != cudaSuccess)
        printf("d_n not allocated.\n");
    int n = N;
    cudaStatus = cudaMemcpy(d_n, &n, sizeof(int), cudaMemcpyHostToDevice);
    if(cudaStatus != cudaSuccess)
        printf("d_n not copied.\n");
    // finally, run on gpu
    cudaEventRecord(start);
    tiled <<< N*N, N >>> (d_a, d_b, N);
    cudaEventRecord(stop);
    // copy result back to host
    float * a = new float[N*N*N+1];
    cudaStatus = cudaMemcpy(a, d_a, (N*N*N)*sizeof(float), cudaMemcpyDeviceToHost);
    if(cudaStatus != cudaSuccess)
        printf("a not returned to host.\n");
    cudaEventSynchronize(stop);
    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, stop);
    if(PRINT_RUNTIME==1) printf("Nontiled took %f milliseconds.\n",milliseconds);
    cudaFree(d_a); cudaFree(d_b);
    return a;
}

int main(int argc, char* argv[]){
    float * b;
    b = (float *)malloc(N*N*N*sizeof(float));
    for(int i=0; i<N; i++) 
        for(int j=0; j<N; j++)
            for(int k=0; k<N; k++)
                b[i+j+k]= static_cast <float> (rand())/(static_cast <float> (RAND_MAX/RANGE));
    float * a;
    a = q2_nontiled(b);
    if(PRINT_RESULT==1)
        for(int i=0; i<N; i++)
            for(int j=0; j<N; j++)
                for(int k=0; k<N; k++)
                    if(a[i+j+k]>0.) printf("[%d,%d,%d] : %f\n", i+1, j+1, k+1, a[i+j+k]); 
    free(b);
    free(a);
    b = (float *)malloc(N*N*N*sizeof(float));
    for(int i=0; i<N; i++)
        for(int j=0; j<N; j++)
            for(int k=0; k<N; k++)
                b[i+j+k]= static_cast <float> (rand())/(static_cast <float> (RAND_MAX/RANGE));
    a = q2_tiled_shared_memory(b);
    if(PRINT_RESULT==1)
        for(int i=0; i<N; i++)
            for(int j=0; j<N; j++)
                for(int k=0; k<N; k++)
                    if(a[i+j+k]>0.) printf("[%d,%d,%d] : %f\n", i+1, j+1, k+1, a[i+j+k]);
    free(b);
    free(a); 
    b = (float *)malloc(N*N*N*sizeof(float));
    for(int i=0; i<N; i++)
        for(int j=0; j<N; j++)
            for(int k=0; k<N; k++)
                b[i+j+k]= static_cast <float> (rand())/(static_cast <float> (RAND_MAX/RANGE));
    a = q2_tiled(b);
    if(PRINT_RESULT==1)
        for(int i=0; i<N; i++)
            for(int j=0; j<N; j++)
                for(int k=0; k<N; k++)
                    if(a[i+j+k]>0.) printf("[%d,%d,%d] : %f\n", i+1, j+1, k+1, a[i+j+k]);
    free(b);
    free(a);
    return 0;
}
