#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
int main(int argc, char *argv[])
{
// Two arguments - date set size, prefetch distance
if(argc<3)
exit(1);
//store the arguments
int32_t arrSize = atoi(argv[1]);
int32_t prefetchDistance = atoi(argv[2]);

//allocate the memory
int32_t *A = (int32_t*) malloc(sizeof(uint32_t)*arrSize);

//initialize the array
for(uint32_t i = 0; i<arrSize; i++)
	A[i]=i;

//timer variables
double start, startNoPrefetch, end;

//be careful to compile with -no-vec -no-simd 
//start prefetch loop
start = clock();

#pragma prefetch A:0:prefetchDistance
for(uint32_t i = 0 ; i < arrSize; i++)
{
	A[i]= A[i+10]*3;
}

//start no prefetch loop
startNoPrefetch = clock();

#pragma noprefetch
for(uint32_t i = 0; i < arrSize; i++)
{
	A[i] = A[i+10]*3;
}

end = clock();

//print timings
printf("\n Prefetching enabled loop took %g seconds",(startNoPrefetch - start)/CLOCKS_PER_SEC);
printf("\n Prefetching disbaled loop took %g seconds",(end - startNoPrefetch)/CLOCKS_PER_SEC);

//free the memory
free(A);
return(0);

}
