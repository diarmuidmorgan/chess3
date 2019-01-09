#include "move_generators.c"
#define TESTING 1

/* Determines legality of moves e.g pins, and moving into check.
* The pin code isn't really important for anything other than the move parser,
  The checking/attacked squares code is techinally necessary to see if castling is possible.
  Ideally it should be run a bare minimum of times because its ridiculously expensive.
  But it is... possible, that it could get run tens of thousands of times.... in a single loop
Also includes the generator functions for castling.

Really handling castling is a massive pain and will no doubt slow everything right the way down.
As it is, it seems to lead to infinite loops. 

Could it be done pseudo legally -> e.g pass a bitboard of squares the king passed through up to 
the next layer of the stack? This would be way better right? Then introduce a huge penalty// set king squares
to 0LL if one of those 'ghost' squares is taken?

then we can just use these functions here for parsing and maybe evaluation.

*/

// Determines if an attacking ray causes a piece to be pinned.

void ray_pins_piece (uint64_t ray, uint64_t * pin_mask, GS * gs, 
            int color, int (* bitsearchfunc)()){
    
    int r_color = (color + 1) % 2;
    uint64_t king = gs->kings[r_color];
        //if the king is, plain and simple, not in the ray, then we can skip and return
    if ((ray | king) != ray){
     
     return;
    }
    //pieces actually attacked/defended in the ray
    uint64_t pieces = gs->all_pieces & ray;
    if (pieces == 0LL){
        
        return;
    }
    
    int index_of_first_piece = bitsearchfunc(pieces);
   
    uint64_t first_piece = 1LL << (index_of_first_piece - 1);
    
    //if the king is the first piece, then we return here, 
    //    or if the first piece belongs to the attacking color.
    
    if (((first_piece | king ) == king) 
                || ((first_piece | gs->pieces[color])
                        ==gs->pieces[color]))  return;
    
    //remove the first piece. This is agnostic to scan direction
    pieces = pieces ^ first_piece;
   
    int index_of_second_piece = bitsearchfunc(pieces);
    
    uint64_t second_piece = 1ll << (index_of_second_piece - 1);
    //get the second piece in the ray.
    // if the second piece is the king, then we update the pin mask with the first piece
    if ((second_piece | king)  == king   )
        *pin_mask |= first_piece;

    return;
}

/*
Horrendously expensive. Builds a mask of all pinned squares by calling the ray pins piece function
For every ray attack/sliding attack piece.
This can probably be replaced by a better solution -> only checking the rays that the king is in.
All the bit searching is really expensive.
*/

uint64_t build_pin_mask (GS * gs, uint64_t * msks) {

    int r_color = gs->color;
    int color = (r_color + 1) % 2;
    uint64_t pin_mask = 0LL;
    uint64_t pieces =gs->bishops[color];
    int piece_incr = 0;
    uint64_t ray;
    
  
    //we cycle through the pieces, selecting each ray, and calling the ray_pins_piece 
    // function to add any pinned pieces to the pin mask.
    //printf("CHECKING BISHOPS\n");
    while (cycle_pieces(gs, &pieces, &piece_incr)){
        ray = msks[(piece_incr-1) * 14 + DIAGULINDEX];
        ray_pins_piece(ray, &pin_mask, gs, color, bitscanreverse);
         ray = msks[(piece_incr-1) * 14 + DIAGURINDEX];
        ray_pins_piece(ray, &pin_mask, gs, color, bitscanreverse);
         ray = msks[(piece_incr-1) * 14 + DIAGDLINDEX];
        ray_pins_piece(ray, &pin_mask, gs, color, bitscanforward);
        ray = msks[(piece_incr-1) * 14 + DIAGDRINDEX];
        ray_pins_piece(ray, &pin_mask, gs, color, bitscanforward);
    }
    piece_incr = 0;
    pieces = gs->rooks[color];
  
    while (cycle_pieces(gs, &pieces, &piece_incr)){
        ray = msks[(piece_incr-1) * 14 + RANKUINDEX];
        ray_pins_piece(ray, &pin_mask, gs, color, bitscanreverse);
         ray = msks[(piece_incr-1) * 14 + COLLINDEX];
        ray_pins_piece(ray, &pin_mask, gs, color, bitscanreverse);
         ray = msks[(piece_incr-1) * 14 + RANKDINDEX];
        ray_pins_piece(ray, &pin_mask, gs, color, bitscanforward);
        ray = msks[(piece_incr-1) * 14 + COLRINDEX];
        ray_pins_piece(ray, &pin_mask, gs, color, bitscanforward);
    }
    piece_incr = 0;
    pieces = gs->queens[color];
  
    while (cycle_pieces(gs, &pieces, &piece_incr)){
        ray = msks[(piece_incr-1) * 14 + DIAGULINDEX];
        ray_pins_piece(ray, &pin_mask, gs, color, bitscanreverse );
         ray = msks[(piece_incr-1) * 14 + DIAGURINDEX];
        ray_pins_piece(ray, &pin_mask, gs, color, bitscanreverse );
         ray = msks[(piece_incr-1) * 14 + DIAGDLINDEX];
        ray_pins_piece(ray, &pin_mask, gs, color, bitscanforward );
        ray = msks[(piece_incr-1) * 14 + DIAGDRINDEX];
        ray_pins_piece(ray, &pin_mask, gs, color, bitscanforward );
        ray = msks[(piece_incr-1) * 14 + RANKUINDEX];
        ray_pins_piece(ray, &pin_mask, gs, color, bitscanreverse );
         ray = msks[(piece_incr-1) * 14 + COLLINDEX];
        ray_pins_piece(ray, &pin_mask, gs, color, bitscanreverse );
         ray = msks[(piece_incr-1) * 14 + RANKDINDEX];
        ray_pins_piece(ray, &pin_mask, gs, color, bitscanforward );
        ray = msks[(piece_incr-1) * 14 + COLRINDEX];
        ray_pins_piece(ray, &pin_mask, gs, color, bitscanforward );

    }
    //printf("ALL RAYS CHECKED\n");
    return pin_mask;
}

