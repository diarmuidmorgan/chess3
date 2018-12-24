#include <stdio.h>
#include <stdint.h>
#include "start_positions.c"
#include "binary_ops.c"
typedef struct {

	uint64_t pawns[2];
	uint64_t rooks[2];
	uint64_t knights[2];
	uint64_t bishops[2] ;
	uint64_t kings[2];
	uint64_t queens[2];
	uint64_t pieces[2];
	uint64_t all_pieces;
	uint64_t enpassants[2];
	int castle_king_side[2];
	int castle_queen_side[2];


} GS;
GS * initial_game_state(){
	
	
	GS * gs = malloc(sizeof(GS));
	gs->pawns[0] = board_to_word(starting_pawns_white());
	gs->pawns[1] = board_to_word(starting_pawns_black());
	gs->rooks[0] = board_to_word(starting_rooks_white());
	gs->rooks[1] = board_to_word(starting_rooks_black());
	gs->knights[0] = board_to_word(starting_knights_white());
	gs->knights[1] = board_to_word(starting_knights_black());
	gs->bishops[0] = board_to_word(starting_bishops_white());
	gs->bishops[1] = board_to_word(starting_bishops_black());
	gs->kings[0] = board_to_word(starting_king_white());
	gs->kings[1] = board_to_word(starting_king_black());
	gs->queens[0] = board_to_word(starting_queen_white());
	gs->queens[1] = board_to_word(starting_queen_black());
	gs->pieces[0] = gs->pawns[0] | gs->rooks[0] | gs->knights[0] | gs->bishops[0] | gs->kings[0] | gs->queens[0]; 	
	gs->pieces[1] = gs->pawns[1] | gs->rooks[1] | gs->knights[1] | gs->bishops[1] | gs->kings[1] | gs->queens[1];
	gs->all_pieces = gs->pieces[0] | gs->pieces[1];
	gs->enpassants[0] = 0LL;
	gs->enpassants[1] = 0LL;
	return gs;

}
GS * copy_game_state(GS * gs) {
	GS * new = malloc(sizeof(GS));
	for (int i=0; i<2; i++) {
	new->pawns[i] = gs->pawns[i];
	new->rooks[i] = gs->rooks[i];
	new->knights[i] = gs->knights[i];
	new->bishops[i] = gs->bishops[i];
	new->kings[i] = gs->kings[i];
	new->queens[i] = gs->queens[i];
	new->all_pieces = gs->all_pieces;
	new->enpassants[i] = gs->enpassants[i]

	}
	return new;

}



