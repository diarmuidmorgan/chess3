#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "start_positions.c"
#include "binary_ops.c"
/* Basic struct for holding position data
*
*/
typedef struct {

	uint64_t pawns[2];
	uint64_t rooks[2];
	uint64_t knights[2];
	uint64_t bishops[2] ;
	uint64_t kings[2];
	uint64_t queens[2];
	uint64_t pieces[2];
	uint64_t all_pieces;
	uint64_t enpassants[2];
	int castle_king_side[2];
	int castle_queen_side[2];
	int color;

} GS;
/* Constructor prodcedure. Returns a game state * with starting position set.
*
*/
GS * initial_game_state(){
	
	
	GS * gs = malloc(sizeof(GS));
	gs->pawns[0] = board_to_word(starting_pawns_white());
	gs->pawns[1] = board_to_word(starting_pawns_black());
	gs->rooks[0] = board_to_word(starting_rooks_white());
	gs->rooks[1] = board_to_word(starting_rooks_black());
	gs->knights[0] = board_to_word(starting_knights_white());
	gs->knights[1] = board_to_word(starting_knights_black());
	gs->bishops[0] = board_to_word(starting_bishops_white());
	gs->bishops[1] = board_to_word(starting_bishops_black());
	gs->kings[0] = board_to_word(starting_king_white());
	gs->kings[1] = board_to_word(starting_king_black());
	gs->queens[0] = board_to_word(starting_queen_white());
	gs->queens[1] = board_to_word(starting_queen_black());
	gs->pieces[0] = gs->pawns[0] | gs->rooks[0] | gs->knights[0] 
		| gs->bishops[0] | gs->kings[0] | gs->queens[0]; 	
	gs->pieces[1] = gs->pawns[1] | gs->rooks[1] | gs->knights[1] 
		| gs->bishops[1] | gs->kings[1] | gs->queens[1];
	gs->all_pieces = gs->pieces[0] | gs->pieces[1];
	gs->enpassants[0] = 0LL;
	gs->enpassants[1] = 0LL;
	gs->color = 0;
	return gs;

}
/* Helper method. Returns an empty game state *.
*
*/
GS * blank_game_state() {
	GS * gs = malloc(sizeof(GS));
	for (int i=0; i<2;i++) {
	gs->pawns[i] = 0LL;
	gs->rooks[i] = 0LL;
	gs->knights[i] = 0LL;
	gs->bishops[i] = 0LL;
	gs->kings[i] = 0LL;
	gs->queens[i] = 0LL;
	}
	return gs;
}
/* Helper method. Constructs a game state from a 64 character string
e.g ___________________________Q___________________________
*
*/

GS * game_state_from_position_string(char * s, int c) {
	GS * gs = blank_game_state();
	gs->color = c;
	for (int i=0;i<8; i++){

		for (int j=0; j<8;j++) {
			int INDEX = i*8 + j;
			switch(s[INDEX]) {

				case('p') :
					gs->pawns[0] |= 1LL << INDEX;
					break;
				case('P') :
					gs->pawns[1] |= 1LL << INDEX;
					break;
				case('r') :
					gs->rooks[0] |= 1LL << INDEX;
					break;
				case('R') :
					gs->rooks[1] |= 1LL << INDEX;
					break;
				case('h') :
					gs->knights[0] |= 1LL << INDEX;
					break;
				case('H') :
					gs->knights[1] |= 1LL << INDEX;
					break;
				case('b') :
					gs->bishops[0] |= 1LL << INDEX;
					break;
				case('B') :
					gs->pawns[1] |= 1LL << INDEX;
					break;
				case('q') :
					gs->queens[0] |= 1LL << INDEX;
					break;
				case('Q') :
					gs->queens[1] |= 1LL << INDEX;
					break;
				case('k') :
					gs->kings[0] |= 1LL << INDEX;
					break;
				case('K') :
					gs->kings[0] |= 1LL << INDEX;
					break;

				}
			}
		}
	for (int color =0; color < 2; color++){
		gs->pieces[color] = gs->pawns[color] | gs->rooks[color] 
				    | gs->knights[color] | gs->bishops[color] 
				    | gs->rooks[color] | gs->queens[color] | gs->kings [color];
		
		gs->enpassants[color] = 0LL;}
	gs->all_pieces = gs->pieces[0] | gs->pieces[1];
	return gs;
}

