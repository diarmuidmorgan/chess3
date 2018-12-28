#include "moves_base.c"

int cycle_pieces (GS * gs, uint64_t * pieces, uint64_t * piece_incr){
	if(*pieces==0LL) return 0;
	int index_p = ffsll(*pieces);
	*piece_incr += index_p;
	*pieces = *pieces >> index_p;
	return 1;
}
int next_piece(GS * gs, int msk_number, 
		uint64_t * msks, uint64_t * pieces, 
		uint64_t * move_squares, uint64_t * piece_incr, 
		uint64_t * move_incr, void (*masking_function)()) {
	//return NULL if pieces have been consumed.
	if (*pieces == 0LL) return 0;
	int index_p = ffsll(*pieces);  
	*piece_incr += index_p;
	//replace this with first order function??
	//*move_squares = msks[(*piece_incr - 1) * 14 + msk_number] & gs->pieces[gs->color];
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
		uint64_t * move_squares, uint64_t * piece_incr, 
		uint64_t * move_incr) {

	if (*move_squares == 0LL) return 0;
	uint64_t index_m = ffsll(*move_squares);
	*move_incr += index_m;
	*move_squares = *move_squares >> index_m;
	return 1;
}
/*simple move - should work for knights, kings and some pawn moves?
 */
void make_simple_move (GS * new_gs, uint64_t * selected_pieces, 
						uint64_t move_incr, uint64_t piece_incr) {
	
	int color = new_gs->color;
	uint64_t new_pos = 1LL << (move_incr - 1);
	printf("new pos below.....");
	//binary_print_board(new_pos);
	uint64_t old_pos = 1LL  << (piece_incr -1);
	//GS * new_gs = copy_game_state(gs);
	new_gs->pieces[color] = (new_gs->pieces[color] ^ old_pos) | new_pos;
	//this bit could also be made into a higher order function?????
	*selected_pieces = (*selected_pieces ^ old_pos) | new_pos;
	//new_gs->pieces[~color] = new_gs->pieces[~color] & (~ new_pos); 
	binary_print_board(new_gs->pieces[color]);
	printf("and what the algo thinks is the game state");
	print_game_state(new_gs);
}