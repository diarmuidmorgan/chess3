#include "move_masks.c"
#include <stdio.h>


uint64_t sliding_ray_lsb_masking (int INDEX, uint64_t * msks, GS * gs, int color, uint64_t * piece_incr) {
		
	uint64_t msk = msks[(*piece_incr -1) * 14 + INDEX] & gs->all_pieces;
	//printf("this tha mask");
	//binary_print_board(msks[(*piece_incr-1) * 14 + INDEX]);
	if(msk == 0LL) return msks[(*piece_incr-1) * 14 + INDEX];
	uint64_t index = ffsll(msk);
	// handles the case where the first piece is of the same color
	if ( (gs->pieces[color] & (1LL << (index - 1) )) == gs->pieces[color]) 
		return msks[(*piece_incr -1) * 14 + INDEX] ^ (msks[(index -1) * 14 + INDEX] | (1LL << (index -1)));
	// 
	return msks[(*piece_incr - 1) * 14 + INDEX] ^ (msks[(index -1) * 14 + INDEX]);

}
/*Same as above, only we use the super slow highest_significant_bit() function 
to determine the blocking piece
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
	return msks[(*piece_incr - 1) * 14 + INDEX] ^ (msks[(index -1) * 14 + index]);

}
/* Simplest masking function. 
Simply ands the base move mask with the inverse of the player's pieces.
* 

*/
void knight_king_masking_function (GS * gs, uint64_t * piece_incr,
	uint64_t* move_squares, uint64_t * msks, uint64_t msk_number, uint64_t color) {

	*move_squares = msks[(*piece_incr - 1) * 14 + msk_number] & ( ~ gs->pieces[gs->color]);

}
/* Pawn attack masking function.
Same as above, but ands the base mask with pieces of the other color.
*/

void pawn_attack_masking_function (GS * gs, uint64_t * move_squares, 
		uint64_t * msks, uint64_t msk_number, 
		uint64_t * piece_incr, uint64_t color) {

	*move_squares = msks[(*piece_incr - 1) * 14 + msk_number] & gs->pieces[~(gs->color)];

}
/* Masking function for pawn forward moves.
* Slightly more complicated. Inefficient. 
*/
void pawn_forward_masking_function ( GS * gs, uint64_t * move_squares,
									uint64_t * msks, uint64_t msk_number,
									uint64_t * piece_incr, uint64_t color){
	
	uint64_t msk = msks[(*piece_incr-1)*14 + color];
	//determine which squares can be moved to in the mask.
	*move_squares = *move_squares & ~gs->all_pieces;
	//find the first valid square
	int index = ffsll(*move_squares);
	//check that the first free square is directly in front. If it isn't, we set moves squares
	// to 0, as the possible double pawn move won't be valid either.
	if (index!= 0 && abs(*piece_incr - index) != 8)
		*move_squares = 0LL;
	

									}
/* This - alot more fucking complicated ...
 *
 * SLIDING PIECES!
 */
void bishop_masking_function (GS * gs, uint64_t * piece_incr, uint64_t * move_squares, uint64_t * msks, uint64_t msk_number, uint64_t color){
	

	*move_squares = *move_squares | sliding_ray_lsb_masking(10LL, msks, gs, color, piece_incr);
	//binary_print_board(*move_squares);
	//printf("-------");
	*move_squares = *move_squares | sliding_ray_lsb_masking(11LL, msks, gs, color, piece_incr);
	//binary_print_board(*move_squares);
	//printf("---------");
	
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

/* The things is, we really need a 'checksquares' kind of mask in order to do this.
* Damn, better build that first?
* How do we build that though, without generating all possible moves first?

* A good question here is --> Do we need an attack/check squares table? Can we just generate it on the
* off chance that castling is actually valid? Which won't be most of the time?
*/
int castling_kingside_function (GS * gs, uint64_t * castle_masks){

	if (!gs->castle_king_side[gs->color]) return 0;
	if (castle_masks[gs->color] == castle_masks[gs->color] & ~gs->all_pieces){
		uint64_t check_squares = generate_check_squares(~gs->color);
		if (castle_masks[gs->color] == castle_masks[gs->color] & ~check_squares)
			return 1;
	}
	return 0;

}

/* Shift the pieces to the next valid piece. Increments the piece index counter.
* The masking function is passed as a parameter
*/
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
void make_simple_move (GS * new_gs, uint64_t * selected_pieces, uint64_t move_incr, uint64_t piece_incr) {
	
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

/* Problem here is we have to know which pieces are pinned first?
 This seems horrendously expensive. Though we would only have to do it if castling is already valid,
 except for passing through attacked squares. Which is probably only true in a fraction of the moves?
 We would also use the two below methods for parsing Game notation, important in the (seemingly less
 and less possible) ML stage.
*/
uint64_t generate_pin_masks(GS * gs, uint64_t * msks){


}
uint64_t generate_check_squares(GS * gs, int color){

	uint64_t check_squares = 0LL;
	uint64_t move_squares = 0LL;
	next_piec


}
