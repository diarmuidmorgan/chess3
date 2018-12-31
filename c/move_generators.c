#include "moves_base.c"

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
*
*/
int next_piece(GS * gs, int msk_number, 
		uint64_t * msks, uint64_t * pieces, 
		uint64_t * move_squares, int * piece_incr, 
		int * move_incr, void (*masking_function)()) {
	
	//return false if pieces have been consumed.
	if (*pieces == 0LL){ 
		piece_incr = 0;
		return 0;
	}
	
	int index_p = ffsll(*pieces);  
	*piece_incr += index_p;
	
	*move_squares = 0LL;
	
	masking_function(gs, piece_incr,move_squares, msks, msk_number,(uint64_t) gs->color);
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
		uint64_t * move_squares, int * piece_incr, 
		int * move_incr) {
	
	if (*move_squares == 0LL){
		 *move_incr = 0;
		 return 0;
	}
	int index_m = ffsll(*move_squares);
	*move_incr += index_m;
	*move_squares = *move_squares >> index_m;
	return 1;
}

/*simple move - should work for knights, kings and some pawn moves?
 */
void make_simple_move (GS * new_gs, uint64_t * selected_pieces, 
						int move_incr, int piece_incr) {
	
	int color = new_gs->color;
	int r_color = (color + 1) % 2;
	uint64_t new_pos = 1LL << (move_incr - 1);
	
	uint64_t old_pos = 1LL  << (piece_incr -1);
	
	new_gs->pieces[color] = (new_gs->pieces[color] ^ old_pos) | new_pos;
	
	selected_pieces[color] = (selected_pieces[color] ^ old_pos) | new_pos;

	new_gs ->pieces[r_color] = new_gs->pieces[r_color] & (~new_pos);
	flip_game_state(new_gs, r_color);
	 
}


GS * game_state_generator(GS * gs, GS * new, int * piece_incr,
						int * move_incr, uint64_t * pieces, 
						uint64_t * move_squares, uint64_t * msks,
						uint64_t * selected_pieces, int msk_number, void (* masking_function)){

	int CAN_MOVE = 0;
	if (next_move(gs, msk_number, msks, pieces, move_squares, piece_incr, move_incr)){
		CAN_MOVE = 1;
		
	}
	else if (next_piece(gs, msk_number, msks, pieces,
		 move_squares, piece_incr, move_incr, 
			masking_function) && 
			next_move(gs, msk_number, msks, pieces, move_squares, piece_incr, move_incr)){
			CAN_MOVE = 1;
			
	}
	else {
		
		free(new);
		return NULL;
	}

}

//don't need most of these arguments actually?
void game_state_generator_next (GS * gs, GS * new, int * piece_incr,
						int * move_incr, uint64_t * pieces, 
						uint64_t * move_squares, uint64_t * msks,
						uint64_t * selected_pieces, int msk_number){
	
		make_simple_move (new, selected_pieces, 
						*move_incr, *piece_incr);
		

						}

int game_state_generator_has_next (GS * gs, int * piece_incr, int * move_incr, uint64_t * pieces,
									uint64_t * move_squares, uint64_t *msks, void (* masking_function)()){
	if (next_move(gs, 0, msks, pieces, move_squares, piece_incr, move_incr)){
		return 1;
		}
	else if (next_piece(gs, 0, msks, pieces,
		 move_squares, piece_incr, move_incr, 
			masking_function) && 
			next_move(gs, 0, msks, pieces, move_squares, piece_incr, move_incr)){
				return 1;
			}
	else{
		return 0;

	}
}

int rook_generator_has_next (GS * gs, int * piece_incr, int * move_incr, uint64_t * pieces,
					uint64_t * move_squares, uint64_t * msks){

	if (game_state_generator_has_next(gs, piece_incr, move_incr, pieces, 
							move_squares, msks, rook_masking_function ) )
		return 1;
	return 0;
}

void rook_generator_next (GS * gs, GS * new_gs, int * piece_incr, int * move_incr, uint64_t * pieces,
					uint64_t * move_squares, uint64_t * msks) {

	uint64_t * selected_pieces = &new_gs->rooks[gs->color];
	game_state_generator_next(gs, new_gs, piece_incr, move_incr, pieces, move_squares,
								msks, selected_pieces, 0);
}
int pawn_generator_has_next (GS * gs, int * piece_incr, int * move_incr, uint64_t * pieces,
					uint64_t * move_squares, uint64_t * msks){

	if (game_state_generator_has_next(gs, piece_incr, move_incr, pieces, 
							move_squares, msks, pawn_masking_function )  )
		return 1;
	return 0;
}

