#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "start_positions.c"
#include "binary_ops.c"
/* Basic struct for holding position data. Could maybe look into packing bits on
  the last four values. Each field is an array of two uint64_t words where the first word is white
  and the second word is black.
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
	int score;

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
	gs->castle_king_side[0] = 0;
	gs->castle_king_side[1] = 0;
	gs->castle_queen_side[0] = 0;
	gs->castle_queen_side[1] = 0;
	gs->score = 0;
	return gs;

}

/* Turns out it would probably be better if GS is allocated on the stack, and THEN passed as a pointer,
	to any function that wants to use it.
	So this just returns the struct, not a pointer.
	 (Though it possibly makes sense to pass it as a value to the
	next level of the game loop stack? )

*/
GS init_game_state(){
	GS gs;
	gs.pawns[0] = board_to_word(starting_pawns_white());
	gs.pawns[1] = board_to_word(starting_pawns_black());
	gs.rooks[0] = board_to_word(starting_rooks_white());
	gs.rooks[1] = board_to_word(starting_rooks_black());
	gs.knights[0] = board_to_word(starting_knights_white());
	gs.knights[1] = board_to_word(starting_knights_black());
	gs.bishops[0] = board_to_word(starting_bishops_white());
	gs.bishops[1] = board_to_word(starting_bishops_black());
	gs.kings[0] = board_to_word(starting_king_white());
	gs.kings[1] = board_to_word(starting_king_black());
	gs.queens[0] = board_to_word(starting_queen_white());
	gs.queens[1] = board_to_word(starting_queen_black());
	gs.pieces[0] = gs.pawns[0] | gs.rooks[0] | gs.knights[0] 
		| gs.bishops[0] | gs.kings[0] | gs.queens[0]; 	
	gs.pieces[1] = gs.pawns[1] | gs.rooks[1] | gs.knights[1] 
		| gs.bishops[1] | gs.kings[1] | gs.queens[1];
	gs.all_pieces = gs.pieces[0] | gs.pieces[1];
	gs.enpassants[0] = 0LL;
	gs.enpassants[1] = 0LL;
	gs.color = 0;
	gs.score=0;
	gs.castle_king_side[0] = 0;
	gs.castle_king_side[1] = 0;
	gs.castle_queen_side[0] = 0;
	gs.castle_queen_side[1] = 0;
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
* Yes, unfortunately this is how we're planning to save parsed data!
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
					gs->bishops[1] |= 1LL << INDEX;
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
					gs->kings[1] |= 1LL << INDEX;
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
/* Hopefully a deep copy of the game state.

   Actually we don't even need to do this as struct doesn't contain pointers. 
   We can just copy with *new_gs = *gs.
*/
GS * copy_game_state (GS * gs){
	GS * new = malloc(sizeof(GS));
	memcpy(new, gs, sizeof(GS));
	return new;
}


/* Update the score of the game state. Do we want to do this?
* Would help with move ordering, but might be totally unnecessary otherwise.
*
*/
int node_score_change (GS * new_gs, GS * old_gs){
    int color = new_gs->color;
    //if pieces are the same, then know change has occurred.
    if (new_gs->pieces[color] == old_gs->pieces[color])
        return 0;
    if (new_gs->pawns[color] != old_gs->pawns[color])
        return 1 * color;
    else if (new_gs->knights[color] != old_gs->knights[color])
        return 3 * color;
    else if (new_gs->bishops[color] != old_gs->bishops[color])
        return 3 * color;
    else if (new_gs->rooks[color] != old_gs->rooks[color])
        return 5 * color;
     else if (new_gs->queens[color] != old_gs->queens[color])
        return 9 * color;
     else if (new_gs->kings[color] != old_gs->kings[color])
        //king got killed! Previous node was illegal.
        return 100000;
    


}

/* Hand game state to the other player.
*/
void flip_game_state (GS * gs, GS * prev_gs){

	int color = (gs->color + 1) %2;
	gs->color = color;
	//only update the new player's pieces if they have changed
	if (gs->pieces[color] == prev_gs->pieces[color])
		return;
	
	gs->score += node_score_change(gs, prev_gs);
	gs->pawns[color] &= gs->pieces[color];
	gs->rooks[color] &= gs->pieces[color];
	gs->knights[color] &= gs->pieces[color];
	gs->bishops[color] &= gs->pieces[color];
	gs->queens[color] &= gs->pieces[color];
	gs->knights[color] &= gs->pieces[color];


}


