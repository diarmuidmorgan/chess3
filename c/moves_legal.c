#include "move_generators.c"
#define TESTING 0
/* Functions that will, I guess, lead to proper evaluation.
* Currently, move generation is pseudo legal, but missing enpassants and castling.
* Castling is the worst, because we need to check for proper legality. The search engine won't just
Find a king-capture to refute an incorrect castle operation
    
    And to do this, its not enough to just generate a list of all possible attack squares. 
    We need (technically) to check for any pins as well. So this will involve reading both side's
    pieces entirely?

    Actually, I'm no longer sure about this

    So far this is a nightmare and needs proper tests etc

*
*/

/* Bitmap of squares that can't move
*
    This is all terrible
*/

/* Equally terrible function.
*
*/


/* oh dear....
*
*/

void ray_pins_piece (uint64_t ray, uint64_t * pin_mask, GS * gs, int color, int (* bitsearchfunc)()){
    //char s[1000];
    //scanf("%s",&s);
    int r_color = (color + 1) % 2;
    uint64_t king = gs->kings[r_color];
    
    if ((ray | king) != ray){
     
     return;
    }
    
    uint64_t pieces = gs->all_pieces & ray;
    int index = bitsearchfunc(pieces);
    uint64_t first_piece = 1LL << (index - 1);
    if (TESTING){
    binary_print_board(first_piece);
    binary_print_board(king);
    binary_print_board(ray);
    }
    if ((gs->pieces[r_color] | first_piece) != gs->pieces[r_color]){
        
        return;

    }
    else if ((king | first_piece) == king) {
       
        return;
    
    }
    
    int count = 0;
    pieces = pieces >> index;
   
    if (((first_piece << ( bitsearchfunc(pieces) +1)) | king) == king){
        //binary_print_board(first_piece << ( bitsearchfunc(pieces) +1) | king);
        printf("KING");
        //binary_print_board(king);
        *pin_mask |= first_piece;
       
    }
   
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
        ray_pins_piece(ray, &pin_mask, gs, color, bitscanreverse);
         ray = msks[(piece_incr-1) * 14 + DIAGURINDEX];
        ray_pins_piece(ray, &pin_mask, gs, color, bitscanreverse);
         ray = msks[(piece_incr-1) * 14 + DIAGDLINDEX];
        ray_pins_piece(ray, &pin_mask, gs, color, bitscanforward);
        ray = msks[(piece_incr-1) * 14 + DIAGDRINDEX];
        ray_pins_piece(ray, &pin_mask, gs, color, bitscanforward);
        ray = msks[(piece_incr-1) * 14 + RANKUINDEX];
        ray_pins_piece(ray, &pin_mask, gs, color, bitscanreverse);
         ray = msks[(piece_incr-1) * 14 + COLLINDEX];
        ray_pins_piece(ray, &pin_mask, gs, color, bitscanreverse);
         ray = msks[(piece_incr-1) * 14 + RANKDINDEX];
        ray_pins_piece(ray, &pin_mask, gs, color, bitscanforward);
        ray = msks[(piece_incr-1) * 14 + COLRINDEX];
        ray_pins_piece(ray, &pin_mask, gs, color, bitscanreverse);

    }
    return pin_mask;
}
void add_to_attack_mask(uint64_t * attack_squares, uint64_t * msks, uint64_t pin_mask,
             GS * gs, uint64_t pieces, void (* masking_function)()){
    int piece_incr = 0;
    int move_incr = 0;
    uint64_t move_squares = 0LL;
    while (next_piece(gs, 0, msks, &pieces, &move_squares,
        &piece_incr, &move_incr, bishop_masking_function)){
        
        if (! ((1LL << (piece_incr - 1) & pin_mask) == pin_mask))
            *attack_squares != move_squares;

    }
}
/* Ugly, ugly;
*
*/
uint64_t build_attack_mask (GS * gs, uint64_t * msks){
    gs->color = (gs->color + 1) % 2;
    uint64_t attack_squares=0LL;
    uint64_t pin_mask = build_pin_mask(gs, msks);    
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



