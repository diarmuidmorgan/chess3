#include "move_masks.c"
#include <stdio.h>


/* Basic functions for applying the generated move masks and finding the pseudo-legal 
* move squares for each piece.
*
* These are kind of subpar implementations 'classical' bit board methods 
*   described in the chessprogramming wiki
 */


/* These two functions handle masking out moves for each 'ray' of a sliding piece. They work for,
* both diagonals and columns/rows. The difference is that we have to use a different bitsearch function
 depending on whether the ray is pointing higher or lower than the piece in the board 
  (higher in terms of the i*8 + j number that indexes the square). 
  Code could be refactored (I think) into one main second order function, and two small functions that just 
  call it, passing in the proper bitsearch function.

  ffsll() is used for lowest significant bit, while __builtin_clzll() is used for highest. This seems to be
  a lot slower than ffsll() and I wonder if it is the right instruction. x86 architectures supposedly have
  built in instructions for both of these.

*/
uint64_t sliding_ray_lsb_masking (int INDEX, uint64_t * msks, GS * gs, int color, int piece_incr) {
	uint64_t base_msk = msks[(piece_incr -1) * 14 + INDEX];
	uint64_t msk = base_msk  & gs->all_pieces;
	
	//if there are no blocking pieces, just return the base msk
	if(msk == 0LL) return base_msk;
	//get the first set bit in the mask

	//this was the wrong way around for two days and I only just noticed :()
	//bit search reverse - this function should be renamed hsb_masking.
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
/*Same as above, Only the bitsearch function differs.
 *
 */
uint64_t sliding_ray_hsb_masking (int INDEX, uint64_t * msks, GS * gs, int color, int piece_incr) {
	uint64_t base_msk = msks[(piece_incr -1) * 14 + INDEX];
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
 Works for both knights and kings

*/
void knight_king_masking_function (GS * gs, int  piece_incr,
	uint64_t* move_squares, uint64_t * msks, int msk_number, int color) {

	*move_squares = msks[(piece_incr - 1) * 14 + msk_number] & ( ~ gs->pieces[gs->color]);
	

}
//implementations of the above function, passing in the right mask INDEX

void knight_masking_function (GS * gs, int piece_incr,
	uint64_t* move_squares, uint64_t * msks, int msk_number, int color) {

		knight_king_masking_function (gs, piece_incr, move_squares, msks, KNIGHTMINDEX, color);
		

	}
void king_masking_function(GS * gs, int  piece_incr,
	uint64_t* move_squares, uint64_t * msks, int msk_number, int color){

		knight_king_masking_function (gs, piece_incr, move_squares, msks, KINGMINDEX, color);
		
	}


/* Pawn attack masking function.
Same as above, but ands the base mask with pieces of the other color.
*/


void pawn_attack_masking_function (GS * gs, int piece_incr,
	uint64_t* move_squares, uint64_t * msks, int msk_number, int color) {
	int r_color = (color + 1) % 2;
	//printf("OPPOSITE PIECES\n");
	//binary_print_board(gs->pieces[r_color]);
	//binary_print_board( 1LL << (*piece_incr -1));
	//binary_print_board(msks[(*piece_incr - 1) * 14 + PAWNATINDEX + color]);
	*move_squares |= (msks[(piece_incr - 1) * 14 + PAWNATINDEX + color] & gs->pieces[r_color]);
	

}
/* Masking function for pawn forward moves.
* Slightly more complicated. Inefficient, I think. Relies on bitscanforward/reverse depending on
	direction.
* Does not handle pawn promotion.

* I think this is fundamentally broken.
*/
void pawn_forward_masking_function (GS * gs, int piece_incr,
	uint64_t* move_squares, uint64_t * msks, int msk_number, int color){
	
	uint64_t msk = msks[(piece_incr-1)*14 + PAWNMVINDEX + color];
	//binary_print_board(msk);
	//determine which squares can be moved to in the mask.
	uint64_t temp_move_squares = msk & ~gs->all_pieces;
	//find the first valid square
	int index;
	if(color==0) index = ffsll(temp_move_squares);
	else index = bitscanreverse(temp_move_squares);
	//check that the first free square is directly in front. If it isn't, we set moves squares
	// to 0, as the possible double pawn move won't be valid either.
	if (index!= 0 && abs(piece_incr - index) != 8)
		return;
	else 
		*move_squares |= temp_move_squares;
		
	
//just calls both of the above masking functions.
									}
void pawn_masking_function (GS * gs, int piece_incr,
	uint64_t* move_squares, uint64_t * msks, int msk_number, int color){
		
	pawn_attack_masking_function(gs, piece_incr, move_squares, msks, msk_number, color);
	pawn_forward_masking_function(gs, piece_incr, move_squares, msks, msk_number, color);
	
	}

//implements the sliding ray functions for bishops, rooks and queens.

void bishop_masking_function (GS * gs, int piece_incr, uint64_t * move_squares, uint64_t * msks, int msk_number, int color){
	

	*move_squares |= sliding_ray_lsb_masking(DIAGULINDEX, msks, gs, color, piece_incr);
	*move_squares |= sliding_ray_lsb_masking(DIAGURINDEX, msks, gs, color, piece_incr);
	*move_squares |= sliding_ray_hsb_masking(DIAGDLINDEX, msks, gs, color, piece_incr);
	*move_squares |= sliding_ray_hsb_masking(DIAGDRINDEX, msks, gs, color, piece_incr);
	
}

void rook_masking_function (GS * gs, int piece_incr, uint64_t * move_squares, uint64_t * msks, int msk_number, int color){
	
	*move_squares |= sliding_ray_lsb_masking(6, msks, gs,color,piece_incr);
	*move_squares |= sliding_ray_lsb_masking(7, msks, gs, color, piece_incr);
	*move_squares |= sliding_ray_hsb_masking(8, msks, gs, color, piece_incr);
	*move_squares |= sliding_ray_hsb_masking(9, msks, gs, color, piece_incr);
	
}

void queen_masking_function (GS * gs, int  piece_incr, uint64_t * move_squares, uint64_t * msks, int msk_number, int color){
	
	
	bishop_masking_function(gs, piece_incr,
				move_squares, msks,
				msk_number, color);
	rook_masking_function(gs, piece_incr, 
				move_squares, msks,
				msk_number, color);
	

}