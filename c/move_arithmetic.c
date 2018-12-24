#include "game_state.c"
#include "binary_ops.c"
#include <stdio.h>
#include <stdlib.h>
#include "move_masks.c"
#include <stdint.h>
#include <strings.h>
#include <math.h>
llist * knight_moves(GS * gs, uint64_t * msks) {
	llist * new_states = create_llist();
	uint64_t knights = gs->knights[gs->color];
	uint64_t knights_msk;
	uint64_t incr = 0LL;
	while knights != 0LL {
		
		uint64_t index = ffsll(knights);
		incr += index;
		knights_msk = msks[index+4];
		knights = knights>>index;
		uint64_t move_squares = knights_msk & (~ gs->pieces[(gs->color + 1)] % 2);
		uint64_t incr2=0
		while (move_squares != 0LL){
			uint64_t index2 = ffsll(move_squares);
			incr2 + index2;
			move_squares = move_squares >> index2;
			new_position = knights_msk & ~temp_squares;
		      	knights_msk = knights_msk & ~new_position;	
			GS * new = copy_game_state(gs);
			new->knights[color] = new->(knights[color] & knights) | new_position;
			new->pieces[other_color] = new->pieces[other_color] ^ new_position;
			update_game_state(new,other_color);
		}
	}


	}	

