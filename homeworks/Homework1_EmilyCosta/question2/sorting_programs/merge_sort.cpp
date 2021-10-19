// C++ program for Merge Sort
#include <chrono>
#include <iostream>
#include <pthread.h>
using namespace std;
using namespace std::chrono;
#define SIZE 5000 
int array[SIZE];
int part = 0;

// Merges two subarrays of array[].
// First subarray is arr[begin..mid]
// Second subarray is arr[mid+1..end]
void merge(int const left, int const mid, int const right){
	auto const subArrayOne = mid - left + 1;
	auto const subArrayTwo = right - mid;

	// Create temp arrays
	auto *leftArray = new int[subArrayOne],
		*rightArray = new int[subArrayTwo];

	// Copy data to temp arrays leftArray[] and rightArray[]
	for (auto i = 0; i < subArrayOne; i++)
		leftArray[i] = array[left + i];
	for (auto j = 0; j < subArrayTwo; j++)
		rightArray[j] = array[mid + 1 + j];

	auto indexOfSubArrayOne = 0, // Initial index of first sub-array
		indexOfSubArrayTwo = 0; // Initial index of second sub-array
	int indexOfMergedArray = left; // Initial index of merged array

	// Merge the temp arrays back into array[left..right]
	while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo) {
		if (leftArray[indexOfSubArrayOne] <= rightArray[indexOfSubArrayTwo]) {
			array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
			indexOfSubArrayOne++;
		}
		else {
			array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
			indexOfSubArrayTwo++;
		}
		indexOfMergedArray++;
	}
	// Copy the remaining elements of
	// left[], if there are any
	while (indexOfSubArrayOne < subArrayOne) {
		array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
		indexOfSubArrayOne++;
		indexOfMergedArray++;
	}
	// Copy the remaining elements of
	// right[], if there are any
	while (indexOfSubArrayTwo < subArrayTwo) {
		array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
		indexOfSubArrayTwo++;
		indexOfMergedArray++;
	}
}

// begin is for left index and end is
// right index of the sub-array
// of arr to be sorted
void merge_sort(int const begin, int const end){
	if (begin >= end)
		return; // Returns recursively

	auto mid = begin + (end - begin) / 2;
	merge_sort(begin, mid);
	merge_sort(mid + 1, end);
	merge(begin, mid, end);
}

void* merge_sort(void* arg){
    // which part out of 4 parts
    int thread_part = part++;

    // calculating low and high
    int low = thread_part * (SIZE/4);
    int high = (thread_part + 1) * (SIZE/ 4) - 1;

    // evaluating mid point
    int mid = low + (high - low) / 2;
    if (low < high) {
        merge_sort(low, mid);
        merge_sort(mid + 1, high);
        merge(low, mid, high);
    }
    return 0;
}

int main(){	
	int n;

	// single threaded
	for(int i=0; i<SIZE; i++) array[i]=rand()%100000+1;
	auto start = high_resolution_clock::now();
	merge_sort(0, SIZE-1);
	auto end = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(end-start);
	cout << "Time taken by serial merge sort: "
         << duration.count() << " microseconds" << endl;

	// 2 threads
	for(int i=0; i<SIZE; i++) array[i]=rand()%100000+1;
	n = 2;
	pthread_t threads2[n];
	start = high_resolution_clock::now();
	for (int i=0; i<n; i++)
        	pthread_create(&threads2[i], NULL, merge_sort, (void*)NULL);
	for (int i=0; i<n; i++)
        	pthread_join(threads2[i], NULL);
	merge(0, SIZE/2, SIZE-1);
	end = high_resolution_clock::now();
        duration = duration_cast<microseconds>(end-start);
        cout << "Time taken by 2-threaded merge sort: "
         << duration.count() << " microseconds" << endl;
	part = 0;

	// 4 threads
        for(int i=0; i<SIZE; i++) array[i]=rand()%100000+1;
        n = 4;
        pthread_t threads4[n];
        start = high_resolution_clock::now();
        for (int i=0; i<n; i++)
                pthread_create(&threads4[i], NULL, merge_sort, (void*)NULL);
        for (int i=0; i<n; i++)
                pthread_join(threads4[i], NULL);
	merge(0, (SIZE/2-1)/2, SIZE/2-1);
    	merge(SIZE/2, SIZE/2+(SIZE-1-SIZE/2)/2, SIZE-1);
    	merge(0, (SIZE-1)/2, SIZE-1);
	end = high_resolution_clock::now();
        duration = duration_cast<microseconds>(end-start);
        cout << "Time taken by 4-threaded merge sort: "
         << duration.count() << " microseconds" << endl;
        part = 0;

	// 8 threads
	for(int i=0; i<SIZE; i++) array[i]=rand()%100000+1;
        n = 8;
        pthread_t threads8[n];
        start = high_resolution_clock::now();
        for (int i=0; i<n; i++)
                pthread_create(&threads8[i], NULL, merge_sort, (void*)NULL);
        for (int i=0; i<n; i++)
                pthread_join(threads8[i], NULL);
	merge(0, (1/8)*(SIZE-1), (1/4)*(SIZE-1));
	merge((1/4)*(SIZE-1), (3/8)*(SIZE-1), (1/2)*(SIZE-1));
	merge((1/2)*(SIZE-1), (5/8)*(SIZE-1), (3/4)*(SIZE-1));
	merge((3/4)*(SIZE-1), (7/8)*(SIZE-1), SIZE-1);
	merge(0, (1/4)*(SIZE-1), (1/2)*(SIZE-1));
	merge((1/2)*(SIZE-1), (3/4)*(SIZE-1), SIZE-1);
	merge(0, (1/2)*(SIZE-1), SIZE-1);
	end = high_resolution_clock::now();
        duration = duration_cast<microseconds>(end-start);
        cout << "Time taken by 8-threaded merge sort: "
         << duration.count() << " microseconds" << endl;
        part = 0;

	return 0;
}


