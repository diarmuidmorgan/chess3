#include "parser.c"
// Abstract the parser of death 


int simple_pawn_move(char * line, GS * gs, GS * new_gs, 
                    uint64_t * msks, CS_mask * cs_msk){

        int piece_incr=0;
        int move_incr = 0;
        uint64_t move_squares = 0LL;
        int move_square = ((int) (line[1]-49)) * 8 + ((int) line[0] -97);
        int index = 1;
        uint64_t pieces = gs->pawns[gs->color];
        uint64_t attack_squares = build_attack_mask(gs, msks);
        
        while ( moves_generator(gs, new_gs, msks, &index, &piece_incr, 
                        &move_incr, &pieces, &move_squares,
                            &attack_squares, cs_msk)
                        
                        && 
                                index < 2){
            
                if ( move_square == (move_incr - 1)  &&
                ( (piece_incr-1) % 8 == ((int) line[0] -97)  && 
                pin_mask_safe(&attack_squares, piece_incr) ) )
                    
                            return 1;

                *new_gs = *gs;
            }
            return 0;      
        }

int simple_pawn_capture(char * line, GS * gs, GS * new_gs, 
                    uint64_t * msks, CS_mask * cs_msk){

        int move_square = ((int) (line[3] - 49)) * 8 + ((int) (line[2]-97)); 
        int col = ((int) (line[0]-97));
        int index = 1;
        int piece_incr=0;
        int move_incr = 0;
        uint64_t move_squares = 0LL;
        uint64_t pieces = gs->pawns[gs->color];
        uint64_t attack_squares = build_attack_mask(gs, msks);
        *new_gs = *gs;
       
        while (moves_generator(gs, new_gs, msks, &index, &piece_incr, 
                    &move_incr, &pieces, &move_squares,
                        &attack_squares, cs_msk)
                 &&
                        index < 2  ){
            
            
             if ( move_square == (move_incr - 1) && 
             (piece_incr -1) % 8 == col  && 
             pin_mask_safe(&attack_squares, piece_incr))
                
                return 1;

            *new_gs = *gs;
        }
        return 0;
    }

int  simple_pawn_capture(char * line, GS * gs, GS * new_gs, 
                    uint64_t * msks, CS_mask * cs_msk){

    int index = 1;
    int piece_incr=0;
    int move_incr = 0;
    uint64_t move_squares = 0LL;
    uint64_t pieces = gs->pawns[gs->color];
    uint64_t attack_squares = build_attack_mask(gs, msks);
    *new_gs = *gs;
    int move_square= ((int) (line[3] - 49)) * 8 + ((int) (line[2]-97)); 
    int position =((int) (line[0] - 49)) * 8 + ((int) (line[1]-97)); 

    while (moves_generator(gs, new_gs, msks, &index, &piece_incr, 
                    &move_incr, &pieces, &move_squares,
                        &attack_squares, cs_msk)
                        &&
                     index < 2    ){
        
        if (move_square == (move_incr - 1) && 
                (piece_incr -1) == position &&
            pin_mask_safe(&attack_squares, piece_incr) )
                
                    return 1;
            
            *new_gs = *gs;
              
        }
    return 0;
}


int simple_piece_move (char * line, GS * gs, GS * new_gs, 
                    uint64_t * msks, CS_mask * cs_msk){

    
    int piece_incr=0;
    int move_incr = 0;
    uint64_t move_squares = 0LL;
    uint64_t attack_squares = build_attack_mask(gs, msks);
    *new_gs = *gs;
    char piece = line[0]; 
    int  index = set_index(piece);
    int check_index = index + 1;
    uint64_t pieces = set_pieces(gs, index);
    int move_square = ((int) (line[2]-49)) * 8 + ((int) line[1] -97); 
    while (moves_generator(gs, new_gs, msks, &index, &piece_incr, 
                    &move_incr, &pieces, &move_squares,
                        &attack_squares, cs_msk)
                    && 
                    index < check_index  ){
            
            if ( move_square == (move_incr - 1) 
                 && pin_mask_safe(&attack_squares, piece_incr) ) 
                return 1;
            
            *new_gs = *gs;
        }
        return 0;
 }

int simple_piece_capture (char * line, GS * gs, GS * new_gs, 
                    uint64_t * msks, CS_mask * cs_msk){

    
    int piece_incr=0;
    int move_incr = 0;
    uint64_t move_squares = 0LL;
    uint64_t attack_squares = build_attack_mask(gs, msks);
    *new_gs = *gs;
    char piece = line[0]; 
    int  index = set_index(piece);
    int check_index = index + 1;
    uint64_t pieces = set_pieces(gs, index);
    int move_square= ((int) (line[3] - 49)) * 8 + ((int) (line[2]-97));
    while (moves_generator(gs, new_gs, msks, &index, &piece_incr, 
                    &move_incr, &pieces, &move_squares,
                        &attack_squares, cs_msk)
                    && 
                    index < check_index  ){
            
             if ( move_square == (move_incr - 1) && 
                        pin_mask_safe(&attack_squares, piece_incr) )
                return 1;
            
            *new_gs = *gs;
        }
        return 0;
 }








