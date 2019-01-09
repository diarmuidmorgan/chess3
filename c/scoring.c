/*
Probably don't even need this, as we can just keep track of any changes in gs score instead?

*/

#include "moves_legal.c"

#define MAX_SCORE 100000
#define MIN_SCORE -100000
int count_pieces (uint64_t pieces) {
	
	int count = 0;
	while (pieces != 0LL)
		pieces = pieces >> __builtin_ffsll(pieces);
		count ++;
	return count;
}

/* Simplest possible evaluation function
*
*/
int base_score( GS * gs ) {
	//check for checkmate
	if (gs->kings[0] == 0LL) return MAX_SCORE;
	if (gs->kings[1] == 0LL) return MIN_SCORE;
	int scores[2] = {0,0};
	for (int color=0;color<2;color++) {
	//pass by value here?
	scores[color] = count_pieces(gs->pawns[color]) +
		5 * count_pieces(gs->rooks[color]) +
		3 * count_pieces(gs->knights[color]) +
		3 * count_pieces(gs->bishops[color]) +
		9 * count_pieces(gs->queens[color]);
	}
	return scores[0] - scores[1];

}
