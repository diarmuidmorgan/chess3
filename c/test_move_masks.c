#include "move_masks.c"

int main () {
	printf("\n");
	uint64_t msk = knight_moves(0,1,1);
	printf("%" PRIu64 "\n", msk);
	binary_print_board(msk);




}
