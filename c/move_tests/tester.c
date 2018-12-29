#include "../move_generators.c"

uint64_t move_squares_from_file(char *fname){

	FILE *fp;
	char s[1000];
	fp =fopen(fname,"r");
	fgets(s,1000, fp);
	fclose(fp);
	int * b = blank_bit_array();
	for (int i=0; i<64; i++){
		if (s[i] == '1'){
			b[i] = 1;
		}
		else b[i] = 0;

	}
	return board_to_word(b);
}
int bishop_tester (char * position_file, char * correct_moves){
	printf("\nTesting bishops---->\n");
	GS * gs = read_position_from_file(position_file);
	uint64_t correct_word = move_squares_from_file(correct_moves);
	//print_game_state(gs);
	gs->color = 1;
	uint64_t * msks = build_mask_object();
	int piece_incr = 0;
	int move_incr = 0;
	uint64_t pieces = 0LL;
	uint64_t move_squares;
	uint64_t * selected_pieces;
	int msk_number;
	int color = 1;
	pieces = gs->bishops[color];
	
	move_squares = 0LL;
	GS * new = copy_game_state(gs);
	selected_pieces = new->bishops;
	//binary_print_board(gs->knights[0]);
	new = next_piece(gs, KNIGHTMINDEX, msks, &pieces, &move_squares, &piece_incr, &move_incr, bishop_masking_function);
	if (move_squares == correct_word) printf("Passed");
	else { printf("Failed");

	binary_print_board(move_squares);
	printf("\n!=\n");
	binary_print_board(correct_word);



	}
	return move_squares == correct_word;

}
int pawn_tester (char * position_file, char * correct_moves){
	printf("\nPawns---->\n");
	GS * gs = read_position_from_file(position_file);
	uint64_t correct_word = move_squares_from_file(correct_moves);
	//print_game_state(gs);
	gs->color = 1;
	uint64_t * msks = build_mask_object();
	int piece_incr = 0;
	int move_incr = 0;
	uint64_t pieces = 0LL;
	uint64_t move_squares;
	uint64_t * selected_pieces;
	int msk_number;
	int color = 1;
	pieces = gs->pawns[color];
	
	move_squares = 0LL;
	GS * new = copy_game_state(gs);
	selected_pieces = new->pawns;
	//binary_print_board(gs->knights[0]);
	new = next_piece(gs, KNIGHTMINDEX, msks, &pieces, &move_squares, &piece_incr, &move_incr, pawn_masking_function);
	if (move_squares == correct_word) printf("Passed");
	else { printf("Failed");

	binary_print_board(move_squares);
	printf("\n!=\n");
	binary_print_board(correct_word);



	}
	return move_squares == correct_word;

}

