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
	uint64_t index = __builtin_clzll(msk) + 1;
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

void knight_masking_function (GS * gs, uint64_t * piece_incr,
	uint64_t* move_squares, uint64_t * msks, uint64_t msk_number, uint64_t color) {

		knight_king_masking_function (gs, piece_incr, move_squares, msks, KNIGHTMINDEX, color);

	}
void king_masking_function(GS * gs, uint64_t * piece_incr,
	uint64_t* move_squares, uint64_t * msks, uint64_t msk_number, uint64_t color){

		knight_king_masking_function (gs, piece_incr, move_squares, msks, KINGMINDEX, color);
	}
/* Pawn attack masking function.
Same as above, but ands the base mask with pieces of the other color.
*/


void pawn_attack_masking_function (GS * gs, uint64_t * piece_incr,
	uint64_t* move_squares, uint64_t * msks, uint64_t msk_number, uint64_t color) {

	*move_squares |= msks[(*piece_incr - 1) * 14 + PAWNATINDEX + color] & gs->pieces[~(gs->color)];

}
/* Masking function for pawn forward moves.
* Slightly more complicated. Inefficient. 
* Does not handle pawn promotion.
*/
void pawn_forward_masking_function (GS * gs, uint64_t * piece_incr,
	uint64_t* move_squares, uint64_t * msks, uint64_t msk_number, uint64_t color){
	
	uint64_t msk = msks[(*piece_incr-1)*14 + PAWNMVINDEX + color];
	//binary_print_board(msk);
	//determine which squares can be moved to in the mask.
	uint64_t temp_move_squares = msk & ~gs->all_pieces;
	//find the first valid square
	int index;
	if(color==0) index = ffsll(temp_move_squares);
	else index = highest_significant_bit_index(temp_move_squares);
	//check that the first free square is directly in front. If it isn't, we set moves squares
	// to 0, as the possible double pawn move won't be valid either.
	if (index!= 0 && abs(*piece_incr - index) != 8)
		return ;
	else 
		*move_squares |= temp_move_squares;
	

									}
void pawn_masking_function (GS * gs, uint64_t * piece_incr,
	uint64_t* move_squares, uint64_t * msks, uint64_t msk_number, uint64_t color){
		pawn_attack_masking_function(gs, piece_incr, move_squares, msks, msk_number, color);
		pawn_forward_masking_function(gs, piece_incr, move_squares, msks, msk_number, color);

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
	
	*move_squares = *move_squares | sliding_ray_hsb_masking(12LL, msks, gs, color, piece_incr);
	*move_squares = *move_squares | sliding_ray_hsb_masking(13LL, msks, gs, color, piece_incr);
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