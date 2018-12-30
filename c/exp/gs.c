#include "../game_state.c"

int main () {

	GS * gs = initial_game_state();
	binary_print_board(gs->pieces[0]);
	printf("%" PRIu64 "\n", gs->pieces[0]);
	printf("%d", bitscanforward(gs->pieces[0]));

}
