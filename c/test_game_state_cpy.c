#include "game_state.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main () {

	GS * g1 = initial_game_state();
	printf("%" PRIu64 "\n",g1->knights[1]);
	GS * g2 = malloc(sizeof(GS));
       	memcpy(g2,g1,sizeof(GS));
	g2->knights[1] = 1823781323;
	printf("%" PRIu64 "\n", g2->knights[1]);	
	printf("%" PRIu64 "\n", g1->knights[1]);
	return 0;



}
