#include "printer.c"

int main () {

	char * s = "________\
		________\
		________\	
		________\
		__Q_____\	
		________\
		________\
		________";

	GS * gs = read_position_from_file("positions/singlequeen.txt");
	print_game_state(gs);
	








}
