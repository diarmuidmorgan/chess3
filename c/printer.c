/*
 "Pretty" prints the game state to the command line
*/

#include "game_state.c"
#include <strings.h>
#include <stdint.h>
#include <stdio.h>
//#include "binary_ops.c"

/* Takes a word full of a single piece, the character to add to the char, and places that char
*  in all valid positions
*
*/
void set_string_cell(char * s, char symbol, uint64_t board){
	uint64_t i;
	uint64_t incr = 0;
	uint64_t zero = 0LL;
	int q=0;
	while (board != 0 && q <8) {
	
		i = ffsll(board);
		incr += i;
		s[incr-1] = symbol;	
		//dirty hack be careful here.
		if (i==64) break;
		board = board >> i;
		q+=1;

	}

}	
//Builds the string and then pretty prints it.

void print_game_state(GS * gs) {

	int index;
	char * string = malloc(64 * sizeof(char));
	for (int i =0; i<64;i++) string[i] = '_';
	set_string_cell(string, 'p', gs->pawns[0]);
	set_string_cell(string, 'r', gs->rooks[0]);
	set_string_cell(string, 'h', gs->knights[0]);
	set_string_cell(string, 'b', gs->bishops[0]);
	set_string_cell(string, 'q', gs->queens[0]);
	set_string_cell(string, 'k', gs->kings[0]);
	set_string_cell(string, 'P', gs->pawns[1]);
	set_string_cell(string, 'R', gs->rooks[1]);
	set_string_cell(string, 'H', gs->knights[1]);
	set_string_cell(string, 'B', gs->bishops[1]);
	set_string_cell(string, 'Q', gs->queens[1]);
	set_string_cell(string, 'K', gs->kings[1]);
	
	printf("\n");
	for (int i = 0; i <8; i++){

		for (int j = 0; j<8; j++){
		printf("%c",string[i*8 + j]);

		}
		printf("\n");


	}	

}


