#include <stdio.h>

int test () {
	int a = 2;
	int b = 1 && ( a= 5);
	return a;

}

int main () { 

	printf("\n%d\n", test());

}