// cycles through a piece type and adds all of its squares to the attack mask
// takes a masking function as a parameter - e.g pawn/bishop masking function

void add_to_attack_mask(uint64_t * attack_squares, uint64_t * msks, uint64_t pin_mask,
             GS * gs, uint64_t pieces, void (* masking_function)()){
    int piece_incr = 0;
    int move_incr = 0;
    uint64_t move_squares = 0LL;
    while (next_piece(gs, 0, msks, &pieces, &move_squares,
        &piece_incr, &move_incr, masking_function)){
        
        if (! (( (1LL << (piece_incr - 1)) | pin_mask) == pin_mask))
            *attack_squares |= move_squares;
            

    }
}
/* Ugly, ugly;
    Lots of code repetition, really. Hideously expensive if we have to run this operation ,
    multiple times. Would be better to just generate all node moves first.
*
*/

//builds the entire attack mask from all of the pieces.

uint64_t build_attack_mask (GS * gs, uint64_t * msks){
    gs->color = (gs->color + 1) % 2;
    uint64_t attack_squares=0LL;
    //actually - do we need the pin mask? What a massive waste of time!
    uint64_t pin_mask = 0LL;
    // uint64_t pin_mask = build_pin_mask(gs, msks);   
    gs->color = (gs->color + 1 ) % 2;
    int color = gs->color;
    uint64_t pieces = gs->bishops[color];
    pieces = gs->pawns[color];
    add_to_attack_mask(&attack_squares, msks, pin_mask, gs, pieces, pawn_masking_function);
    pieces = gs->rooks[color];
    add_to_attack_mask(&attack_squares, msks, pin_mask, gs, pieces, rook_masking_function);
    pieces = gs->bishops[color];
    add_to_attack_mask(&attack_squares, msks, pin_mask, gs, pieces, bishop_masking_function);
    pieces = gs->knights[color];
    add_to_attack_mask(&attack_squares, msks, pin_mask, gs, pieces, knight_masking_function);
    pieces = gs->kings[color];
    add_to_attack_mask(&attack_squares, msks, pin_mask, gs, pieces, king_masking_function);
    pieces = gs->queens[color];
    add_to_attack_mask(&attack_squares, msks, pin_mask, gs, pieces, queen_masking_function);
    gs->color = (gs->color + 1) % 2;
    return attack_squares;

}

/* When this function is run, attack mask will be declared, but we probably won't have
    bothered to build it yet. At least I hope not!

    I feel the four functions below could be reduced to half as much code? Way less anyway.
*/

