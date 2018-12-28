#include "move_masks.c"
#include <stdio.h>
GS * knight_move(GS * gs, uint64_t * msks, uint64_t * knights, uint64_t * move_squares, uint64_t * piece_incr, uint64_t * move_incr) {
	int color = gs->color;
	int rcolor = ~color;
	uint64_t index_p;
	uint64_t index_m;
	//run out of both pieces and potential moves
	
	if (*move_squares == 0LL){
		if (*knights == 0LL)
		return NULL; 
		else {
		//print_game_state(gs);
		//shift the knights
		index_p = ffsll(*knights);
		*piece_incr += index_p;
		*move_squares = msks[(*piece_incr-1) * 14 + 4] & ~gs->pieces[color];
		*knights = *knights >> index_p;
		*move_incr = 0;
		}
	//print_game_state(gs);
	//	
	}
	if (*move_squares == 0LL) return NULL;
	index_m = ffsll(*move_squares);
	*move_incr += index_m;
	*move_squares = *move_squares >> index_m;
	uint64_t new_pos = 1LL << (*move_incr - 1);
	uint64_t old_pos = 1LL  << (*piece_incr - 1);
	GS * new_gs = copy_game_state(gs);
	new_gs->pieces[color] = (new_gs->pieces[color] ^ old_pos) | new_pos;
	new_gs->knights[color] = (new_gs->knights[color] ^ old_pos) | new_pos;
       return new_gs;	
	
}

int main () {
	printf("WORKING");
	GS * gs = initial_game_state();
	uint64_t knights = gs->knights[0];
	binary_print_board(knights);
	uint64_t move_squares = 0LL;
	uint64_t piece_incr = 0LL;
	uint64_t move_incr = 0LL;
	uint64_t * msks = build_mask_object();
	//print_game_state(gs);
	//print_game_state(gs);
	GS * new = knight_move(gs,msks,&knights,&move_squares,
				&piece_incr, &move_incr);
       	while (new != NULL){
		printf("%" PRIu64 "\n", move_squares);
		print_game_state(new);
		binary_print_board(move_squares);
		free(new);
		
		printf("\n");
		new = knight_move(gs, msks, &knights, &move_squares, &piece_incr, &move_incr);
		
	}	
	return 0;

}
