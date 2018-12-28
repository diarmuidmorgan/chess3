#include <strings.h>
#include <stdint.h>
#include <stdio.h>

int main () {

	uint64_t x = 16;
	printf("%d\n",x);
	printf("%d\n", ffs(x));
	x = x >> 1;
	printf("%d\n",x);
	printf("%d\n", ffs(x));


	x = x >> 1;
	printf("%d\n", x);
	printf("%d\n", ffs(x));

	x = x >> 1;
	printf("%d\n", x);
	printf("%d\n", ffs(x));

	if (x==0) {
	printf("fucked");

	}



}
