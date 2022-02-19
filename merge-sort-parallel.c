#include "merge-sort.c"

void parallelMergeSort(double *a, int left, int right)
{
    if (left == right)
    {
        return;
    }

    //divide array into two by mid index
    int mid = (left + right) / 2;

    //generate task, make task untied, share the array to every thread
    //executed parallelly or not is depend on the if condition
#pragma omp task shared(a) untied if (mid - left > 10000)
    parallelMergeSort(a, left, mid);
#pragma omp task shared(a) untied if (right - mid - 1 > 10000)
    parallelMergeSort(a, mid + 1, right);
#pragma omp taskwait
    merge(a, left, mid + 1, right);
}