/* Reads position from file as 64 character string. Hopefully.
*
*/
GS * read_position_from_file (char * fname) {

	FILE *fp;
	char s[1000];
	fp =fopen(fname,"r");
	fgets(s,1000, fp);
	fclose(fp);
	return game_state_from_position_string(s,0);

}
GS * copy_game_state (GS * gs){
	GS * new = malloc(sizeof(GS));
	/* leads to madness and dumped cores
	*/
	//GS * new = malloc(sizeof(gs));
	for (int color = 0; color<2; color++) {
		new->pawns[color] = gs->pawns[color];
		new->knights[color] = gs->knights[color];
		new->rooks[color] =  gs->rooks[color];
		new->bishops[color] = gs->bishops[color];
		new->queens[color] = gs->queens[color];
		new->kings[color] = gs->kings[color];
		new->enpassants[color]= gs->enpassants[color];
		new->pieces[color]=  gs->pieces[color];

	}
	

	new ->all_pieces = gs->all_pieces;
	return new;
}
/* Should return a deep copy of the gamestate
* Currently does not, actually do that.
*
*
GS * copy_game_state(GS * gs) {
	/*
	 * This code only seems to return a shallow copy and I'm not sure why
	GS * new = malloc(sizeof(GS));
	memcpy(new,gs,sizeof(GS));
	return new;
	*
	GS * new = malloc(sizeof(gs));
	for (int color = 0; color<2; color++) {

	
	
	/*this is all fucked!	
	*new->pawns[color] = *(gs->pawns[color]);
	*new->knights[color] = *(gs->knights[color]);
	*new->rooks[color] = *(gs->rooks[color]);
	*new->bishops[color] = *(gs->bishops[color]);
	*new->queens[color] = *(gs->queens[color]);
	*new->kings[color] = *(gs->kings[color]);
	*new->enpassants[color]=*(gs->enpassants[color]);
	*new->pieces[color]=*(gs->pieces[color]);
	*
	memcpy(new->pawns[color], gs->pawns[color], sizeof(uint64_t));
	memcpy(new->knights[color], gs->knights[color], sizeof(uint64_t));
	memcpy(new->rooks[color], gs->rooks[color], sizeof(uint64_t));
	memcpy(new->bishops[color], gs->bishops[color], sizeof(uint64_t));
	memcpy(new->queens[color], gs->queens[color], sizeof(uint64_t));
	memcpy(new->kings[color], gs->kings[color], sizeof(uint64_t));
	memcpy(new->enpassants[color], gs->enpassants[color], sizeof(uint64_t));
	memcpy(new->pieces[color], gs->pieces[color], sizeof(uint64_t));

	}
	new->color = gs->color;
	memcpy(new->all_pieces, gs->all_pieces,sizeof(uint64_t));
	return new;
}

* Procedure for handing the game state from one player to the next.
* Needs to check for and update any pieces that have been removed.
*
*
void game_state_change_color(GS * gs, int color) {
	//flip the color bit
	gs->color = color;
	gs->pawns[color] = gs->pawns[color] & gs->pieces[color];
	gs->rooks[color] = gs->rooks[color] & gs->pieces[color];
	gs->knights[color] = gs->knights[color] & gs->pieces[color];
	gs->bishops[color] = gs->bishops[color] & gs->pieces[color];
	gs->kings[color] = gs->kings[color] & gs->pieces[color];
	gs->queens[color] = gs->queens[color] & gs->pieces[color];


}

*/

