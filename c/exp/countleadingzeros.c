#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <strings.h>

int main () {

	uint64_t a = 1LL;
	printf("%d\n", ffsll(a));
	printf("%d\n", __builtin_clzll(a));

}
