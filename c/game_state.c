#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "start_positions.c"
#include "binary_ops.c"

/* GS gamestate structure. Basic struct for holding position data. Could maybe look into packing bits on
  the last four values. Each field is an array of two uint64_t words where the first word is white
  and the second word is black.
*
*/
typedef struct {

	//the first thing we do should be hash the position?
	uint64_t hash;

	//single word holds the squares that the king moved through if it castled on the last turn.
	uint64_t castle_ghost_squares[2];

	uint64_t pawns[2];
	uint64_t rooks[2];
	uint64_t knights[2];
	uint64_t bishops[2] ;
	uint64_t kings[2];
	uint64_t queens[2];
	uint64_t pieces[2];
	uint64_t all_pieces;
	uint64_t enpassants[2];
	//marks the squares the king passed after a castling attempt
	uint64_t ghost_squares[2];
	//is this really a good idea like?
	// actually we don't need this board_rep thing, or if we do,
	// not if we can use incremental hashing instead. That's 64 bytes that don't have 
	// to be copied?? And updating that, replaces the process of updating this. And makes
	// hash function computation fairly trivial.
	// thank fuck!
	
	int castle_king_side[2];
	int castle_queen_side[2];
	int color;
	int score;
	int has_castled[2];

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
	gs->ghost_squares[0] = 0LL;
	gs->ghost_squares[1] = 0LL;
	gs->has_castled[0] = 0;
	gs->has_castled[1] = 0;
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
	gs.castle_king_side[0] = 1;
	gs.castle_king_side[1] = 1;
	gs.castle_queen_side[0] = 1;
	gs.castle_queen_side[1] = 1;
	gs.ghost_squares[0]= 0LL;
	gs.ghost_squares[1]= 0LL;
	gs.has_castled[0] = 0;
	gs.has_castled[1] = 0;
	//is this really a good idea? Do we want a transposition table?
	// want to get rid of this at the first opportunity.
	
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

/* This basically just combines the flip game state functions and node score change functions
*  Into one function. Set the right bits, check whether the opposite players pieces have changed
	Update them, and update the score
	Also check for illegal castling using the castling ghost squares.
*/
void normal_game_state_update(GS * new_gs, uint64_t new_pos, 
					uint64_t old_pos, uint64_t * selected_pieces){
					
	int color = new_gs->color;
	int r_color = (color + 1) % 2;
	
	//update the pieces for the player who just moved.
	
	//'selected pieces' is a pointer to the filed in the game state (e.g pawns, rooks)
	//that needs to be updated. 
	//despite the game state itself being passed as a pointer.
	
	new_gs->pieces[color] = (new_gs->pieces[color] | new_pos) & (~old_pos);
	
	*selected_pieces = (*selected_pieces | new_pos) & (~old_pos);
	//binary_print_board(*selected_pieces);
	new_gs ->all_pieces = (new_gs->all_pieces | new_pos) & (~old_pos);

	//change the game state color;
	new_gs->color = r_color;
	
	//handle captures.
	//if this update changes the other sides piece's, we need to update every piece array,
	// and check for a base score change.
	// I think this is kind of inefficient, so many if statements.
	// At least, we should exit on the first condition for the majority of moves.
	// And hopefully exit on the pawns condition for most of the rest, right?

	//TBH it might not even be worth it keeping a running score count, 
	//easier to just check for it on the terminal nodes?
	
	// though it MIGHT be good if we implement move_ordering and quiesence search.

	if (((new_gs->pieces[r_color] & (~new_pos)) ) != new_gs->pieces[r_color]){
	//	printf("\nUPDATING\n");
		int multiplier;
		if (color == 1) multiplier = 1;
		else multiplier = -1;
		//binary_print_board(new_pos);
		new_gs ->pieces[r_color] = new_gs->pieces[r_color] & (~new_pos);
		//binary_print_board(new_gs->pieces[r_color]);
		//new_gs->all_pieces |= new_gs->pieces[r_color];
		//binary_print_board(new_gs->rooks[r_color]);
		uint64_t new_pieces = new_gs->pawns[r_color] & new_gs->pieces[r_color];
		
		if (new_pieces != new_gs->pawns[r_color]){
			
			new_gs->score += 1 * multiplier;
			new_gs->pawns[r_color] = new_pieces;
			return;
		}
		new_pieces = new_gs->pieces[r_color] & new_gs->bishops[r_color];
		if (new_pieces != new_gs->bishops[r_color]){
			
			new_gs->score += 3 * multiplier;
			new_gs->bishops[r_color] = new_pieces;
			//binary_print_board(new_gs->bishops[r_color]);
			return;
		}
		new_pieces = new_gs->knights[r_color] & new_gs->pieces[r_color];
		if (new_pieces != new_gs->knights[r_color]){
			new_gs->score += 3 * multiplier;
			new_gs->knights[r_color] = new_pieces;
			return;
		}
		
		new_pieces = new_gs->rooks[r_color] & new_gs->pieces[r_color];
		if (new_pieces != new_gs->rooks[r_color]){
			//binary_print_board(new_pieces);
			//binary_print_board(new_gs->rooks[r_color]);
			new_gs->score += 5 * multiplier;
			new_gs->rooks[r_color] = new_pieces;
			return;
		}
		new_pieces = new_gs->queens[r_color] & new_gs->pieces[r_color];
		if (new_pieces != new_gs->queens[r_color]){
			new_gs->score += 9 * multiplier;
			new_gs->queens[r_color] = new_pieces;
			return;
		}
		new_pieces = new_gs->kings[r_color] & new_gs->pieces[r_color];
		if (new_pieces != new_gs->kings[r_color]){
			new_gs->score += 1000 * multiplier;
			new_gs->kings[r_color] = new_pieces;
			return;
		}
		


	}
}


