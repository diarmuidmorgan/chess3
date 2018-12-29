#include "moves_base.c"

int cycle_pieces (GS * gs, uint64_t * pieces, int * piece_incr){
	if(*pieces==0LL) return 0;
	int index_p = ffsll(*pieces);
	*piece_incr += index_p;
	*pieces = *pieces >> index_p;
	return 1;
}
int next_piece(GS * gs, int msk_number, 
		uint64_t * msks, uint64_t * pieces, 
		uint64_t * move_squares, int * piece_incr, 
		int * move_incr, void (*masking_function)()) {
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
		uint64_t * move_squares, int * piece_incr, 
		int * move_incr) {
	
	if (*move_squares == 0LL) return 0;
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
	//printf("about to make this move _____>");
	//binary_print_board(new_pos);
	//binary_print_board(old_pos);
	//GS * new_gs = copy_game_state(gs);
	new_gs->pieces[color] = (new_gs->pieces[color] ^ old_pos) | new_pos;
	//binary_print_board(new_gs->pieces[color]);
	//this bit could also be made into a higher order function?????
	//printf("the old piece positions");
	//binary_print_board(selected_pieces[color]);
	//printf("the new piece positions");
	selected_pieces[color] = (selected_pieces[color] ^ old_pos) | new_pos;

	// here we have to update the other color's "all pieces variable"
	new_gs ->pieces[r_color] = new_gs->pieces[r_color] & (~new_pos);
	flip_game_state(new_gs, r_color);
	//binary_print_board(selected_pieces[color]);
	//printf("reading game state");
	//binary_print_board(new_gs->knights[0]);
	//new_gs->pieces[~color] = new_gs->pieces[~color] & (~ new_pos); 
	return;
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
	if (CAN_MOVE){
			
			//binary_print_board(1LL << (*move_incr - 1));
			make_simple_move(new, selected_pieces,*move_incr, *piece_incr);
			
			return new;
	}
	else {
		
		*piece_incr = 0;
		*move_incr = 0;
		free(new);
		return NULL;
	}

						}


/*
void generation_loop (GS * gs){
	
	uint64_t * msks = build_mask_object();
	uint64_t piece_incr = 0LL;
	uint64_t move_incr = 0LL;
	uint64_t pieces = 0LL;
	uint64_t move_squares;
	int msk_number;
	piece_incr = 0LL;
	move_incr = 0LL;
	pieces = gs->pawns[0];
	move_squares = 0LL;
	printf("\n\n\n\n\n\n");
	msk_number = PAWNMVINDEX+1;
	while (next_piece(gs, msk_number, msks,&pieces,
		&move_squares,&piece_incr, &move_incr, 
			pawn_masking_function)){
			//binary_print_board(move_squares);
			move_incr = 0;

		while (next_move(gs, msk_number, msks, &pieces, &move_squares, &piece_incr, &move_incr)){
			//binary_print_board(1LL << (move_incr -1));
			GS * new = copy_game_state(gs);
			make_simple_move(new, &new->pawns[0],move_incr, piece_incr);
			//print_game_state(new);
			free(new);
		}
			}

	
}

/* Really its like we have to go and write a ton of manual test cases.
	Board arrays, and what their move matching should look like?
*	Seems like incredibly dull and difficult work?
	How did I do it in python? Just parsing all of those games and checking
	the engine didn't crash?
/
void test_loop(GS * gs, int TEST_PAWNATTACKS, int TEST_PAWNMOVES, int TEST_KNIGHTS){
	uint64_t * msks = build_mask_object();
	print_game_state(gs);
	
	uint64_t piece_incr = 0LL;
	uint64_t move_incr = 0LL;
	uint64_t pieces = 0LL;
	uint64_t move_squares;
	int msk_number;
	if (TEST_PAWNMOVES){
		//I think this bit works, for starting board at least.
	printf("PAWN FORWARDS TEST ---> ");
	piece_incr = 0LL;
	move_incr = 0LL;
	pieces = gs->pawns[0];
	move_squares = 0LL;
	printf("\n\n\n\n\n\n");
	msk_number = PAWNMVINDEX+1;
	
	while (next_piece(gs, msk_number, msks,&pieces,
		&move_squares,&piece_incr, &move_incr, 
			pawn_forward_masking_function))
			binary_print_board(move_squares);
	}
	
	// this works
	if (TEST_KNIGHTS){
	printf("KNIGHTS TEST ----> ");
	piece_incr = 0LL;
	move_incr = 0LL;
	pieces = gs->knights[0];
	move_squares = 0LL;
	msk_number = KNIGHTMINDEX;
	while (next_piece(gs, msk_number, msks,&pieces,
		&move_squares,&piece_incr, &move_incr, 
			knight_king_masking_function))
			binary_print_board(move_squares);
	}
}

*/