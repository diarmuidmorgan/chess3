#include "move_generators.c"


/*
* This is an absolutely horrible game loop
*  :(
*/

/* If we just put everything on a stack, that will handle the memory leak right?
*
*/



void loop_with_generator(GS * gs, int depth, int * positions_considered, uint64_t * msks){
	//char s[1000];
	//scanf("%s", &s);
	if (depth == 0) return;

	depth--;
	*positions_considered = *positions_considered + 1;
	
	uint64_t piece_incr = 0LL;
	uint64_t move_incr = 0LL;
	uint64_t pieces = 0LL;
	uint64_t move_squares;
	uint64_t * selected_pieces;
	int msk_number;
	piece_incr = 0LL;
	move_incr = 0LL;
	pieces = gs->knights[0];
	
	move_squares = 0LL;
	GS * new = copy_game_state(gs);
	selected_pieces = new->knights;
	//binary_print_board(gs->knights[0]);
	new = game_state_generator(gs, new, &piece_incr, &move_incr, 
	&pieces, &move_squares, msks, selected_pieces, KNIGHTMINDEX, knight_king_masking_function);
	
	//loop_with_generator(new,depth,positions_considered);
	while (new != NULL){
		loop_with_generator(new, depth, positions_considered, msks);
		free(new);
		new = copy_game_state(gs);
		selected_pieces = new->knights;
		
		
		
		new = game_state_generator(gs, new, &piece_incr, &move_incr, &pieces, 
							&move_squares, msks, selected_pieces, KNIGHTMINDEX, knight_masking_function);
		//printf("%" PRIu64 "\n", piece_incr);
		
	}
	pieces = gs->pawns[0];
    piece_incr = 0LL;
	move_incr = 0LL;
	free(new);
	new = copy_game_state(gs);
	selected_pieces = new->pawns;
	new = game_state_generator(gs, new, &piece_incr, &move_incr, 
	&pieces, &move_squares, msks, selected_pieces, PAWNMVINDEX, pawn_masking_function);
	
	while (new != NULL){
		loop_with_generator(new, depth, positions_considered, msks);
		free(new);
		new = copy_game_state(gs);
		selected_pieces = new->pawns;
		
		
		new = game_state_generator(gs, new, &piece_incr, &move_incr, &pieces, 
							&move_squares, msks, selected_pieces, PAWNMVINDEX, pawn_masking_function);
		
		//printf("%" PRIu64 "\n", piece_incr);
		//loop_with_generator(new, depth, positions_considered);
	}/*
    pieces = gs->bishops[0];
    piece_incr = 0LL;
	move_incr = 0LL;
	free(new);
	new = copy_game_state(gs);
	selected_pieces = new->bishops;
	new = game_state_generator(gs, new, &piece_incr, &move_incr, 
	&pieces, &move_squares, msks, selected_pieces, PAWNMVINDEX, bishop_masking_function);
	
	while (new != NULL){
		loop_with_generator(new, depth, positions_considered, msks);
		free(new);
		new = copy_game_state(gs);
		selected_pieces = new->bishops;
		
		
		new = game_state_generator(gs, new, &piece_incr, &move_incr, &pieces, 
							&move_squares, msks, selected_pieces, PAWNMVINDEX, bishop_masking_function);
		
		//printf("%" PRIu64 "\n", piece_incr);
		//loop_with_generator(new, depth, positions_considered);
	}
    /*
    pieces = gs->rooks[0];
    piece_incr = 0LL;
	move_incr = 0LL;
	free(new);
	new = copy_game_state(gs);
	selected_pieces = new->rooks;
	new = game_state_generator(gs, new, &piece_incr, &move_incr, 
	&pieces, &move_squares, msks, selected_pieces, PAWNMVINDEX, rook_masking_function);
	
	while (new != NULL){
		loop_with_generator(new, depth, positions_considered, msks);
		free(new);
		new = copy_game_state(gs);
		selected_pieces = new->bishops;
		
		
		new = game_state_generator(gs, new, &piece_incr, &move_incr, &pieces, 
							&move_squares, msks, selected_pieces, PAWNMVINDEX, rook_masking_function);
		
		//printf("%" PRIu64 "\n", piece_incr);
		//loop_with_generator(new, depth, positions_considered);
	}
    */
    /*
    pieces = gs->queens[0];
    piece_incr = 0LL;
	move_incr = 0LL;
	free(new);
	new = copy_game_state(gs);
	selected_pieces = new->queens;
	new = game_state_generator(gs, new, &piece_incr, &move_incr, 
	&pieces, &move_squares, msks, selected_pieces, PAWNMVINDEX, queen_masking_function);
	
	while (new != NULL){
		loop_with_generator(new, depth, positions_considered, msks);
		free(new);
		new = copy_game_state(gs);
		selected_pieces = new->bishops;
		
		
		new = game_state_generator(gs, new, &piece_incr, &move_incr, &pieces, 
							&move_squares, msks, selected_pieces, PAWNMVINDEX, queen_masking_function);
		
		//printf("%" PRIu64 "\n", piece_incr);
		//loop_with_generator(new, depth, positions_considered);
	}
    */







    //free(selected_pieces);
	free(new);
    //free(pieces);
    //free(piece_incr);
    //free(move_incr);
    //free(selected_pieces);
    //free(selected_pieces);
    return;
	}

int main () {
	uint64_t * msks = build_mask_object();
	int * positions = malloc(sizeof(int)) ;
	*positions = 0;
	GS * gs = initial_game_state();
	//generation_loop(gs);
	loop_with_generator(gs, 6, positions, msks);
	
	printf("\n%d\n",*positions);
	return 0;
}