int kingside_castling_generator_has_next (GS * gs, uint64_t * attack_mask, 
                                            uint64_t * msks, CS_mask * cs_msk){
   
    int color = gs->color;
    //printf("COLOR %d\n", color);
    // printf("CAN CASTLE %d\n", gs->castle_king_side[color]);
    //if castling rules have been previously violated
    if (! gs->castle_king_side[color])
        return 0;
   
    //if there is nay the free squares to move
    //binary_print_board(cs_msk->kingside_free[(color)]);


    //dirty hack
   return 1;
    if (cs_msk->kingside_free[color] & gs->all_pieces == 0ULL){
        
    
    //printf("COLOR: %d\n",gs->color);
    //binary_print_board(cs_msk->kingside_free[color] & gs->all_pieces);
   // printf("STILL HERE\n");
    //if we haven't built the attacked square mask, build it.        
    //if (attack_mask == NULL)
    //    *attack_mask = build_attack_mask(gs, msks);
    //binary_print_board(cs_msk->kingside_non_attack[color]);
    //if there are no squares conflicting with the attack mask, then give the go ahead.
   
    //if (cs_msk->kingside_non_attack[color] && *attack_mask == 0LL){
       
    return 1;
    }
    return 0;
    //otherwise, return 0
    //return 0;


}


//does the same but for the queen side.

int queenside_castling_generator_has_next (GS * gs, uint64_t * attack_mask, 
                                uint64_t * msks, CS_mask * cs_msk){
   
    int color = gs->color;
    
    //if castling rules have been previously violated
    if (! gs->castle_queen_side[color])
        return 0;
    //if there is nay the free squares to move

    //dirty hack
    return 1;
    if (cs_msk->queenside_free[color] & gs->all_pieces == 0LL)
        return 1;
    //if we haven't built the attacked square mask, build it.        
    //if (attack_mask == NULL)
    //    *attack_mask = build_attack_mask(gs, msks);

    //if there are no squares conflicting with the attack mask, then give the go ahead.
    //if (cs_msk->queenside_non_attack[color] && *attack_mask == 0LL)
    return 0;
    //otherwise, return 0
    //return 0;


}

//functions to perform the actual castling procedure.

void kingside_castling_generator_next (GS * new_gs, Zob * z){
       
        int color = new_gs->color;
        //should make this a subtract/add operation? And the same in the other methods.
        char k = (color) ? 'K' : 'k';
        char r = (color) ? 'R' : 'r';
        int k_index = z->dict[(int) k];
        int r_index = z->dict[(int) r];
        uint64_t n_klocation =  1LL << (6 + (7*8*color));
        uint64_t n_rlocation =   1LL << (5 + (7*8*color));
        uint64_t o_klocation = 1LL << (4 + (7*8*color));
        uint64_t o_rlocation = 1LL << (7 + (7*8*color));
        new_gs->all_pieces &= ~(o_klocation | o_rlocation);
        new_gs->all_pieces |= (n_klocation | n_rlocation);
        new_gs->pieces[color] &= ~(o_klocation | o_rlocation);
        new_gs->pieces[color] |= (n_klocation | n_rlocation);
        new_gs->kings[color] = n_klocation;  
        new_gs->rooks[color] &= ~o_rlocation;
        new_gs->rooks[color] |= n_rlocation;
        new_gs->castle_king_side[color] = 0;
        new_gs->castle_queen_side[color] = 0;
        
        new_gs->color = (color + 1) % 2;
        new_gs->hash ^= z->vals[(6 + 7*8*color) * 12 + k_index ];
        new_gs->hash ^= z->vals[(5 + 7*8*color) * 12 + r_index ];
        new_gs->hash ^= z->vals[(4 + 7*8*color) * 12 + k_index ];
         new_gs->hash ^= z->vals[(7 + 7*8*color) * 12 + r_index ];
         new_gs->has_castled[color]=1;
}

void queenside_castling_generator_next (GS * new_gs, Zob * z){
    
        int color = new_gs->color;
        char k = (color) ? 'K' : 'k';
        char r = (color) ? 'R' : 'r';
        int k_index = z->dict[(int) k];
        int r_index = z->dict[(int) r];
    

        uint64_t n_klocation =  1LL << (2 + (7*8*color));
        uint64_t n_rlocation =   1LL << (3 + (7*8*color));
        uint64_t o_klocation = 1LL << (4 + (7*8*color));
        uint64_t o_rlocation = 1LL << (0 + (7*8*color));
        new_gs->all_pieces &= ~(o_klocation | o_rlocation);
        new_gs->all_pieces |= (n_klocation | n_rlocation);
        new_gs->pieces[color] &= ~(o_klocation | o_rlocation);
        new_gs->pieces[color] |= (n_klocation | n_rlocation);
        new_gs->kings[color] = n_klocation;  
        new_gs->rooks[color] &= ~o_rlocation;
        new_gs->rooks[color] |= n_rlocation;
        new_gs->castle_king_side[color] = 0;
        new_gs->castle_queen_side[color] = 0;
        new_gs->has_castled[color]=1;
        new_gs->color = (color + 1) % 2;
        new_gs->hash ^= z->vals[(2 + 7*8*color) * 12 + k_index ];
        new_gs->hash ^= z->vals[(3 + 7*8*color) * 12 + r_index ];
        new_gs->hash ^= z->vals[(4 + 7*8*color) * 12 + k_index ];
        new_gs->hash ^= z->vals[(0 + 7*8*color) * 12 + r_index ];
}