void pawn_generator_next (GS * gs, GS * new_gs, int * piece_incr, int * move_incr, uint64_t * pieces,
					uint64_t * move_squares, uint64_t * msks) {

	uint64_t * selected_pieces = &new_gs->pawns[gs->color];
	game_state_generator_next(gs, new_gs, piece_incr, move_incr, pieces, move_squares,
								msks, selected_pieces, 0);
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
}



//OLD

GS * rook_generator(GS * gs, int * piece_incr, int * move_incr, uint64_t * pieces,
					uint64_t * move_squares, uint64_t * msks){
		
		GS * new = copy_game_state(gs);
		uint64_t * selected_pieces = &new->rooks[gs->color];
		new = game_state_generator(gs, new, piece_incr, move_incr, pieces, move_squares, msks,
					selected_pieces, 0, rook_masking_function);
		//set castling bits
		/*
		int color = gs->color;
		if (new->castle_queen_side[color] && *piece_incr == 7 + (56 * color) )
			new->castle_queen_side[color] = 0;
		else if (new->castle_king_side[color] && *piece_incr == 7 + (56 * color) )
			new->castle_king_side[color] = 0;
		*/
		return new;
}

GS * pawn_generator(GS * gs, int * piece_incr, int * move_incr, uint64_t * pieces,
					uint64_t * move_squares, uint64_t * msks){
		
		GS * new = copy_game_state(gs);
		uint64_t * selected_pieces = &new->pawns[gs->color];
		new = game_state_generator(gs, new, piece_incr, move_incr, pieces, move_squares, msks,
					selected_pieces, 0, pawn_masking_function);
		//enpassant rule here
		// actually this below rule won't work at all.
		/*
		int color = gs->color;
		if ((color == 0 && (*move_incr < 48 && *move_incr > 40 ))
			|| color == 1 && (*move_incr > 8 && *move_incr < 16))){
			new->enpassants[color] = 1LL << (*move_incr - 1);

		}
		*/
		return new;
	}
GS * knight_generator(GS * gs, int * piece_incr, int * move_incr, uint64_t * pieces,
					uint64_t * move_squares, uint64_t * msks){
		
		
		GS * new = copy_game_state(gs);
		uint64_t * selected_pieces = &new->knights[gs->color];
		new = game_state_generator(gs, new, piece_incr, move_incr, pieces, move_squares, msks,
					selected_pieces, 0, knight_masking_function);
		return new;
	}
GS * king_generator(GS * gs, int * piece_incr, int * move_incr, uint64_t * pieces,
					uint64_t * move_squares, uint64_t * msks){
		
		
		GS * new = copy_game_state(gs);
		uint64_t * selected_pieces = &new->kings[gs->color];
		new = game_state_generator(gs, new, piece_incr, move_incr, pieces, move_squares, msks,
					selected_pieces, 0, king_masking_function);
		if (new){
		int color = gs->color;
		//handle castling
		new->castle_king_side[color] = 0;
		new->castle_king_side[color] = 0;
		}
		return new;
	}
GS * queen_generator(GS * gs, int * piece_incr, int * move_incr, uint64_t * pieces,
					uint64_t * move_squares, uint64_t * msks){
		
		
		GS * new = copy_game_state(gs);
		uint64_t * selected_pieces = &new->queens[gs->color];
		new = game_state_generator(gs, new, piece_incr, move_incr, pieces, move_squares, msks,
					selected_pieces, 0, queen_masking_function);
		return new;
	}
GS * bishop_generator(GS * gs, int * piece_incr, int * move_incr, uint64_t * pieces,
					uint64_t * move_squares, uint64_t * msks){
		
		
		GS * new = copy_game_state(gs);
		uint64_t * selected_pieces = &new->bishops[gs->color];
		new = game_state_generator(gs, new, piece_incr, move_incr, pieces, move_squares, msks,
					selected_pieces, 0, bishop_masking_function);
		return new;
	}




