#include <stdio.h>
#include <stdlib.h>

typedef struct {

	int a [2];

} shit;
shit produce () {
	shit s;
	s.a[0] = 1;
	s.a[1] = 2;
	return s;

}
void reconfigure (shit * s) {

	s->a[0] = 123;
	
}
int [] create_arr () {

	int arr[2] = {2,3};
	return arr;
}
int main () {

	shit s = produce();
	reconfigure(&s);
	printf("%d\n", s.a[0]);
	int arr[2] = create_arr();
	printf("%d\n", arr[0]);
	return 0;


}
