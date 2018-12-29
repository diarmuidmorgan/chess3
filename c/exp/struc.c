#include <stdio.h>
#include <stdlib.h>

typedef struct {

	int a [2];

} shit;
int main () {

	shit * s = malloc(sizeof(shit));
	s->a[0] = 1;
	s->a[1] = 2;
	return 0;


}
