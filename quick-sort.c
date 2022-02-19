int partition(double *a, int left, int right)
{
    double pivot = a[left];

    while (left < right)
    {
        //Move right pointer to left until the while condition is
        // not satisfied.
        while (left < right && a[right] >= pivot)
        {
            right--;
        }
        
        //fill a[left] up with the value smaller than pivot
        a[left] = a[right];

        //Move left pointer to right until the while condition is
        // not satisfied.
        while (left < right && a[left] <= pivot)
        {
            left++;
        }

        //fill a[right] up the value greater than pivot
        a[right] = a[left];
    }

    // set the value to the number in the middle of the array
    a[left] = pivot;
    return left;
}

void serialQuickSort(double *a, int left, int right)
{
    if (left >= right)
        return;
    int mid = partition(a, left, right);
    serialQuickSort(a, left, mid);
    serialQuickSort(a, mid + 1, right);
}