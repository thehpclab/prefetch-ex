/*
** Author: Team The HPC Lab
** Email : admin@thehpclab.com
** Details: A program to demonstrate prefetching features on Xeon and Xeon Phi 
** 	    We have two data arrays say A and B. Both containing random numbers 
**	    in some fashion. The values stored in array A is related to array B
**	    in some way. The logic of the algorithm requires to scan the array A
**	    linearly one by one and do processing as outlined below
**	    Pseudo code
**		for(i = 1; i < size; i++)
**			{
**				int indexA = i;
**				int indexB = A[i];
**				B[indexB]*=indexA;
**			}
**
** Example: say A[30]=40, then indexA = 30, indexB = 40, we visit B[40] 
**	    and multiply B[40] with 30;
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
void randomize(int32_t* arr, int32_t size)
{
	for(uint32_t i=0;i<size;i++)
	{
		arr[i]=i;
	}
	//intialize the random number generator with current as seed
	srand( time(NULL) );
	int temp;
	int random;
	/* Swap the numbers in array using random seed, you can always write a faster swap function
	** I am not doing it to keep things simple and explicit
	**/
	for(uint32_t i=0;i<size;i++)
	{
		random = rand()%size;
		temp = arr[i];
		arr[i]=arr[random];
		arr[random] = temp;
	}
}


int main(int argc, char *argv[])
{
//Program takes size of the array as argument
if(argc < 3) 
	exit(1);
int32_t arrSize = atoi(argv[1]);
int32_t prefetchDistance = atoi(argv[2]);
int32_t *A = (uint32_t*) malloc(sizeof(uint32_t)*arrSize);
int32_t *B = (uint32_t*) malloc(sizeof(uint32_t)*arrSize);

//initialize and randomize the input data
randomize(A,arrSize);
randomize(B,arrSize);

// Lets create time storing variables
double start, startNoPrefetch, startPrefetch,end;

uint32_t indexB;
//first we run the code directly
start = clock(); 
for(uint32_t i=0;i<arrSize;i++)
{
	indexB = A[i];
	B[indexB]*=i;
}

//lets deliberately disable prefetching
startNoPrefetch = clock();
#pragma noprefetch A,B
for(uint32_t i=0;i<arrSize;i++)
{
	indexB = A[i];
	B[indexB]*=i;
}
startPrefetch = clock();
//lets now use prefetching
for(uint32_t i=0;i<arrSize;i++)
{
	if(i+prefetchDistance < arrSize)
	{
		int prefetchIndexB = A[i+prefetchDistance];
		__builtin_prefetch(&(B[prefetchIndexB]),1,1);
	}

	indexB = A[i];
	B[indexB]*=i;
}

end = clock();

//Print the timings
printf("\n Direct %g", ((startNoPrefetch - start)/CLOCKS_PER_SEC));
printf("\n No Prefetch %g", ((startPrefetch - startNoPrefetch)/CLOCKS_PER_SEC ) );
printf("\n Manual Prefetch %g", ((end - startPrefetch) /CLOCKS_PER_SEC));

//clean up
free(A);
free(B);
return 0;
}

