#include "moves_base.c"
#include <math.h>
/* Simple bitboard serilaizer, used by pin generation.
*
*/
int cycle_pieces (GS * gs, uint64_t * pieces, int * piece_incr){
	if(*pieces==0LL) return 0;
	int index_p = ffsll(*pieces);
	*piece_incr += index_p;
	*pieces = *pieces >> index_p;
	return 1;
}

/* Serializes piece bitboards, and sets the move square mask.
	Also shifts the piece_incr which is used for I dunno what anymore.
*
*/
int next_piece(GS * gs, int msk_number, 
		uint64_t * msks, uint64_t * pieces, 
		uint64_t * move_squares, int * piece_incr, 
		int * move_incr, void (*masking_function)()) {
	
	//return false if pieces have been consumed.
	
	if (*pieces == 0ULL){ 
		
		//piece_incr = 0;
		return 0;
	}
	
	
	
	int index_p = ffsll(*pieces);  
	
	*piece_incr = *piece_incr + index_p;
	
	*move_squares = 0LL;
	
	
	masking_function(gs, *piece_incr,move_squares, msks, msk_number, gs->color);
	
	*pieces = *pieces >> index_p;
	
	*move_incr = 0;
	
	return 1;
}

/*
 *@returns - boolean, true if there is another valid move to be played.
 Shifts the move incr and move mask to the next valid move
 */
int next_move (GS * gs, int msk_number, 
		uint64_t * msks, uint64_t * pieces, 
		uint64_t * move_squares, 
		int * move_incr) {
	
	if (*move_squares == 0LL){
		
		 *move_incr = 0;
		 return 0;
	}
	int index_m = ffsll(*move_squares);
	*move_incr = *move_incr + index_m;
	*move_squares = *move_squares >> index_m;
	return 1;
}

/*simple MAKE move - should work for knights, kings and some pawn moves?
  NEED TO LEARN HOW TO USE A DEBUGGER.
 */
void make_simple_move (GS * new_gs, GS * gs, uint64_t * selected_pieces, 
						int * move_incr, int * piece_incr) {
	

	uint64_t new_pos = 1LL << (*move_incr - 1);
	
	uint64_t old_pos = 1LL  << (*piece_incr -1);
	normal_game_state_update(new_gs, new_pos, old_pos, selected_pieces);
	
	//print_game_state(new_gs);
	//binary_print_board(new_gs->pawns[color]);
	
	 
}


GS * game_state_generator(GS * gs, GS * new, int * piece_incr,
						int * move_incr, uint64_t * pieces, 
						uint64_t * move_squares, uint64_t * msks,
						uint64_t * selected_pieces, int msk_number, void (* masking_function)){

	int CAN_MOVE = 0;
	if (next_move(gs, msk_number, msks, pieces, move_squares, move_incr)){
		CAN_MOVE = 1;
		
	}
	else if (next_piece(gs, msk_number, msks, pieces,
		 move_squares, piece_incr, move_incr, 
			masking_function) && 
			next_move(gs, msk_number, msks, pieces, move_squares,  move_incr)){
			CAN_MOVE = 1;
			
	}
	else {
		
		//free(new);
		return NULL;
	}

}

//don't need most of these arguments actually?
void game_state_generator_next (GS * gs, GS * new, int * piece_incr,
						int * move_incr, uint64_t * pieces, 
						uint64_t * move_squares, uint64_t * msks,
						uint64_t * selected_pieces, int msk_number){
	
		make_simple_move (new, gs, selected_pieces, 
						move_incr, piece_incr);
		

						}



int game_state_generator_has_next (GS * gs, int * piece_incr, int * move_incr, 
									uint64_t * pieces, uint64_t * move_squares, 
									uint64_t *msks, void (* masking_function)()){
	if (next_move(gs, 0, msks, pieces, move_squares,  move_incr)){
		return 1;
	}
	else { while (next_piece(gs, 0, msks, pieces,
		 move_squares, piece_incr, move_incr, 
			masking_function)) {
			
			if (next_move(gs, 0, msks, pieces, move_squares, move_incr))
				return 1;

		}
		
		return 0;
	}


	
}


