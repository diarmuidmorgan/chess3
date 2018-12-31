#include <stdio.h>

int test (int *num) {

	*num -= 1;
	if (*num > 0) return 1;
	
	return NULL;	

}

int main () {

	int a = 2;
	int b;
	while (b=test(&a)){

	printf("\nTRUE\n");
	}
	printf("\nFALSE\n");


}