int knight_tester (char * position_file, char * correct_moves){
	printf("\nTesting knights---->\n");
	GS * gs = read_position_from_file(position_file);
	uint64_t correct_word = move_squares_from_file(correct_moves);
	//print_game_state(gs);
	gs->color = 1;
	uint64_t * msks = build_mask_object();
	int piece_incr = 0;
	int move_incr = 0;
	uint64_t pieces = 0LL;
	uint64_t move_squares;
	uint64_t * selected_pieces;
	int msk_number;
	int color = 1;
	pieces = gs->knights[color];
	
	move_squares = 0LL;
	GS * new = copy_game_state(gs);
	selected_pieces = new->knights;
	//binary_print_board(gs->knights[0]);
	new = next_piece(gs, KNIGHTMINDEX, msks, &pieces, &move_squares, &piece_incr, &move_incr, knight_masking_function);
	
	if (move_squares == correct_word) printf("Passed");
	else { printf("Failed");

	binary_print_board(move_squares);
	printf("\n!=\n");
	binary_print_board(correct_word);



	}
	printf("\n");
	return move_squares == correct_word;

}
int rook_tester (char * position_file, char * correct_moves){
	printf("\nTesting Rooks---->\n");
	GS * gs = read_position_from_file(position_file);
	uint64_t correct_word = move_squares_from_file(correct_moves);
	//print_game_state(gs);
	gs->color = 1;
	uint64_t * msks = build_mask_object();
	int piece_incr = 0;
	int move_incr = 0;
	uint64_t pieces = 0LL;
	uint64_t move_squares;
	uint64_t * selected_pieces;
	int msk_number;
	int color = 1;
	pieces = gs->rooks[color];
	
	move_squares = 0LL;
	GS * new = copy_game_state(gs);
	selected_pieces = new->rooks;
	//binary_print_board(gs->knights[0]);
	new = next_piece(gs, KNIGHTMINDEX, msks, &pieces, &move_squares, &piece_incr, &move_incr, rook_masking_function);
	
	if (move_squares == correct_word) printf("Passed");
	else { printf("Failed");

	binary_print_board(move_squares);
	printf("\n!=\n");
	binary_print_board(correct_word);



	}
	return move_squares == correct_word;

}
int king_tester (char * position_file, char * correct_moves){
	printf("\nTesting Kings---->\n");
	GS * gs = read_position_from_file(position_file);
	uint64_t correct_word = move_squares_from_file(correct_moves);
	//print_game_state(gs);
	gs->color = 1;
	uint64_t * msks = build_mask_object();
	int piece_incr = 0;
	int move_incr = 0;
	uint64_t pieces = 0LL;
	uint64_t move_squares;
	uint64_t * selected_pieces;
	int msk_number;
	int color = 1;
	pieces = gs->kings[color];
	
	move_squares = 0LL;
	GS * new = copy_game_state(gs);
	selected_pieces = new->kings;
	//binary_print_board(gs->knights[0]);
	new = next_piece(gs, KNIGHTMINDEX, msks, &pieces, &move_squares, &piece_incr, &move_incr, king_masking_function);
	
	if (move_squares == correct_word) printf("Passed");
	else { printf("Failed");

	binary_print_board(move_squares);
	printf("\n!=\n");
	binary_print_board(correct_word);



	}
	return move_squares == correct_word;

}

int main () {
	bishop_tester("positions/bishop2.chess","positions/bishop2correct.chess");
	
	bishop_tester("positions/bishop2.chess","positions/bishop2correct.chess");
	bishop_tester("positions/bishops3.chess","positions/bishops3correct.chess");
	bishop_tester("positions/bishops4.chess","positions/bishops4correct.chess");
	bishop_tester("positions/bishops5.chess","positions/bishops5correct.chess");
	knight_tester("positions/horses.chess","positions/horsescorrect.chess");
	king_tester("positions/kings.chess", "positions/kingscorrect.chess");
	pawn_tester("positions/pawns1.chess", "positions/pawns1correct.chess");
	pawn_tester("positions/pawns2.chess", "positions/pawns2correct.chess");
	rook_tester("positions/rooks.chess", "positions/rookscorrect.chess");
	
	/*
	printf("examine this...");
	binary_print_board(gs->pieces[1]);
	binary_print_board(move_squares);
	if (move_squares & gs->pieces[1] != 0LL )
		printf("FUCKED");
	binary_print_board(move_squares);
	//loop_with_generator(new,depth,positions_considered);
	while (new != NULL){
		//loop_with_generator(new, depth, positions_considered, msks);
		printf("-------->");
		print_game_state(gs);
		print_game_state(new);
		scanf("%s",&s);
		free(new);
		new = copy_game_state(gs);
		selected_pieces = new->kings;
		
		
		
		new = game_state_generator(gs, new, &piece_incr, &move_incr, &pieces, 
							&move_squares, msks, selected_pieces, KNIGHTMINDEX, king_masking_function);
		//printf("%" PRIu64 "\n", piece_incr);
	*/	
	

	return 0;

}
