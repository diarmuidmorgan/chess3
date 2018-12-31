#include "move_generators.c"


/*
* This is an absolutely horrible game loop.

It should rightly be about 100 lines shorter.
*  :(
*/

/* Damn this still isn't good.
*

* Redundant.
*/

void game_loop (GS * gs, int depth, int * positions_considered, uint64_t * msks){
	print_game_state(gs);
	int color = gs-> color;
	if (depth == 0) return;
	depth--;
	*positions_considered = *positions_considered + 1;
	int piece_incr = 0;
	int move_incr = 0;
	uint64_t pieces;
	uint64_t move_squares;
	pieces = gs->knights[color];
	GS * new_gs = knight_generator(gs, &piece_incr, &move_incr, &pieces,
							&move_squares, msks);
	while (new_gs != NULL){
		game_loop(new_gs, depth-1, positions_considered, msks);
		free(new_gs);
		new_gs = knight_generator(gs, &piece_incr, &move_incr, &pieces,
							&move_squares, msks);
	}
	
	pieces = gs->bishops[color];
	new_gs = bishop_generator(gs, &piece_incr, &move_incr, &pieces,
							&move_squares, msks);
	while (new_gs != NULL){
		game_loop(new_gs, depth-1, positions_considered, msks);
		free(new_gs);
		new_gs = bishop_generator(gs, &piece_incr, &move_incr, &pieces,
							&move_squares, msks);
	}
	pieces = gs->rooks[color];
	new_gs = rook_generator(gs, &piece_incr, &move_incr, &pieces,
							&move_squares, msks);
	while (new_gs != NULL){
		game_loop(new_gs, depth-1, positions_considered, msks);
		free(new_gs);
		new_gs = rook_generator(gs, &piece_incr, &move_incr, &pieces,
							&move_squares, msks);
	}
	pieces = gs->queens[color];
	new_gs = queen_generator(gs, &piece_incr, &move_incr, &pieces,
							&move_squares, msks);
	while (new_gs != NULL){
		game_loop(new_gs, depth-1, positions_considered, msks);
		free(new_gs);
		new_gs = queen_generator(gs, &piece_incr, &move_incr, &pieces,
							&move_squares, msks);
	}
	pieces = gs->pawns[color];
	new_gs = pawn_generator(gs, &piece_incr, &move_incr, &pieces,
							&move_squares, msks);
	while (new_gs != NULL){
		game_loop(new_gs, depth-1, positions_considered, msks);
		free(new_gs);
		new_gs = pawn_generator(gs, &piece_incr, &move_incr, &pieces,
							&move_squares, msks);
	}
	pieces = gs->kings[color];
	new_gs = king_generator(gs, &piece_incr, &move_incr, &pieces,
							&move_squares, msks);
	while (new_gs != NULL){
		game_loop(new_gs, depth-1, positions_considered, msks);
		free(new_gs);
		new_gs = king_generator(gs, &piece_incr, &move_incr, &pieces,
							&move_squares, msks);
	}

}


int main () {
	uint64_t * msks = build_mask_object();
	int * positions = malloc(sizeof(int)) ;
	*positions = 0;
	GS * gs = initial_game_state();
	//generation_loop(gs);
	game_loop(gs, 1, positions, msks);
	
	printf("\n%d\n",*positions);
	return 0;
}