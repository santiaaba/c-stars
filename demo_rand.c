#include <time.h>
#include <stdlib.h>
#include <stdio.h>

void main(){
	srand(time(NULL));   // Initialization, should only be called once.
	int r = rand();      // Returns a pseudo-random integer between 0 and RAND_MAX.
	printf("random %i\n",r);
}
