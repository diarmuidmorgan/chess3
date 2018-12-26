#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
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
	int color;

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
	gs->color = 0;
	return gs;

}
GS * blank_game_state() {
	GS * gs = malloc(sizeof(GS));
	for (int i=0; i<2;i++) {
	gs->pawns[i] = board_to_word(blank_bit_array());
	gs->rooks[i] = board_to_word(blank_bit_array());
	gs->knights[i] = board_to_word(blank_bit_array());
	gs->bishops[i] = board_to_word(blank_bit_array());
	gs->kings[i] = board_to_word(blank_bit_array());
	gs->queens[i] = board_to_word(blank_bit_array());
	}
	return gs;
}

GS * game_state_from_position_string(char * s, int c) {
	GS * gs = blank_game_state();
	gs->color = c;
	for (int i=0;i<8; i++){

		for (int j=0; j<8;j++) {
			int INDEX = i*8 + j;
			switch(s[INDEX]) {

				case('p') :
					gs->pawns[0] |= 1LL << INDEX;
					break;
				case('P') :
					gs->pawns[1] |= 1LL << INDEX;
					break;
				case('r') :
					gs->rooks[0] |= 1LL << INDEX;
					break;
				case('R') :
					gs->rooks[1] |= 1LL << INDEX;
					break;
				case('h') :
					gs->knights[0] |= 1LL << INDEX;
					break;
				case('H') :
					gs->knights[1] |= 1LL << INDEX;
					break;
				case('b') :
					gs->bishops[0] |= 1LL << INDEX;
					break;
				case('B') :
					gs->pawns[1] |= 1LL << INDEX;
					break;
				case('q') :
					gs->queens[0] |= 1LL << INDEX;
					break;
				case('Q') :
					gs->queens[1] |= 1LL << INDEX;
					break;
				case('k') :
					gs->kings[0] |= 1LL << INDEX;
					break;
				case('K') :
					gs->kings[0] |= 1LL << INDEX;
					break;

				}
			}
		}
	for (int color =0; color < 2; color++){
		gs->pieces[color] = gs->pawns[color] | gs->rooks[color] 
				    | gs->knights[color] | gs->bishops[color] 
				    | gs->rooks[color] | gs->queens[color] | gs->kings [color];
		
		gs->enpassants[color] = 0LL;}
	gs->all_pieces = gs->pieces[0] | gs->pieces[1];
	return gs;
}

GS * read_position_from_file (char * fname) {

	FILE *fp;
	char s[1000];
	fp =fopen(fname,"r");
	fgets(s,1000, fp);
	fclose(fp);
	return game_state_from_position_string(s,0);

}

GS * copy_game_state(GS * gs) {
	GS * new = malloc(sizeof(GS));
	memcpy(new,gs,sizeof(GS));
	return new;
}

void game_state_change_color(GS * gs, int color) {
	//flip the color bit
	gs->color = color;
	gs->pawns[color] = gs->pawns[color] & gs->pieces[color];
	gs->rooks[color] = gs->rooks[color] & gs->pieces[color];
	gs->knights[color] = gs->knights[color] & gs->pieces[color];
	gs->bishops[color] = gs->bishops[color] & gs->pieces[color];
	gs->kings[color] = gs->kings[color] & gs->pieces[color];
	gs->queens[color] = gs->queens[color] & gs->pieces[color];


}



