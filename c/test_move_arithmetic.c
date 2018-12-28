#include "move_arithmetic.c";

int main () {
	uint64_t * msks = build_mask_object();
	GS * gs = read_position_from_file("positions/queentwopawns.txt");
	print_game_state(gs);
	uint64_t pieces = gs->queens[1];
	binary_print_board(pieces);
	uint64_t piece_incr = 0LL;
	uint64_t move_incr = 0LL;
	uint64_t move_squares = 0LL;
	next_piece(gs,4,msks,&pieces, &move_squares, &piece_incr, &move_incr, queen_masking_function); 
	binary_print_board(move_squares);
	
}
