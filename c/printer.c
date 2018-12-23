#include "game_state.c"
#include <strings.h>
#include <stdint.h>
#include <stdio.h>
//#include "binary_ops.c"
void set_string(char * s, char symbol, uint64_t board){
	uint64_t i;
	uint64_t incr = 0;
	uint64_t zero = 0LL;
	int q=0;
	while (board != 0 && q <8) {
	
		i = ffsll(board);
		incr += i;
		s[incr-1] = symbol;	
		board = board >> i;
		q+=1;

	}

}	
void print_game_state(GS * gs) {

	int index;
	char * string = malloc(64 * sizeof(char));
	for (int i =0; i<64;i++) string[i] = '_';
	set_string(string, 'p', gs->pawns[0]);
	
	set_string(string, 'r', gs->rooks[0]);
	
	set_string(string, 'h', gs->knights[0]);
	set_string(string, 'b', gs->bishops[0]);
	set_string(string, 'q', gs->queens[0]);
	set_string(string, 'k', gs->kings[0]);
	
	set_string(string, 'P', gs->pawns[1]);
	
	set_string(string, 'R', gs->rooks[1]);
	set_string(string, 'H', gs->knights[1]);
	set_string(string, 'B', gs->bishops[1]);
	
	set_string(string, 'Q', gs->queens[1]);
	set_string(string, 'k', gs->kings[1]);
	
	printf("\n");
	for (int i = 0; i <8; i++){

		for (int j = 0; j<8; j++){
		printf("%c",string[i*8 + j]);

		}
		printf("\n");


	}	

}


int main () {
	
	//printf("HELLO");	
	GS * gs = initial_game_state();
	//printf("%d", gs->pawns[1]);
	//printf("\n%d", gs->pawns[0]);
	
	print_game_state(gs);
	return 0;

}
