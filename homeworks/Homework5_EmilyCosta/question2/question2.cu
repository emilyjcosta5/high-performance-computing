#include <cmath>
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <cuda.h>

#define N 64
#define RANGE 10

__global__ void kernel(float * a, float * b, int n){
    int i = threadIdx.x + blockIdx.x * blockDim.x;
    int j = threadIdx.y + blockIdx.y * blockDim.y;
    int k = threadIdx.z + blockIdx.z * blockDim.z;
    float result;
    result = 0.8*(b[(i-1)+n*j+n*n*k]+b[(i+1)+n*j+n*n*k]+b[i+n*(j-1)+n*n*k]+b[i+n*(j+1)+n*n*k]+b[i+n*j+n*n*(k-1)]+b[i+n*j+n*n*(k+1)]);
    a[i+n*j+n*n*k]=(float)result;
    //atomicAdd(&a[i],1);
    printf("%f\n",result);
    //a[i][j][k]=0.8*(b[i-1][j][k]+b[i+1][j][k]+b[i][j-1][k]+b[i][j+1][k]+b[i][j][k-1]+b[i][j][k+1]);
}

float* q2(float * b){
    float* d_a;
    float* d_b;
    //int* d_n;
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
    /*cudaStatus = cudaMalloc((void**)&d_n, sizeof(int));
    if(cudaStatus != cudaSuccess)
        printf("d_n not allocated.\n");
    int n = N;
    cudaStatus = cudaMemcpy(d_n, &n, sizeof(int), cudaMemcpyHostToDevice);
    if(cudaStatus != cudaSuccess)
        printf("d_n not copied.\n");*/
    // finally, run on gpu
    int d = (int)sqrt(N);
    dim3 grid(d,d,d);
    dim3 block(d,d,d);
    kernel <<< grid, block >>> (d_a, d_b, N);
    cudaDeviceSynchronize();
    // copy result back to host
    float * a = (float *)malloc(N*N*N*sizeof(float));
    cudaStatus = cudaMemcpy(a, d_a, (N*N*N)*sizeof(float), cudaMemcpyDeviceToHost);
    if(cudaStatus != cudaSuccess)
        printf("a not returned to host.\n");
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
    a = q2(b);
    for(int i=0; i<N; i++)
        for(int j=0; j<N; j++)
            for(int k=0; k<N; k++)
                if(a[i+j+k]>0.) printf("[%d,%d,%d] : %f\n", i+1, j+1, k+1, a[i+j+k]); 
    return 0;
}
