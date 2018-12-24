#include <strings.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
int main () {

	uint64_t a = 9;
	printf("%d\n", ffsll(a));
	printf("%d", a >> ffsll(a));
       	return 0;	


}
