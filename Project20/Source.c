#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void printArray(int* arr, int n)
{
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

void swap(int* a, int* b) {
    int c = *a;
    *a = *b;
    *b = c;
}

int hoare_partition(int* arr, int low, int high)
{
    int pivot = arr[low];
    int i = low - 1;
    int j = high + 1;
    while (1) {
        while (arr[++i] < pivot);
        while (arr[--j] > pivot);
        if (i >= j)
            return j;
        swap(arr + i, arr + j);
    }
}

int lomuto_partition_naive(int* arr, int low, int high) {
    if (high == low )
        return low;
    int pivot = arr[low];
    int i = low;
    for (int j = low + 1; j <= high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr + i, arr + j);
        }
    }
    swap(arr + low, arr + i);
    return i;
}
int lomuto_partition_branchfree(int* arr, int low, int high) {
    if (high == low)
        return low;
    int pivot = arr[low];
    int i = low;
    while (arr[i++] < pivot);
    for (int j = i; j <= high; j++) {
        int smaller = -(arr[j] < pivot);
        int buff = smaller & (arr[j] - arr[i]);
        arr[i] += buff;
        arr[j] -= buff;
        i -= smaller;
    }
    swap(arr + low, arr + --i);
    return i;
}
void quickSort(int* arr, int low, int high, int (*partition)(int*, int, int))
{
    if (low < high) {
        int ind = partition(arr, low, high);
        quickSort(arr, low, ind, partition);
        quickSort(arr, ind + 1, high, partition);
    }
}

int benchmark(void (*func)(void*), void* args) {
    clock_t time = clock();
    func(args);
    return (int)(clock() - time);
}
#define N 10
void test1(void* args) {
    int* arr = *((int**)args);
    quickSort(arr, 0, N - 1, lomuto_partition_naive);
    printArray(arr, N);
}

int main()
{
    unsigned int seed = 777;
    srand(seed);
    int n = 10;
    int* arr1 = (int*)malloc(n * sizeof(int));
    int* arr2 = (int*)malloc(n * sizeof(int));
    int* arr3 = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        int buff = rand();
        arr1[i] = buff;
        arr2[i] = buff;
        arr3[i] = buff;
    }
    quickSort(arr1, 0, n - 1, lomuto_partition_naive);
    quickSort(arr2, 0, n - 1, hoare_partition);
    quickSort(arr3, 0, n - 1, lomuto_partition_branchfree);
	printArray(arr1, n);
    printArray(arr2, n);
    printArray(arr3, n);
    return 0;
}
