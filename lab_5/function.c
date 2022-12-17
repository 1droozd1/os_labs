#include "stdio.h"
#include "stdlib.h"

void * sort_bubble(int* array) {
    int size = sizeof(array) / sizeof(array[0]);
    int i, j;
    int tmp;
    for (i = 1; i < size; i++) {
        for (j = 1; j < size; j++) {
            if (array[j] > array[j-1]) {
                tmp = array[j];
                array[j] = array[j-1];
                array[j-1] = tmp;
            }
        }
    }
}
//-----------------------------------------------------quick sort----------------------------------------------------------------

void swap(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

int partition(int array[], int low, int high)
{
    int pivot = array[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (array[j] < pivot) {
            i++;
            swap(&array[i], &array[j]);
        }
    }
    swap(&array[i + 1], &array[high]);
    return (i + 1);
}

void quickSort(int* array, int low, int high) //first low = 0; high = n - 1, where n = sizeof(array) / sizeof(array[0]);
{
    if (low < high) {
        int pi = partition(array, low, high);
        quickSort(array, low, pi - 1);
        quickSort(array, pi + 1, high);
    }
}

void quick_sort(int *array)
{
    int n = sizeof(array) / sizeof(array[0]);
    int low = 0;
    int high = n - 1;

    quickSort(array, low, high);
}

//----------------------------------------------------------------------------------------------------------------------


