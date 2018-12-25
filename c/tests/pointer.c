#include <stdio.h>
#include <stdlib.h>
int test (int * c) {

	*c = 3;
	printf("%d",*c);
}

int main () {

	//int * c = malloc(sizeof(int));
	int c = 2;
	
	test(&c);
	printf("%d", c);
	return 0;


}
