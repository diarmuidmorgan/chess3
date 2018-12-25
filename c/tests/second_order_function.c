#include <stdio.h>

void test ( void (*greeter)()) {

	greeter();
}	

void hello() {
	printf("\nHELLO\n");

}

int main () {

	test(hello);
	





}
