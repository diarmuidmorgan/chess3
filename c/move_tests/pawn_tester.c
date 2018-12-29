#include "../move_generators.c"

int main () {

	GS * gs = read_position_from_file("positions/pawns.chess");
	print_game_state(gs);
	gs->color = 1;
	uint64_t * msks = build_mask_object();
	int piece_incr = 0;
	int move_incr = 0;
	uint64_t pieces = 0LL;
	uint64_t move_squares;
	uint64_t * selected_pieces;
	int msk_number;
	int color = 1;
	pieces = gs->pawns[color];
	
	move_squares = 0LL;
	GS * new = copy_game_state(gs);
	selected_pieces = new->pawns;
	//binary_print_board(gs->knights[0]);
	new = game_state_generator(gs, new, &piece_incr, &move_incr, 
	&pieces, &move_squares, msks, selected_pieces, KNIGHTMINDEX, pawn_maspawn_function);
	char s[1000];
	printf("examine this...");
	binary_print_board(gs->pieces[1]);
	binary_print_board(move_squares);
	if (move_squares & gs->pieces[1] != 0LL )
		printf("FUCKED");
	binary_print_board(move_squares);
	//loop_with_generator(new,depth,positions_considered);
	while (new != NULL){
		//loop_with_generator(new, depth, positions_considered, msks);
		printf("-------->");
		print_game_state(gs);
		print_game_state(new);
		scanf("%s",&s);
		free(new);
		new = copy_game_state(gs);
		selected_pieces = new->pawns;
		
		
		
		new = game_state_generator(gs, new, &piece_incr, &move_incr, &pieces, 
							&move_squares, msks, selected_pieces, KNIGHTMINDEX, pawn_maspawn_function);
		//printf("%" PRIu64 "\n", piece_incr);
		
	}

	return 0;

}