/* Piece specific move generators. Lots of reppetion,
// The has_next generators handle generating move squares and shifting move incr.
	They return 0/1 depnding on whether a move is available
// The "next" generators actually update the new game state.
They are all the same except:

* The rook and king generators also have to change the can_castle bits
* 
* The pawn generator_next has to update the enpassant squares
*
*/


int rook_generator_has_next (GS * gs, int * piece_incr, 
					int * move_incr, uint64_t * pieces,
					uint64_t * move_squares, uint64_t * msks){

	if (game_state_generator_has_next(gs, piece_incr, move_incr, pieces, 
							move_squares, msks, rook_masking_function ) )
		return 1;
	return 0;
}

void rook_generator_next (GS * gs, GS * new_gs, 
					int * piece_incr, int * move_incr, 
					uint64_t * pieces, uint64_t * move_squares, 
												uint64_t * msks) {

	uint64_t * selected_pieces = &new_gs->rooks[gs->color];
	game_state_generator_next(gs, new_gs, piece_incr, move_incr, pieces, move_squares,
								msks, selected_pieces, 0);
	//flip castling bits
	int position = *move_incr - 1;
	int old_position = *piece_incr - 1;
	if (position = 0 + gs->color * 56)
		new_gs->castle_queen_side[gs->color] = 0;
	else if (position = 7 + gs->color * 56)
		new_gs->castle_king_side[gs->color] = 0;
	
	char p = (gs->color) ? 'R' : 'r'; 
	new_gs->board_rep[position] = p;
	new_gs->board_rep[old_position] = '_';
}
int pawn_generator_has_next (GS * gs, int * piece_incr, int * move_incr, uint64_t * pieces,
					uint64_t * move_squares, uint64_t * msks){

	if (game_state_generator_has_next(gs, piece_incr, move_incr, pieces, 
							move_squares, msks, pawn_masking_function )  ){
		
		return 1;
							}
	return 0;
}

//also sets enpsassant squares. The squares are set in gs.enpassants[r_color]
void pawn_generator_next (GS * gs, GS * new_gs, int * piece_incr, int * move_incr, uint64_t * pieces,
					uint64_t * move_squares, uint64_t * msks) {

	uint64_t * selected_pieces = &new_gs->pawns[gs->color];
	game_state_generator_next(gs, new_gs, piece_incr, move_incr, pieces, move_squares,
								msks, selected_pieces, 0);

	int position = *move_incr - 1;
	int old_position = *piece_incr - 1;
	char p = (gs->color) ? 'P' : 'p'; 
	new_gs->board_rep[position] = p;
	new_gs->board_rep[old_position] = '_';

	//routine for setting ENPASSANT bits;
	//basically if a pawn moves two squares we just set that as an enpassant square.
	//code commented out would check for legal enpassants, but this is so frustrating
	//that right now we just check at the move generation stage.

	if (abs(old_position - position) == 16){
		
		new_gs->enpassants[new_gs->color] |= (1<<position);
		
		
		//the below code MIGHT be able to check for legal enpassants,
		//but right now we just do this at the move generation stage
		//which is slow, I guess.

		/*
		int color = gs->color;
		int r_color = (color + 1) % 2;
		uint64_t RIGHT = 1LL << (*move_incr);
		uint64_t LEFT = 1LL << (*move_incr - 2);
		int col = (*move_incr-1) & 8;
		if ( col > 0
				&&
			((new_gs->pawns[r_color] | LEFT) == new_gs->pawns[r_color]))

				new_gs->enpassants[r_color] |= (1<<(*move_incr-1));
			
		else if ( col < 7
				&&
			((new_gs->pawns[r_color] | RIGHT) == new_gs->pawns[r_color]))

				new_gs->enpassants[r_color] |= (1<<(*move_incr-1));
		*/		


	} 

	//also check for PROMOTION.
	// at this stage we will only ever award queens.
	else if ( (position / 8) == 7 * new_gs->color){
		printf("Shiiiiiiit\n");
		//destroy this pawn
	//	new_gs->pawns[gs->color] &= ~ (1LL << (position));
	//	int multiplier = (gs->color) ? -1 : 1;
	//	// always award a queen. Only give 8 because we lose a pawn in the process.
	//	new_gs->score += multiplier * 8;
	//	new_gs->queens[gs->color] |= (1LL << position);
	}


}

