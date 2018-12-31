#include "move_masks.c"
#include <stdio.h>

uint64_t sliding_ray_lsb_masking (int INDEX, uint64_t * msks, GS * gs, int color, int * piece_incr) {
	uint64_t base_msk = msks[(*piece_incr -1) * 14 + INDEX];
	uint64_t msk = base_msk  & gs->all_pieces;
	//printf("this tha mask");
	//binary_print_board(msks[(*piece_incr-1) * 14 + INDEX]);

	//if there are no blocking pieces, just return the base msk
	if(msk == 0LL) return base_msk;
	//get the first set bit in the mask

	//this was the wrong way around for two days and I only just fucking noticed :()
	int index =  64 - (__builtin_clzll(msk));
	// handles the case where the first piece is of the same color
	if ( (gs->pieces[color] | (1LL << (index - 1) )) == gs->pieces[color]){ 
		//binary_print_board(base_msk ^ (msks[(index -1) * 14 + INDEX] | (1LL << (index -1))));
		return base_msk ^ (msks[(index -1) * 14 + INDEX] | (1LL << (index -1)));
	}
	// 
	//binary_print_board(base_msk ^ (msks[(index -1) * 14 + INDEX]));
	return base_msk ^ (msks[(index -1) * 14 + INDEX]);

}
/*Same as above, only we use the super slow highest_significant_bit() function 
to determine the blocking piece
 *
 */
uint64_t sliding_ray_hsb_masking (int INDEX, uint64_t * msks, GS * gs, int color, int * piece_incr) {
	uint64_t base_msk = msks[(*piece_incr -1) * 14 + INDEX];
	uint64_t msk = base_msk & gs->all_pieces;
	if(msk == 0LL) return base_msk;
	// this shit here --> this can be a maximum of 14??  uint64_t calculations.
	int index = ffsll(msk);
	// handles the case where the first piece is of the same color
	if ( (gs->pieces[color] | (1LL << (index - 1) )) == gs->pieces[color]) {
		//binary_print_board(base_msk ^ (msks[(index -1) * 14 + INDEX] | (1LL << (index -1))));
		return base_msk ^ (msks[(index -1) * 14 + INDEX] | (1LL << (index -1)));
	}
	// }
	//binary_print_board(base_msk ^ (msks[(index -1) * 14 + INDEX]));
	return base_msk ^ (msks[(index -1) * 14 + INDEX]);
	

}
/* Simplest masking function. 
Simply ands the base move mask with the inverse of the player's pieces.
* 

*/
void knight_king_masking_function (GS * gs, int * piece_incr,
	uint64_t* move_squares, uint64_t * msks, int msk_number, int color) {

	*move_squares = msks[(*piece_incr - 1) * 14 + msk_number] & ( ~ gs->pieces[gs->color]);
	

}

void knight_masking_function (GS * gs, int * piece_incr,
	uint64_t* move_squares, uint64_t * msks, int msk_number, int color) {

		knight_king_masking_function (gs, piece_incr, move_squares, msks, KNIGHTMINDEX, color);
		

	}
void king_masking_function(GS * gs, int * piece_incr,
	uint64_t* move_squares, uint64_t * msks, int msk_number, int color){

		knight_king_masking_function (gs, piece_incr, move_squares, msks, KINGMINDEX, color);
		
	}
/* Pawn attack masking function.
Same as above, but ands the base mask with pieces of the other color.
*/


void pawn_attack_masking_function (GS * gs, int * piece_incr,
	uint64_t* move_squares, uint64_t * msks, int msk_number, int color) {
	int r_color = (color + 1) % 2;
	*move_squares |= msks[(*piece_incr - 1) * 14 + PAWNATINDEX + color] & gs->pieces[r_color];
	

}
/* Masking function for pawn forward moves.
* Slightly more complicated. Inefficient. 
* Does not handle pawn promotion.

* I think this is fundamentally broken.
*/
void pawn_forward_masking_function (GS * gs, int * piece_incr,
	uint64_t* move_squares, uint64_t * msks, int msk_number, int color){
	
	uint64_t msk = msks[(*piece_incr-1)*14 + PAWNMVINDEX + color];
	//binary_print_board(msk);
	//determine which squares can be moved to in the mask.
	uint64_t temp_move_squares = msk & ~gs->all_pieces;
	//find the first valid square
	int index;
	if(color==0) index = ffsll(temp_move_squares);
	else index = bitscanreverse(temp_move_squares);
	//check that the first free square is directly in front. If it isn't, we set moves squares
	// to 0, as the possible double pawn move won't be valid either.
	if (index!= 0 && abs(*piece_incr - index) != 8)
		return ;
	else 
		*move_squares |= temp_move_squares;
		return;
	

									}
void pawn_masking_function (GS * gs, int * piece_incr,
	uint64_t* move_squares, uint64_t * msks, int msk_number, int color){
		pawn_attack_masking_function(gs, piece_incr, move_squares, msks, msk_number, color);
		pawn_forward_masking_function(gs, piece_incr, move_squares, msks, msk_number, color);
	
	}
/* This - alot more fucking complicated ...
 *
 * SLIDING PIECES!
 */
void bishop_masking_function (GS * gs, int * piece_incr, uint64_t * move_squares, uint64_t * msks, int msk_number, int color){
	

	*move_squares |= sliding_ray_lsb_masking(DIAGULINDEX, msks, gs, color, piece_incr);
	//binary_print_board(*move_squares);
	//printf("-------");
	*move_squares |= sliding_ray_lsb_masking(DIAGURINDEX, msks, gs, color, piece_incr);
	//binary_print_board(*move_squares);
	//printf("---------");
	
	*move_squares |= sliding_ray_hsb_masking(DIAGDLINDEX, msks, gs, color, piece_incr);
	*move_squares |= sliding_ray_hsb_masking(DIAGDRINDEX, msks, gs, color, piece_incr);
	return;
}

void rook_masking_function (GS * gs, int * piece_incr, uint64_t * move_squares, uint64_t * msks, int msk_number, int color){
	*move_squares |= sliding_ray_lsb_masking(6, msks, gs,color,piece_incr);
	*move_squares |= sliding_ray_lsb_masking(7, msks, gs, color, piece_incr);
	*move_squares |= sliding_ray_hsb_masking(8, msks, gs, color, piece_incr);
	*move_squares |= sliding_ray_hsb_masking(9, msks, gs, color, piece_incr);
	return;
}

void queen_masking_function (GS * gs, int * piece_incr, uint64_t * move_squares, uint64_t * msks, int msk_number, int color){
	
	
	bishop_masking_function(gs, piece_incr,
				move_squares, msks,
				msk_number, color);
	rook_masking_function(gs, piece_incr, 
				move_squares, msks,
				msk_number, color);
	return;

}