/**
 Cade Newell
 Project 4
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE				10
#define NUMBER_OF_THREADS	3

void *sorter(void *params);	/* thread that performs basic sorting algorithm */
void *merger(void *params);	/* thread that performs merging of results */

int list[SIZE] = {7,12,19,3,18,4,2,6,15,8};

int result[SIZE];

typedef struct
{
	int from_index;
	int to_index;
} parameters;

int main (int argc, const char * argv[]) 
{
	int i;
    
	pthread_t workers[NUMBER_OF_THREADS];
	
	/* establish the first sorting thread */
	//1. call malloc to allocate a “parameters”
    parameters *nums = (parameters *) malloc (sizeof(parameters));
	//2. use “parameters” to specify the first half of the array
    nums->from_index = 0;
    nums->to_index = (SIZE/2) - 1;
    //3. create the first thread
    pthread_create(&workers[0], 0, sorter, nums);
	

	/* establish the second sorting thread */
	//1. call malloc to allocate a “parameters”
    parameters *vals = (parameters *) malloc (sizeof(parameters));
	//2. use “parameters” to specify the first half of the array
    vals->from_index = (SIZE/2);
    vals->to_index = SIZE - 1;
    //3. create the second thread
    pthread_create(&workers[1], 0, sorter, vals);
	
	/* now wait for the 2 sorting threads to finish */
	// use ptheread_join; wait for 2 sorting threads to finish 
    for (i = 0; i < NUMBER_OF_THREADS - 1; i++)
    pthread_join(workers[i], NULL);
	

	/* establish the merge thread */
	//reuse “parameters” to hold the beginning index in each half
	//create the third thread: merge 
    parameters *merge = (parameters *) malloc(sizeof(parameters));
    merge->from_index = 0;
    merge->to_index = (SIZE/2);
    pthread_create(&workers[2], 0, merger, merge);
	
	/* wait for the merge thread to finish */
     pthread_join(workers[2], NULL);
	 
	/* output the sorted array */
    printf("Sorted elements of the array \n");
    for (int i = 0; i < 10; i++)
        printf("%d\n", result[i]);


	 
	
    return 0;
}

void swap(int *xp, int *yp) 
{ 
    int temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
}

/**
 * Sorting thread.
 *
 * This thread can essentially use any algorithm for sorting
 */

void *sorter(void *params)
{
    int i, j; 
	parameters* p = (parameters *)params;

    int start = p->from_index;
    int end = p->to_index;
	
// The holy sorting algorithm: bubble sort
   for (i = start; i < end; i++)          
       for (j = start; j < end; j++)  
           if (list[j] > list[j+1]) 
              swap(&list[j], &list[j+1]);

	pthread_exit(0);
}

/**
 * Merge thread
 *
 * Uses simple merge sort for merging two sublists
 */

void *merger(void *params)
{
    int i, j, k; 
	parameters* p = (parameters *)params;
	
	//reuse “parameters” to hold the first index in each half
    int start = p->from_index;
    int end   = p->to_index;
	//merge two sorted sublist to the arry result
    i = start;
    j = end;
    k = 0;
    while (i < end && j < SIZE) 
    { 
        if (list[i] <= list[j]) 
        { 
            result[k] = list[i]; 
            i++; 
        } 
        else
        { 
            result[k] = list[j]; 
            j++; 
        } 
        k++; 
    }

    while (i < end) 
    { 
        result[k] = list[i]; 
        i++; 
        k++; 
    }

    while (j < SIZE) 
    { 
        result[k] = list[j]; 
        j++; 
        k++; 
    } 


    
	
	pthread_exit(0);
}