int knight_generator_has_next (GS * gs, int * piece_incr, int * move_incr, uint64_t * pieces,
					uint64_t * move_squares, uint64_t * msks){

	if (game_state_generator_has_next(gs, piece_incr, move_incr, pieces, 
							move_squares, msks, knight_masking_function ) )
		return 1;


	return 0;
}

void knight_generator_next (GS * gs, GS * new_gs, int * piece_incr, int * move_incr, uint64_t * pieces,
					uint64_t * move_squares, uint64_t * msks) {

	uint64_t * selected_pieces = &new_gs->knights[gs->color];
	game_state_generator_next(gs, new_gs, piece_incr, move_incr, pieces, move_squares,
								msks, selected_pieces, 0);
	int position = *move_incr - 1;
	int old_position = *piece_incr - 1;
	char p = (gs->color) ? 'H' : 'h'; 
	new_gs->board_rep[position] = p;
	new_gs->board_rep[old_position] = '_';
}


int bishop_generator_has_next (GS * gs, int * piece_incr, int * move_incr, uint64_t * pieces,
					uint64_t * move_squares, uint64_t * msks){

	if (game_state_generator_has_next(gs, piece_incr, move_incr, pieces, 
							move_squares, msks, bishop_masking_function ) ) 
		return 1;
	return 0;
}

void bishop_generator_next (GS * gs, GS * new_gs, int * piece_incr, int * move_incr, uint64_t * pieces,
					uint64_t * move_squares, uint64_t * msks) {

	uint64_t * selected_pieces = &new_gs->bishops[gs->color];

	game_state_generator_next(gs, new_gs, piece_incr, move_incr, pieces, move_squares,
								msks, selected_pieces, 0);
	int position = *move_incr - 1;
	int old_position = *piece_incr - 1;
	char p = (gs->color) ? 'B' : 'b'; 
	new_gs->board_rep[position] = p;
	new_gs->board_rep[old_position] = '_';
}

int queen_generator_has_next (GS * gs, int * piece_incr, int * move_incr, uint64_t * pieces,
					uint64_t * move_squares, uint64_t * msks){

	if (game_state_generator_has_next(gs, piece_incr, move_incr, pieces, 
							move_squares, msks, queen_masking_function ) )
		return 1;
	return 0;
}

void queen_generator_next (GS * gs, GS * new_gs, int * piece_incr, int * move_incr, uint64_t * pieces,
					uint64_t * move_squares, uint64_t * msks) {

	uint64_t * selected_pieces = &new_gs->queens[gs->color];
	game_state_generator_next(gs, new_gs, piece_incr, move_incr, pieces, move_squares,
								msks, selected_pieces, 0);
	int position = *move_incr - 1;
	int old_position = *piece_incr - 1;
	char p = (gs->color) ? 'Q' : 'q'; 
	new_gs->board_rep[position] = p;
	new_gs->board_rep[old_position] = '_';
}

int king_generator_has_next (GS * gs, int * piece_incr, int * move_incr, uint64_t * pieces,
					uint64_t * move_squares, uint64_t * msks){

	if (game_state_generator_has_next(gs, piece_incr, move_incr, pieces, 
							move_squares, msks, king_masking_function ) )
		return 1;
	return 0;
}

void king_generator_next (GS * gs, GS * new_gs, int * piece_incr, int * move_incr, uint64_t * pieces,
					uint64_t * move_squares, uint64_t * msks) {

	uint64_t * selected_pieces = &new_gs->kings[gs->color];
	game_state_generator_next(gs, new_gs, piece_incr, move_incr, pieces, move_squares,
								msks, selected_pieces, 0);
	//flip castling bits
	new_gs->castle_king_side[gs->color] = 0;
	new_gs->castle_queen_side[gs->color] = 0;
	int position = *move_incr - 1;
	int old_position = *piece_incr - 1;
	char p = (gs->color) ? 'K' : 'k'; 
	new_gs->board_rep[position] = p;
	new_gs->board_rep[old_position] = '_';
}




/*
	Redundant in the main game loop, and only currently used for the move_testing suite
	 and probably, possibly, the masking functions. 
	Have to update that to use the ones above instead.


*/






