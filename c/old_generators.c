

GS * rook_generator(GS * gs, int * piece_incr, int * move_incr, uint64_t * pieces,
					uint64_t * move_squares, uint64_t * msks){
		
		GS * new = copy_game_state(gs);
		uint64_t * selected_pieces = &new->rooks[gs->color];
		new = game_state_generator(gs, new, piece_incr, move_incr, pieces, move_squares, msks,
					selected_pieces, 0, rook_masking_function);
		
		//set castling bits
		/*
		int color = gs->color;
		if (new->castle_queen_side[color] && *piece_incr == 7 + (56 * color) )
			new->castle_queen_side[color] = 0;
		else if (new->castle_king_side[color] && *piece_incr == 7 + (56 * color) )
			new->castle_king_side[color] = 0;
		*/
		return new;
}

GS * pawn_generator(GS * gs, int * piece_incr, int * move_incr, uint64_t * pieces,
					uint64_t * move_squares, uint64_t * msks){
		
		GS * new = copy_game_state(gs);
		uint64_t * selected_pieces = &new->pawns[gs->color];
		new = game_state_generator(gs, new, piece_incr, move_incr, pieces, move_squares, msks,
					selected_pieces, 0, pawn_masking_function);
		//enpassant rule here
		// actually this below rule won't work at all.
		/*
		int color = gs->color;
		if ((color == 0 && (*move_incr < 48 && *move_incr > 40 ))
			|| color == 1 && (*move_incr > 8 && *move_incr < 16))){
			new->enpassants[color] = 1LL << (*move_incr - 1);

		}
		*/
		return new;
	}
GS * knight_generator(GS * gs, int * piece_incr, 
						int * move_incr, uint64_t * pieces,
					uint64_t * move_squares, uint64_t * msks){
		
		
		GS * new = copy_game_state(gs);
		uint64_t * selected_pieces = &new->knights[gs->color];
		new = game_state_generator(gs, new, piece_incr, move_incr, pieces, move_squares, msks,
					selected_pieces, 0, knight_masking_function);
		return new;
	}
GS * king_generator(GS * gs, int * piece_incr, int * move_incr, 
						uint64_t * pieces, uint64_t * move_squares, 
												uint64_t * msks){
		
		
		GS * new = copy_game_state(gs);
		uint64_t * selected_pieces = &new->kings[gs->color];
		new = game_state_generator(gs, new, piece_incr, move_incr, pieces, move_squares, msks,
					selected_pieces, 0, king_masking_function);
		if (new){
		int color = gs->color;
		//handle castling
		new->castle_king_side[color] = 0;
		new->castle_king_side[color] = 0;
		}
		return new;
	}
GS * queen_generator(GS * gs, int * piece_incr, int * move_incr, 
					uint64_t * pieces, int64_t * move_squares, uint64_t * msks){
		
		
		GS * new = copy_game_state(gs);
		uint64_t * selected_pieces = &new->queens[gs->color];
		new = game_state_generator(gs, new, piece_incr, move_incr, pieces, move_squares, msks,
					selected_pieces, 0, queen_masking_function);
		return new;
	}
GS * bishop_generator(GS * gs, int * piece_incr, int * move_incr, uint64_t * pieces,
					uint64_t * move_squares, uint64_t * msks){
		
		
		GS * new = copy_game_state(gs);
		uint64_t * selected_pieces = &new->bishops[gs->color];
		new = game_state_generator(gs, new, piece_incr, move_incr, pieces, move_squares, msks,
					selected_pieces, 0, bishop_masking_function);
		return new;
	}

*/