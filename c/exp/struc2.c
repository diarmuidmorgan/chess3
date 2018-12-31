#include "../printer.c"


GS make_game_state () {

	GS g;
	g.pawns[0] = 2LL;
	return g;

}

int main () {

	GS g = make_game_state();
	printf("%" PRIu64 "\n", g.pawns[0]);
	GS g1 = init_game_state();
	print_game_state(&g1);
	printf("\n\n\n\n");
	GS g2 = g1;
	g2.color = 1;
	printf("%d\n", g1.color);
	printf("%d\n", g2.color);
	return 0;


}
