#include "move_generators.c"
#define TESTING 1


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
Horrendously expensive. Doesn't actually work :(
*/
uint64_t build_pin_mask (GS * gs, uint64_t * msks) {

    int color = gs->color;
    int r_color = (color + 1) % 2;
    uint64_t pin_mask = 0LL;
    uint64_t pieces =gs->bishops[color];
    int piece_incr = 0;
    uint64_t ray;
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
    return pin_mask;
}
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

int can_castle_kingside (GS * gs, uint64_t * attack_mask, uint64_t * msks, CS_mask * cs_msk){

    int color = gs->color;
    //if castling rules have been previously violated
    if (! gs->castle_king_side[color])
        return 0;
    //if there is nay the free squares to move
    if (cs_msk->kingside_free[color] & gs->all_pieces != 0LL)
        return 0;
    //if we haven't built the attacked square mask, build it.        
    if (attack_mask == NULL)
        *attack_mask = build_attack_mask(gs, msks);

    //if there are no squares conflicting with the attack mask, then give the go ahead.
    if (cs_msk->kingside_non_attack[color] && *attack_mask == 0LL)
        return 1;
    //otherwise, return 0
    return 0;


}

int can_castle_queenside (GS * gs, uint64_t * attack_mask, uint64_t * msks, CS_mask * cs_msk){

    int color = gs->color;
    //if castling rules have been previously violated
    if (! gs->castle_queen_side[color])
        return 0;
    //if there is nay the free squares to move
    if (cs_msk->queenside_free[color] & gs->all_pieces != 0LL)
        return 0;
    //if we haven't built the attacked square mask, build it.        
    if (attack_mask == NULL)
        *attack_mask = build_attack_mask(gs, msks);

    //if there are no squares conflicting with the attack mask, then give the go ahead.
    if (cs_msk->queenside_non_attack[color] && *attack_mask == 0LL)
        return 1;
    //otherwise, return 0
    return 0;


}

GS * kingside_castling_moves_generator (GS * gs, uint64_t * attack_mask, uint64_t msks, CS_mask * cs_msk){
    int color = gs->color;
    if (!can_castle_kingside(gs, attack_mask, msks, cs_msk)) return NULL;

        uint64_t n_klocation =  1LL << (6 + (7*8*color));
        uint64_t n_rlocation =   1LL << (5 + (7*8*color));
        uint64_t o_klocation = 1LL << (4 + (7*8*color));
        uint64_t o_rlocation = 1LL << (7 + (7*8*color));
        GS * new_gs = copy_game_state(gs);
        new_gs->all_pieces ^= (o_klocation | o_rlocation);
        new_gs->all_pieces |= (n_klocation | n_rlocation);
        new_gs->kings[color] &= new_gs->pieces[color];  
        new_gs->rooks[color] &= new_gs->pieces[color];
        return new_gs;
}

GS * queenside_castling_moves_generator (GS * gs, uint64_t * attack_mask, uint64_t msks, CS_mask * cs_msk){
    int color = gs->color;
    if (!can_castle_kingside(gs, attack_mask, msks, cs_msk)) return NULL;

        uint64_t n_klocation =  1LL << (2 + (7*8*color));
        uint64_t n_rlocation =   1LL << (3 + (7*8*color));
        uint64_t o_klocation = 1LL << (0 + (7*8*color));
        uint64_t o_rlocation = 1LL << (5 + (7*8*color));
        GS * new_gs = copy_game_state(gs);
        new_gs->all_pieces ^= (o_klocation | o_rlocation);
        new_gs->all_pieces |= (n_klocation | n_rlocation);
        new_gs->kings[color] &= new_gs->pieces[color];  
        new_gs->rooks[color] &= new_gs->pieces[color];
        return new_gs;









