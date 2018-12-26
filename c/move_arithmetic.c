#include "move_masks.c"
#include <stdio.h>
/* Boolean - returns if the generator is still valid.
 * Handles incrementing the piece position counter, picking the new move msk,  and shifting the piece array
 */
uint64_t sliding_ray_lsb_masking (int INDEX, uint64_t * msks, GS * gs, int color, uint64_t * piece_incr) {
	
	uint64_t msk = msks[(*piece_incr -1) * 14 + INDEX] & gs->all_pieces;
	if(msk == 0LL) return msks[(*piece_incr-1) * 14 + INDEX];
	uint64_t index = ffsll(msk);
	// handles the case where the first piece is of the same color
	if ( (gs->pieces[color] & (1LL << (index - 1) )) == gs->pieces[color]) 
		return msks[(*piece_incr -1) * 14 + INDEX] ^ (msks[(index -1) * 14 + INDEX] | (1LL << (index -1)));
	// 
	return msks[(*piece_incr - 1) * 14 + INDEX] ^ (msks[(index -1) * 14 + INDEX]);

}
/*Same as above, only we use the super slow highest_significant_bit() function to determine the blocking piece
 *
 */
uint64_t sliding_ray_hsb_masking (int INDEX, uint64_t * msks, GS * gs, int color, uint64_t * piece_incr) {
	
	uint64_t msk = msks[(*piece_incr -1) * 14 + INDEX] & gs->all_pieces;
	if(msk == 0LL) return msks[(*piece_incr-1) * 14 + INDEX];
	// this shit here --> this can be a maximum of 14??  uint64_t calculations.
	uint64_t index = highest_significant_bit_index(msk);
	// handles the case where the first piece is of the same color
	if ( (gs->pieces[color] | (1LL << (index - 1) )) == gs->pieces[color]) 
		return msks[(*piece_incr -1) * 14 + INDEX] ^ (msks[(index -1) * 14 + INDEX] | (1LL << (index -1)));
	// 
	return msks[(*piece_incr - 1) * 14 + INDEX] ^ (msks[(index -1) * 14 + INDEX]);

}
void knight_king_masking_function (GS * gs, uint64_t * move_squares, 
		uint64_t * msks, uint64_t msk_number, 
		uint64_t * piece_incr, uint64_t color) {

	*move_squares = msks[(*piece_incr - 1) * 14 + msk_number] & ( ~ gs->pieces[gs->color]);

}

void pawn_attack_masking_function (GS * gs, uint64_t * move_squares, 
		uint64_t * msks, uint64_t msk_number, 
		uint64_t * piece_incr, uint64_t color) {

	*move_squares = msks[(*piece_incr - 1) * 14 + msk_number] & gs->pieces[~(gs->color)];

}
/* This - alot more fucking complicated ...
 *
 * SLIDING PIECES!
 */
void bishop_masking_function (GS * gs, uint64_t * piece_incr, uint64_t * move_squares, uint64_t * msks, uint64_t msk_number, uint64_t color){
	
	
	*move_squares = *move_squares | sliding_ray_lsb_masking(10, msks, gs, color, piece_incr);
	*move_squares = *move_squares | sliding_ray_lsb_masking(11, msks, gs, color, piece_incr);
	*move_squares = *move_squares | sliding_ray_hsb_masking(12, msks, gs, color, piece_incr);
	*move_squares = *move_squares | sliding_ray_hsb_masking(13, msks, gs, color, piece_incr);
}

void rook_masking_function (GS * gs, uint64_t * piece_incr, uint64_t * move_squares, uint64_t * msks, uint64_t msk_number, uint64_t color){
		*move_squares = *move_squares | sliding_ray_lsb_masking(6, msks, gs,color,piece_incr);
	*move_squares = *move_squares | sliding_ray_lsb_masking(8, msks, gs, color, piece_incr);
	*move_squares = *move_squares | sliding_ray_hsb_masking(7, msks, gs, color, piece_incr);
	*move_squares = *move_squares | sliding_ray_hsb_masking(9, msks, gs, color, piece_incr);
}

void queen_masking_function (GS * gs, uint64_t * piece_incr, uint64_t * move_squares, uint64_t * msks, uint64_t msk_number, uint64_t color){
	
	
	bishop_masking_function(gs, piece_incr,
				move_squares, msks,
				msk_number, color);
	rook_masking_function(gs, piece_incr, 
				move_squares, msks,
				msk_number, color);
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
	masking_function(move_squares, msks, msk_number, gs->color);
	*pieces = *pieces >> index_p;
	*move_incr = 0;
	return 1;
}
/*
 *@returns - boolean, true if there is another valid move to be played.
 */
int next_move (GS * gs, int msk_number, 
		uint64_t * msks, uint64_t * pieces, 
		uint64_t * move_squares, uint64_t * piece_incr, 
		uint64_t * move_incr) {

	if (*move_squares == 0LL) return 0;
	uint64_t index_m = ffsll(*move_squares);
	*move_incr += index_m;
	*move_squares = *move_squares >> index_m;
}
/*simple move - should work for knights, kings and some pawn moves?
 */
void make_simple_move (GS * new_gs, uint64_t * selected_pieces, uint64_t move_incr, uint64_t piece_incr) {
	int color = new_gs->color;
	uint64_t new_pos = 1LL << (move_incr - 1);
	uint64_t old_pos = 1LL  << (piece_incr -1);
	//GS * new_gs = copy_game_state(gs);
	new_gs->pieces[color] = (new_gs->pieces[color] ^ old_pos) | new_pos;
	//this bit could also be made into a higher order function?????
	*selected_pieces = (*selected_pieces ^ old_pos) | new_pos;
	new_gs->pieces[~color] = new_gs->pieces[~color] & (~ new_pos); 

}

/*
int main () {
	printf("WORKING");
	GS * gs = initial_game_state();
	uint64_t knights = gs->knights[0];
	binary_print_board(knights);
	uint64_t move_squares = 0LL;
	uint64_t piece_incr = 0LL;
	uint64_t move_incr = 0LL;
	uint64_t * msks = build_mask_object();
	//print_game_state(gs);
	//print_game_state(gs);
	GS * new = knight_move(gs,msks,&knights,&move_squares,
				&piece_incr, &move_incr);
       	while (new != NULL){
		printf("%" PRIu64 "\n", move_squares);
		print_game_state(new);
		binary_print_board(move_squares);
		free(new);
		
		printf("\n");
		new = knight_move(gs, msks, &knights, &move_squares, &piece_incr, &move_incr);
		
	}	
	return 0;

} */
