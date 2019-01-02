#include <stdio.h>
#include <regex.h>
int main () {
	
	//char * s = "^[a-z]\b\d+(\n|\\+)";
	regex_t pforwards;
	int reti = regcomp(&pforwards,"^[a-z][0-9]+", 0);
	if (!reti)
		printf("COMPILED\n");
	int result = regexec(&pforwards, "e4+", 0, NULL, 0);
	if (!result) {

		printf("MATCH");
	}
	else{

		printf("NO MATCH");
	}
	
	return 0;



}
