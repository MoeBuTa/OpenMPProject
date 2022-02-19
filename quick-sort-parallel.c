#include "quick-sort.c"

void parallelQuickSort(double *a, int left, int right)
{

    if (left >= right)
        return;
    int mid = partition(a, left, right);

    //generate task, make task untied, copy initialized mid pointer to every thread
    //executed parallelly or not is depend on the if condition
#pragma omp task firstprivate(mid) untied if (mid - left > 10000)
    parallelQuickSort(a, left, mid);
#pragma omp task firstprivate(mid) untied if (right - mid - 1 > 10000)
    parallelQuickSort(a, mid + 1, right);
}