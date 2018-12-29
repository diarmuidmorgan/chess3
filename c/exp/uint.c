#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
int main() {
	
	uint64_t a = 5;
	printf("%d\n",a);
	uint64_t b = 71776119061217280;
	printf("%" PRIu64 "\n", b);
	b = b >> 1;
	printf("%" PRIu64 "\n", b);


}
