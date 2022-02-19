#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <omp.h>
#include "quick-sort.c"
#include "merge-sort.c"
#include "enumeration-sort.c"
#include "enumeration-sort-parallel.c"
#include "merge-sort-parallel.c"
#include "quick-sort-parallel.c"
#define NUM_THREADS 4
struct timeval start, end;

//a function to gerenate random array
void randomArray(double *a, int size)
{
    srand((unsigned)time(NULL));
    for (int i = 0; i < size; i++)
    {
        a[i] = (double)(rand() % 10000 + 1) / (rand() % 100 + 1);
    }
}

// serial solution for quick sort
double *quickSort(double *a, int size)
{
    gettimeofday(&start, NULL);
    serialQuickSort(a, 0, size - 1);
    gettimeofday(&end, NULL);
    double delta = ((end.tv_sec - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6;
    printf("time spent by serial quick sort: %12.10f\n", delta);
    return a;
}

// serial solution for merge sort
double *mergeSort(double *a, int size)
{
    gettimeofday(&start, NULL);
    serialMergeSort(a, 0, size - 1);
    gettimeofday(&end, NULL);
    double delta = ((end.tv_sec - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6;
    printf("time spent by serial merge sort: %12.10f\n", delta);
    return a;
}

// serial solution for enumeration sort
double *enumerationSort(double *a, int size)
{
    gettimeofday(&start, NULL);
    serialEnumerationSort(a, size);
    gettimeofday(&end, NULL);
    double delta = ((end.tv_sec - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6;
    printf("time spent by serial enumeration sort: %12.10f\n", delta);
    return a;
}

// parallel solution for quick sort
double *quickSortParallel(double *a, int size)
{
    gettimeofday(&start, NULL);
#pragma omp parallel
    {
#pragma omp single
        parallelQuickSort(a, 0, size - 1);
    }
    gettimeofday(&end, NULL);
    double delta = ((end.tv_sec - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6;
    printf("time spent by parallel quick sort: %12.10f\n", delta);
    return a;
}

// parallel solution for merge sort
double *mergeSortParallel(double *a, int size)
{
    gettimeofday(&start, NULL);
#pragma omp parallel
    {
#pragma omp single
        parallelMergeSort(a, 0, size - 1);
    }
    gettimeofday(&end, NULL);
    double delta = ((end.tv_sec - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6;
    printf("time spent by parallel merge sort: %12.10f\n", delta);
    return a;
}

// parallel solution for enumeration sort
double *enumSortParallel(double *a, int size)
{
    gettimeofday(&start, NULL);
    parallelEnumerationSort(a, size);
    gettimeofday(&end, NULL);
    double delta = ((end.tv_sec - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6;
    printf("time spent by parallel enumeration sort: %12.10f\n", delta);
    return a;
}

// a function to analyse the sorted array
void sortedAnalysis(double *serialSorted, double *parallelSorted, int size)
{
    for (int i = 1; i < size; i++)
    {
        if (serialSorted[i - 1] > serialSorted[i])
        {
            printf("incorrect serial sorting array!\n");
            printf("-----------------------\n");
            return;
        }
        if (parallelSorted[i - 1] > parallelSorted[i])
        {
            printf("incorrect parallel sorting array!\n");
            printf("-----------------------\n");
            return;
        }
        if (serialSorted[i] != parallelSorted[i])
        {
            printf("Sorting results are not coincide!\n");
            printf("-----------------------\n");
            return;
        }
    }
    printf("sorting successful!\n");
    printf("-----------------------\n");
}

int main(int argc, char *argv[])
{
    //  check the number of arguments
    if (argc != 3)
    {
        fprintf(stderr, "Usage example: %s quick 100000\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int size = atoi(argv[2]);
    omp_set_num_threads(NUM_THREADS);
    double *a = (double *)malloc(size * sizeof(double));
    randomArray(a, size);
    double *tmp = (double *)malloc(size * sizeof(double));

    if (strcmp(argv[1], "quick")==0)
    {
        // copy the generated array a into tmp to maintain the same permutation
        memcpy(tmp, a, size * sizeof(double));
        double *quick = quickSort(tmp, size); //serial quick sort
        memcpy(tmp, a, size * sizeof(double));
        double *paraQuick = quickSortParallel(tmp, size); // parallel quick sort
        sortedAnalysis(quick, paraQuick, size);
    }
    else if (strcmp(argv[1], "merge")==0)
    {
        memcpy(tmp, a, size * sizeof(double));
        double *merge = mergeSort(tmp, size); // serial merge sort
        memcpy(tmp, a, size * sizeof(double));
        double *paraMerge = mergeSortParallel(tmp, size); // parallel merge sort
        sortedAnalysis(merge, paraMerge, size);
    }
    else if (strcmp(argv[1], "enum")==0)
    {
        memcpy(tmp, a, size * sizeof(double));
        double *enumeration = enumerationSort(tmp, size); // serial enumeration sort
        memcpy(tmp, a, size * sizeof(double));
        double *paraEnum = enumSortParallel(tmp, size); //parallel enumeration sort
        sortedAnalysis(enumeration, paraEnum, size);
    }
    else if(strcmp(argv[1],"all")==0)
    {
        memcpy(tmp, a, size * sizeof(double));
        double *quick = quickSort(tmp, size); //serial quick sort
        memcpy(tmp, a, size * sizeof(double));
        double *paraQuick = quickSortParallel(tmp, size); // parallel quick sort
        sortedAnalysis(quick, paraQuick, size);
        memcpy(tmp, a, size * sizeof(double));
        double *merge = mergeSort(tmp, size); // serial merge sort
        memcpy(tmp, a, size * sizeof(double));
        double *paraMerge = mergeSortParallel(tmp, size); // parallel merge sort
        sortedAnalysis(merge, paraMerge, size);
        memcpy(tmp, a, size * sizeof(double));
        double *enumeration = enumerationSort(tmp, size); // serial enumeration sort
        memcpy(tmp, a, size * sizeof(double));
        double *paraEnum = enumSortParallel(tmp, size); //parallel enumeration sort
        sortedAnalysis(enumeration, paraEnum, size);
    }
    else{
        printf("wrong arguments!\n");
        exit(EXIT_FAILURE);
    }
}
