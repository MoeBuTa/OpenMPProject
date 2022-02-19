
void parallelEnumerationSort(double *a, int length)
{
    int rank;
    double *temp = (double *)malloc(length * sizeof(double));
#pragma omp parallel
    {
//dynamically distribute iterations to each thread
#pragma omp for schedule(dynamic, 1) private(rank)
        for (int i = 0; i < length; i++)
        {
            rank = 0;
            for (int j = 0; j < length; j++)
            {
                //1. a[i] > a[j] element smaller
                //2. a[i] == a[j] && i > j element which has duplicate value
                if ((a[i] > a[j]) || (i > j && a[i] == a[j]))
                {
                    rank++;
                }
            }

            temp[rank] = a[i];
        }
//dynamically distribute iterations to each thread
#pragma omp for schedule(dynamic, 1)
        for (int k = 0; k < length; k++)
        {
            a[k] = temp[k];
        }
    }
    free(temp);
}
