#include "move_arithmetic.c"


int count_pieces (uint64_t pieces) {
	
	int count = 0;
	while (pieces != 0LL)
		pieces = pieces >> ffsll(pieces);
		count ++;
	return count;
}

/* Simplest possible evaluation function
*
*/
int base_score( GS * gs ) {

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
