#include "move_arithmetic.c"
/*This is getting unpleasant.
 * How to ensure the proper part of the game state is selected?
 * Pass it as a pointer?
 */
GS * generic_move_generator(GS * gs, uint64_t * msks, uint64_t * move_squares, 
		uint64_t * pieces, uint64_t * piece_incr, uint64_t * move_incr, uint64_t * selected_pieces) {
	int color = gs->color;
	while (next_piece(gs, 4, msks, &pieces,
			&move_squares, &piece_incr, &move_incr,
			knight_king_masking_function)){

		while (next_move(gs, 4, msks, &pieces,
				&move_squares, &piece_incr,
				&move_incr)){
			
			GS * new_gs = copy_game_state(gs);
			uint64_t * selected_pieces = &new_gs->knights[color];
			make_simple_move(new_gs, selected_pieces, 
					move_incr, piece_incr);
			//print_game_state(new_gs);
			free(new_gs);
			return new_gs;
				}	}
	return NULL;
}
void test_loop (GS * gs, uint64_t * msks) {
	int color = gs->color;
	uint64_t move_squares = 0LL;
	uint64_t knights = gs->knights[color];
	uint64_t piece_incr = 0;
	uint64_t move_incr = 0;
	//knights - How can this be made into a generic method?
	while (next_piece(gs, 4, msks, &knights,
			&move_squares, &piece_incr, &move_incr,
			knight_king_masking_function)){
		printf("msk--->");
		binary_print_board(move_squares);
		while (next_move(gs, 4, msks, &knights,
				&move_squares, &piece_incr,
				&move_incr)){
			
			GS * new_gs = copy_game_state(gs);
			printf("the game state copy:");
			print_game_state(new_gs);
			uint64_t * selected_pieces = &gs->knights[color];
			make_simple_move(new_gs, selected_pieces, 
					move_incr, piece_incr);
			//print_game_state(new_gs);
			free(new_gs);
				}
		
		
	}
	move_squares = 0LL;
	uint64_t bishops =gs->bishops[color];
	piece_incr = 0LL;
	move_incr = 0LL;
	/*
	while (next_piece(gs, 0, msks, &bishops,
		&move_squares, &piece_incr, &move_incr,
		bishop_masking_function)){

		while (next_move(gs, 4, msks, &bishops,
			&move_squares, &piece_incr,
			&move_incr)){
			
			GS * new_gs = copy_game_state(gs);
			uint64_t * selected_pieces = &gs->bishops[color];
			make_simple_move(new_gs, selected_pieces, 
					move_incr, piece_incr);
			//print_game_state(new_gs);
			free(new_gs);
				}
		
		
	}
	*/

}

int main () {
	uint64_t * msks = build_mask_object();
	for (int i=0; i<1; i++){
		GS * gs = initial_game_state();
		test_loop (gs, msks);
		free(gs);}
		

}
