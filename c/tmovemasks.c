#include "move_masks.c"

int main () {


	uint64_t * msks = build_mask_object();
	binary_print_board(msks[54 * 14 + 10]);
	return 0;




}
