#include "move_arithmetic.c"


int count_pieces (uint64_t pieces) {
	
	int count = 0;
	while (pieces != 0LL)
		pieces = pieces >> ffsll(pieces);
	return count;
}