//if there is a valid enpassant, sets the three target squares.
// returns 1 if valid, false otherwise
int enpassant_generator_has_next (GS * gs, int * pawn_incr, int * pawn_square_number, int * target_square_number,
                            uint64_t * pawns, uint64_t * pawn_square,
                            uint64_t * enpassant_square, uint64_t * target_square){

    int color = gs->color;
    //printf("%d\n", *pawn_incr);
    //no enpassant squares to play. This is the majority case.
    if (gs->enpassants[color] == 0LL || *pawn_incr >1) return 0;
    else{
        int enpassant_index = ffsll(gs->enpassants[color]) - 1;
        *enpassant_square = 1LL << enpassant_index;
       
        //test for left pawn
        //not sure if I have the directions right here.
       
        if (*pawn_incr == 0){
            if (enpassant_index % 8 < 7){
            *pawn_incr = *pawn_incr + 1;
            *pawn_square_number = enpassant_index + 1;
            *pawn_square = *enpassant_square << 1;
            
            if ((gs->pawns[color] | *pawn_square) == gs->pawns[color]){

                //target square will be either 8 squares ahead or behind of enpassant square.
                if (color == 0){
                    *target_square = *enpassant_square << 8;
                    *target_square_number = enpassant_index + 8;

                }
                
                else {
                    *target_square = *enpassant_square >> 8;
                    *target_square_number = enpassant_index - 8;
                }
                
                return 1;
            }
            else{

            }
                
        }
        else *pawn_incr = * pawn_incr + 1;
        }
        
        if (*pawn_incr == 1 && (enpassant_index % 8 > 0)){
            *pawn_incr = *pawn_incr + 1;
            *pawn_square = *enpassant_square >> 1;
            *pawn_square_number = enpassant_index - 1;
            //binary_print_board(*pawn_square);
            if ((gs->pawns[color] | *pawn_square) == gs->pawns[color]){
                
                 if (color == 0){
                    *target_square = *enpassant_square << 8;
                    *target_square_number = enpassant_index + 8;
                   //  binary_print_board(*target_square);
                   //  binary_print_board(*enpassant_square);


                }
                
                else {
                    *target_square = *enpassant_square >> 8;
                    *target_square_number = enpassant_index - 8;
                  //  binary_print_board(*target_square);
                   //  binary_print_board(*enpassant_square);

                }
                
                return 1;
            }
        }
        else{ *pawn_incr = *pawn_incr + 1;
        return 0;
        }
    }
}

//perform the enpassant move and update the new game state (no need for a game_state.c function)
void enpassant_generator_next(GS * new_gs, uint64_t * pawn_square, 
                                uint64_t * target_square, uint64_t * enpassant_square){
    int color = new_gs->color;
    int r_color = (color + 1) % 2;
    new_gs -> pieces[color] &=   ~( *pawn_square);
    new_gs -> pieces[r_color] &= ~(*enpassant_square);
    new_gs -> pieces[color] |= *target_square;
    new_gs -> pawns[color]  &=   ~( *pawn_square);
    new_gs -> pawns[color] |= *target_square;
    new_gs ->pawns[r_color] &= ~ ( * enpassant_square);
    new_gs->all_pieces = new_gs->pieces[color] | new_gs->pieces[r_color];
    if (color == 0)
        new_gs->score += 1;
    else
        new_gs->score-=1;
    new_gs->color = r_color; 

	char p = (color) ? 'P' : 'p';
    //damn should have just saved these somewhere...
   /* int tsq = ffsll(*target_square) -1;
    int epsq = ffsll(*enpassant_square) -1;
    int psq = ffsll(*pawn_square) - 1; 
	new_gs->board_rep[tsq] = p;
	new_gs->board_rep[epsq] = '_';
    
    new_gs->board_rep[psq] = '_';
    */

}









