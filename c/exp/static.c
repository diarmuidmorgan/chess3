
#include <stdio.h>

void myfunc(int *a) {

	*a = 0;
}

int func(int x) {

	static int a = x;
	a++;
	if (a % 5 == 0) {

		myfunc(&a);
	}
	return a;


}

int main () {

	int i = 0;
	while (i < 100) {

		printf("%d\n", func(1));
	i++;
	}
	return 0;

}
