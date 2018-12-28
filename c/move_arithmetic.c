#include "move_masks.c"
#include <stdio.h>



/* The things is, we really need a 'checksquares' kind of mask in order to do this.
* Damn, better build that first?
* How do we build that though, without generating all possible moves first?

* A good question here is --> Do we need an attack/check squares table? Can we just generate it on the
* off chance that castling is actually valid? Which won't be most of the time?
*/
int castling_kingside_function (GS * gs, uint64_t * castle_masks){

	if (!gs->castle_king_side[gs->color]) return 0;
	if (castle_masks[gs->color] == castle_masks[gs->color] & ~gs->all_pieces){
		uint64_t check_squares = generate_check_squares(gs, ~gs->color);
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

/* GETTING CHECK SQUARES AND PIN SQUARES TO DETERMINE VALID CASTLING. 


Problem here is we have to know which pieces are pinned first?
 This seems horrendously expensive. Though we would only have to do it if castling is already valid,
 except for passing through attacked squares. Which is probably only true in a fraction of the moves?
 We would also use the two below methods for parsing Game notation, important in the (seemingly less
 and less possible) ML stage.
*/
uint64_t generate_pin_masks(GS * gs, 
						uint64_t * msks){

	int color =  -1 * gs->color;
	int rcolor=gs->color;
	uint64_t pin_mask = 0LL;
	//for bishops, queens and rooks
	// select each potential ray
	// determine whether a king of opposing color is in that ray
	// determine number of pieces blocking the ray
	//
	uint64_t pieces = gs->bishops[color];
	int msk_numbers1[4] = {10,11,12,13};
	int msk_numbers2[4] = msk_numbers = {10,11,12,13};
	add_to_pin_mask(pieces, &pin_mask, gs, &msk_numbers1, msks,color,rcolor);
	pieces = gs->rooks[color];
	
	add_to_pin_mask(pieces, &pin_mask, gs, &msk_numbers2, msks,color,rcolor);
	pieces = gs->queens[color];
	add_to_pin_mask(pieces, &pin_mask, gs, &msk_numbers2, msks,color,rcolor);
	pieces = gs->queens[color];
	
	add_to_pin_mask(pieces, &pin_mask, gs, &msk_numbers1, msks,color,rcolor);
	return pin_mask; 
}
void add_to_pin_mask (uint64_t pieces, uint64_t * pin_mask, 
					GS * gs, int * msk_numbers, int * msks, int color, int rcolor){
	uint64_t piece_incr = 0LL;
	while(cycle_pieces(gs,&pieces,&piece_incr)) {
		uint64_t ray = msks[(piece_incr -1) * 14 + 10];
		uint64_t ray_with_king = ray && gs->kings[rcolor];
		for(int i=0; i<2; i ++){
			//return if the king was found inside this ray
			if (add_to_pin_mask_update(ffsll, piece_incr, color, rcolor, msk_numbers[i]))
				break;
		}
		for (int i=2; i<4; i++){
			if (add_to_pin_mask_update(highest_significant_bit_index, piece_incr, color, rcolor, msk_numbers[i]))
				break;

		}
	}
}
int add_to_pin_mask_update( int (* bit_find_func)(), uint64_t piece_incr, 
							int color, int rcolor, int msk_number){
	uint64_t ray = msks[(piece_incr -1) * 14 + msk_number];
	uint64_t ray_with_king = ray && gs->kings[rcolor];
	if (ray_with_king == ray){
			uint64_t pieces_inbetween = ray_with_king & gs->all_pieces & (~gs->kings[rcolor]);
			int first = func(pieces_inbetween);
			if (first!=0){
				int count = 1;
				while (pieces_inbetween != 0LL && count == 1){
					pieces_inbetween = pieces_inbetween >> bit_find_func(pieces_inbetween);
					count ++;
				}
				if (count == 1 && (gs->pieces[rcolor] & (1LL << (first-1) == gs->pieces[rcolor] ))
					pin_mask |= 1LL << first;

			}
			return 1;
		}
	return 0;
}

void add_attack_squares (GS * gs, uint64_t * attack_squares, int msk_number, uint64_t pieces, void (* masking_function)()){
	uint64_t piece_incr = 0LL;
	uint64_t move_squares = 0LL;
	uint64_t move_incr = 0LL;
	while (next_piece(gs, msk_number, msks, &pieces, &move_squares, &piece_incr, &move_incr, masking_function)){

		*attack_squares |= move_squares;
	}


}

uint64_t generate_attack_squares(GS * gs, int color, uint64_t * msks){

	uint64_t attack_squares = 0LL;
	uint64_t pieces = gs->pawns[color];
	add_attack_squares(&attack_squares, 2, pieces, pawn_attack_masking_function);
	pieces = gs->kings[color];
	add_attack_squares(&attack_squares, 3, pieces, knight_king_masking_function);
	pieces = gs->knights[color];
	add_attack_squares(&attack_squares, 3, pieces, knight_king_masking_function);
	pieces = gs->bishops[color];
	add_attack_squares(&attack_squares, 3, pieces, bishop_masking_function);
	pieces = gs->queens[color];
	add_attack_squares(&attack_squares, 3, pieces, queen_masking_function);
	pieces = gs->rooks[color];
	add_attack_squares(&attack_squares, 3, pieces, rook_masking_function);
	return attack_squares;
}
