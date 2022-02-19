<h1 align=center>CITS5507 High Performance Computing - Project1</h1>

<h2 align=center>Wenxiao Zhang (22792191)</h2>

## Introduction
<p style='text-align: justify;'>

This report contains five sections in total.  
Firstly, in <b>Serial sort pseudocode</b> and <b>Parallel sort pseudocode</b> section, pseudocode are applied to explain each serial sorting algorithm and each parallel sorting algorithm respectively. In <b>Experimental environment</b> section, the software and hardware environment to perform the experiment are introduced. In addtion, <b>Compile method description</b> section mainly describes how the program is compiled. Finally, in <b>Speed analysis</b> section, the results of the experiment are listed and analysed.
</p>

## Sections

- [Introduction](#introduction)
- [Sections](#sections)
- [Serial sort pseudocode](#serial-sort-pseudocode)
  - [1. Serial quick sort Pseudocode](#1-serial-quick-sort-pseudocode)
  - [2. Serial merge sort pseudocode](#2-serial-merge-sort-pseudocode)
  - [3. Serial enumeration sort pseudocode](#3-serial-enumeration-sort-pseudocode)
  - [Summary](#summary)
- [Parallel sort pseudocode](#parallel-sort-pseudocode)
  - [1. Parallel quick sort pseudocode](#1-parallel-quick-sort-pseudocode)
  - [2. Parallel merge sort pseudocode](#2-parallel-merge-sort-pseudocode)
  - [3. Parallel enumeration sort pseudocode](#3-parallel-enumeration-sort-pseudocode)
  - [Summary](#summary-1)
- [Experimental environment](#experimental-environment)
- [Compile method description](#compile-method-description)
- [Speedup analysis](#speedup-analysis)
  - [Array sizes 50,000](#array-sizes-50000)
  - [Array sizes 100,000](#array-sizes-100000)
  - [Array sizes 300,000](#array-sizes-300000)
  - [Array sizes 1,000,000](#array-sizes-1000000)
  - [Array sizes 10,000,000](#array-sizes-10000000)
  - [Array sizes 20,000,000](#array-sizes-20000000)

<br>



<div style="page-break-after: always;"></div>

## Serial sort pseudocode

### 1. Serial quick sort Pseudocode

```c
partition(array, left pointer, right pointer) {
    Initialize pivot <- left pointer;
    while (left pointer less than right pointer) 

        //Move right pointer to left until the while condition is
        // not satisfied.    
        while((array[right pointer] greater than or equal to pivot) and 
            (right pointer not equal to left pointer)) 
            increment right pointer by 1;
        endwhile  

        //Move left pointer to right until the while condition is
        // not satisfied.
        array[left pointer] <- array[right pointer]; 
        while((array[left pointer] less than pivot) and 
            (left pointer not equal to right pointer)) 
            increment left pointer by 1;
        endwhile  
        array[right pointer] <- a[left pointer];
    endwhile
    array[left pointer] <- pivot;
    return left pointer;
}

quickSort(array, left pointer, right pointer) {
    if (left pointer greater than or equal to right pointer)
        return;
    endif
    Initialize mid pointer <- partition(array, left pointer, right pointer);  
    quickSort(array, left pointer, mid pointer);
    quickSort(array, mid pointer + 1, right pointer);
}
```


<div style="page-break-after: always;"></div>

### 2. Serial merge sort pseudocode

```c
merge(array, left index, mid index, right index) {
    Intialize temp <- a temporary memory sizes 
        (right index - left index + 1)*sizeof(double);
    Intialize left pointer <- left index;
    Intialize mid pointer <- mid index;

    //pop the smaller element each time and push it into temp
    while (left pointer less than mid index and 
        mid pointer less than or equal to right index )
        if (array[left pointer] less than array[mid pointer])
            Push a[left pointer] into temp;
            move left pointer to right by 1;
        else
            Push a[mid pointer] into temp;
            move mid pointer to right by 1;
        endif
    endwhile
    
    //Push remaining in the either of the two parts of the array into temp
    while(left pointer less than mid index){
        push a[left pointer] into temp;
        move left pointer to right by 1;
    }
    while(mid pointer less than right index){
        Push a[mid pointer] into temp;
        move mid pointer to right by 1;
    }

    //copy temp into the corresponding address of the array
    array[left index, right index] <- temp
    free temp;
}

mergeSort(array, left index, right index) {
    if (left index equal to right index )
        return;
    endif
    mid index = (left index + right index) / 2;
    mergeSort(array, left index, mid index);
    mergeSort(array, mid index + 1, right index);
    merge(array, left index, mid index + 1, right index);
}
```

<div style="page-break-after: always;"></div>

### 3. Serial enumeration sort pseudocode

```c
enumerationSort(array, length of the array){
    Initialize rank<-0;
    Initialize temp <- a temporary memory sizes length*sizeof(double);
    for i in range (0, length of the array -1)
        reset rank to 0;
        //count the rank of the current element
        for j in range (0, length of the array -1)
            if ((array[i] not equal to array[j]) or 
                (i greater than j and a[i] equal to a[j]))
                increment rank by 1;
            endif
        endfor
    endfor
    copy temp into the array;
    free temp;
}
```

<br>
<br>

### Summary
The average case run time of quick sort is O(nlogn) . This case happens when we dont exactly get evenly balanced partitions. However, it is not a stable algorithm because the swapping of elements is done according to pivot’s position, which means the worst case runtime of quick sort can be O(n^2).

Merge sort is an efficient, stable sorting algorithm with an average, best-case, and worst-case time complexity of O(nlogn). However, it takes more memory resources as it needs to allocate a new set of memory location each time when performing merge operation.

As enumeration sort has a nested for-loop, the time complextiy of which is always O(n^2), it may have the worst performance among these three algorithms.

<div style="page-break-after: always;"></div>

## Parallel sort pseudocode

### 1. Parallel quick sort pseudocode
```c
parallelPartition(array, left pointer, right pointer){
    (same as partition() in serial quick sort)
}
parallelQuickSort(array, left pointer, right pointer){
    if (left pointer greater than or equal to right pointer)
        return;
    endif
    Initialize mid pointer <- partition(array, left pointer, right pointer);  
    #task directive //generate task
        #untied clause //make task untied
            #firstprivate(mid) //copy initialized mid pointer to every thread
                #if((mid pointer - left pointer) greater than 10000)
                {// executed parallelly or not is depend on the if condition 
                    parallelQuickSort(array, left pointer, mid pointer);
                }
    #task directive //generate task
        #untied clause //make task untied
            #firstprivate(mid) //copy initialized mid pointer to every thread
                #if((right pointer-mid pointer-1) greater than 10000)
                {// executed parallelly or not is depend on the if condition 
                    parallelQuickSort(array, mid pointer + 1, right pointer);
                }
}


#parallel directive{
    #omp single driective{  //generate tasks
        parallelQuickSort(array, left pointer, right pointer);
    }    
}


```

<div style="page-break-after: always;"></div>

### 2. Parallel merge sort pseudocode
```c
parallelMerge(array, left index, mid index, right index){
    (same as merge() in serial merge sort)
}


parallelMergeSort(array, left index, right index) {
    if (left index equal to right index )
        return;
    endif
    #task directive //generate task
        #untied clause //make task untied
            #share(array) //share the array to every thread    
            if ((mid index - left index) greater than 10000)
            {// executed parallelly or not is depend on the if condition 
                mergeSort(array, left index, mid index);
            } 
    
    #task directive //generate task
        #untied clause //make task untied
            #share(array) //share the array to every thread       
                #if((right index-mid index-1) greater than 10000) 
                { // executed parallelly or not is depend on the if condition 
                    mergeSort(array, mid index + 1, right index);
                }
    #taskwait directive{ //execute merge after division in each recursion
            merge(array, left index, mid index + 1, right index);
    }

}

#parallel directive{
    #omp single driective{  //generate tasks
        parallelMergeSort(array, left index, right index);
    }    
}


```

<div style="page-break-after: always;"></div>

### 3. Parallel enumeration sort pseudocode
```c
enumerationSort(array, length of the array){
    Initialize rank <- 0;
    Initialize temp <- a temporary memory sizes length*sizeof(double);
    #parallel directive{
        //dynamically distribute iterations to each thread
        #omp for dynamic{
            set rank private to every thread{
                for i in range (0, length of the array -1)
                    reset rank to 0;
                    for j in range (0, length of the array -1)
                        if ((array[i] not equal to array[j]) or 
                            (i greater than j and a[i] equal to a[j]))
                            increment rank by 1;
                        endif
                    endfor
                endfor
            }
        //dynamically distribute iterations to each thread
        #omp for dynamic{
            copy temp into the array;
        }
    }

    free temp;
}

```
<br>
<br>

### Summary

When using OpenMP to perform parallel solution, applying task directive to quick sort and merge sort can deal with the rescursive process effectively. As for the enumeration sort, omp for and omp schedule can be applied to this sorting algorithm to dynamically distribute iterations to each thread.

parallel quick sort needs to set the mid pointer (which obtained after partition) to be firstprivate. So that each thread can have a local copy of the mid value, which can increase their performing speed and avoid race condition. 

As for parallel merge sort, each thread may perform the sorting by traversing different parts of the array, so that we can make the array be shared to increase their performing speed. However, as merge sort needs to divide array up before sorting,  it needs a barrier(i.e. taskwait) to achieve this.


<div style="page-break-after: always;"></div>


## Experimental environment
The experiment is performed in the virtual machine running by VirtualBox. Using Vagrant to set parameters and log in to the virtual machine. Here are the details of the experimental environment:

- **VM Memory**: 4096 MB
- **Number of VM Processor**: 4
- **gcc version**: 10.0
- **Operating System**: bento/ubuntu-18.04 powered by VirtualBox
- **Processor**: Intel(R) Core(TM) i7-7700HQ CPU @ 2.80GHz

<br>

## Compile method description


1. Seven C files are created, six of them are applied to write sorting functions for each type of sorting algorithm, i.e., **quick-sort.c**, **merge-sort.c**, **enumeration-sort.c** for serial sorting algorithms and **quick-sort-parallel.c**, **merge-sort-parallel.c**, **enumeration-sort-parallel.c** for parallel sorting algorithms.

2. The remaining C file **sort.c** is the file contains main function. It has the following features:
   - set the number of the thread (four threads are set in this experiment)
   - a function named **randomArray()** to generate an array with random **double** typed values. 
   - apply **sortedAnalysis()** to check the correctness of the sorting results for each type of sorting algorithm.
   - six functions to calculate the elapsed time for each type of sorting algorithm by call each sorting function respectively. 

3. Use command: **gcc -fopenmp -o sort sort.c** to compile sort.c.
4. Use command: **./sort algorithmName arraySize** to execute the program. The **algorithmName** can be "**quick**", "**merge**", "**enum**", and "**all**". Each of them will test both serial sort and parallel sort of that algorithm (or all algorthms if you use "**all**") by sorting the unsorted arrays which have the same permutation. The **arraySize** can be any integer value. However, it could be **very slow** when using a very large size (>100000) to test "**enum**" sort, as the time complexity of it is very high (O(n^2)). The time elapsed results and its correctness will be printed out in the terminal after the program being executed.


<br>


## Speedup analysis

 We'll analyse the difference of performance between serial sort and parallel sort by changing the size (50k, 100k, 300k, 1m, 10m, 20m) of the array to be sorted. Applying a 2x3 table, two rows represents the solution type: serial/parallel, three columns represents three algorithms: quick, merge, enumeration. The value in the cells are the test results of the elapsed time in seconds.

<br>

### Array sizes 50,000

|              	| quick    	| merge    	| enumeration 	|
|--------------	|----------	|----------	|:-----------:	|
| **serial**   	| 0.008126 	| 0.011328 	| 13.901986     |
| **parallel** 	| 0.005631 	| 0.006135 	| 4.6902590     |


### Array sizes 100,000
|              	| quick    	| merge    	| enumeration 	|
|--------------	|----------	|----------	|:-----------:	|
| **serial**   	| 0.016440 	| 0.024456 	| 57.611739   	|
| **parallel** 	| 0.010704 	| 0.016246 	| 19.898863   	|


### Array sizes 300,000
|              	| quick    	| merge    	| enumeration 	|
|--------------	|----------	|----------	|:-----------:	|
| **serial**   	| 0.052346 	| 0.072184 	|  500.374546  	|
| **parallel** 	| 0.020471 	| 0.044470 	|  175.850024 	|


### Array sizes 1,000,000
|              	| quick    	| merge    	| enumeration 	|
|--------------	|----------	|----------	|:-----------:	|
| **serial**   	| 0.177850 	| 0.243721 	| -  	        |
| **parallel** 	| 0.068197 	| 0.132652 	| -   	        |


### Array sizes 10,000,000
|              	| quick    	| merge    	| enumeration 	|
|--------------	|----------	|----------	|:-----------:	|
| **serial**   	| 2.370862 	| 2.826111 	| -  	        |
| **parallel** 	| 0.798341 	| 1.430225 	| -   	        |


### Array sizes 20,000,000
|              	| quick    	| merge    	| enumeration 	|
|--------------	|----------	|----------	|:-----------:	|
| **serial**   	| 5.416288 	| 5.729092 	| -  	        |
| **parallel** 	| 1.897848 	| 3.026863 	| -   	        |

<p style='text-align: justify;'>
We can see quick sort is the fastest sorting algorithm among these three sorting algorithms, followed by merge sort. As enumeration sort has the time complexity of O(n^2), it is much slower than other sorting algorithms, especially when it comes to larger size of array.
<br>
For quick sort, the parallel solution has a better performance with larger size of array. The speedup outcomes of parallel solution varies between 40%~65% faster than its serial solution based on the array size, which means the array size bigger, the speedup of parallel solution more obvious.
<br>
For merge sort, the parallel solution is about 50%~55% faster than its serial solution. Although merge sort has a more stable speedup outcomes of parallel solution, it is slower than quick sort overall.
<br>
For enumeration sort, the parallel solution is about 3 times faster than its serial solution. However, due to the issue of high time complexity, the overall performance of enumeration sort is much poorer than the other two sorting algorithms.
</p>