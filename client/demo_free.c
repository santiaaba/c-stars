#include <stdio.h>
#include <stdlib.h>
void main(){
	int *a = NULL;

	a = (int*)malloc(sizeof(int));
	printf("puntero %p\n",a);
	free(a);
	printf("puntero %p\n",a);
	a = (int*)malloc(sizeof(int));
}
