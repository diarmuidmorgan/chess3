#include "parser.c"
// Abstract the parser of death 

typedef struct {

    regex_t pforwards;
    regex_t pcapturess;
    regex_t pcapturesc;
    regex_t pcmovess;
    regex_t pccapturess;
    regex_t ampccapturecl;
    regex_t ampccapturerw;
    regex_t ampcemovecl; 
    regex_t ampcemoverw;
    regex_t promom;
    regex_t promoc;
    regex_t kscastle;
    regex_t qscastle;
    regex_t ep;
} parser_cases;

parser_cases * build_regex() {

    static char pawnforwards [] = "^[a-z][1-7](_|\\+)";
    static char pawncapturesimple [] = "^[a-z]x[a-z][1-7](_|\\+)";
    static char pawncapturescomplex [] = "^[a-z][1-7]x[a-z][1-7](_|\\+)";
    static char piecemovesimple [] = "^[A-Z][a-z][1-7][_|\\+]";
    static char piececapturesimple [] = "^[A-Z]x[a-z][1-7][_|\\+]";
    static char ambiguouspiececapturecol [] = "^[A-Z][a-z]x[a-z][1-7][_|\\+]";
    static char ambiguouspiececapturerow [] = "^[A-Z][1-7]x[a-z][1-7][_|\\+]";
    static char ambiguouspiecemovecol [] = "^[A-Z][a-z][a-z][1-7][_|\\+]";
    static char ambiguouspiecemoverow [] = "^[A-Z][1-7][a-z][1-7][_|\\+]";
    static char promotionm [] = "[a-z][1-7]\\=[A-Z]";
    static char promotionc [] = "[a-z]x[a-z][1-7]\\=[A-Z]";
    static char kingsidecastle [] = "0-0_";
    static char queensidecastle [] = "0-0-0_";
    int reti;
    parser_cases * pc = malloc(sizeof(parser_cases));
    reti = regcomp(&pc->pforwards, pawnforwards, REG_EXTENDED);
    reti = regcomp(&pc->pcapturess, pawncapturesimple, REG_EXTENDED);
    reti = regcomp(&pc->pcapturesc, pawncapturescomplex, REG_EXTENDED);
    reti = regcomp(&pc->pcmovess, piecemovesimple, REG_EXTENDED);
    reti = regcomp(&pc->pccapturess, piececapturesimple, REG_EXTENDED);
    reti = regcomp(&pc->ampccapturecl, ambiguouspiececapturecol, REG_EXTENDED);
    reti = regcomp(&pc->ampccapturerw, ambiguouspiececapturerow, REG_EXTENDED);
    reti = regcomp(&pc->ampcemovecl, ambiguouspiecemovecol, REG_EXTENDED);
    reti = regcomp(&pc->ampcemoverw, ambiguouspiecemoverow, REG_EXTENDED);
    reti = regcomp(&pc->promom, promotionm, REG_EXTENDED);
     reti = regcomp(&pc->promoc, promotionc, REG_EXTENDED);
    reti = regcomp(&pc->kscastle, kingsidecastle, REG_EXTENDED);
    reti = regcomp(&pc->qscastle, queensidecastle, REG_EXTENDED);
    return pc;

}

int simple_pawn_move(char * line, GS * gs, GS * new_gs, 
                    uint64_t * msks, CS_mask * cs_msk){
        printf("MATCHED SIMPLE PAWN MOVE\n");
        int piece_incr=0;
        int move_incr = 0;
        uint64_t move_squares = 0LL;
        int move_square = ((int) (line[1]-49)) * 8 + ((int) line[0] -97);
        int index = 1;
        uint64_t pieces = gs->pawns[gs->color];
        uint64_t attack_squares = build_attack_mask(gs, msks);
        int count = 0;
        
        while ( pieces != 0LL && moves_generator(gs, new_gs, msks, &index, &piece_incr, 
                        &move_incr, &pieces, &move_squares,
                            &attack_squares, cs_msk) && index < 2){
                printf("ITERATION %d PC INCR %d MV INCR %d\n", count++, piece_incr, move_incr);
                if ( move_square == (move_incr - 1) 
                && 
               pin_mask_safe(&attack_squares, piece_incr) ) 
                    
                            return 1;

                *new_gs = *gs;
            }
            return 0;      
        }

int simple_pawn_capture(char * line, GS * gs, GS * new_gs, 
                    uint64_t * msks, CS_mask * cs_msk){
        printf("MATCHED SIMPLE PAWN CAPTURE\n");
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
             (piece_incr -1) % 8 == col   
             //pin_mask_safe(&attack_squares, piece_incr)

             )
                
                return 1;

            *new_gs = *gs;
        }
        return 0;
    }

int  complex_pawn_capture(char * line, GS * gs, GS * new_gs, 
                    uint64_t * msks, CS_mask * cs_msk){
printf("MATCHED COMPLEX PAWN CAPTURE\n");
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

    printf("MATCHED SIMPLE PIECE MOVE\n");
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

    printf("MATCHED SIMPLE PIECE CAPTURE\n");
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

 int ambiguous_piece_capture_rank(char * line, GS * gs, GS * new_gs, 
                    uint64_t * msks, CS_mask * cs_msk){
    printf("MATCHED AMBIGUOUS PIECE CAPTURE RANK\n");
    int piece_incr=0;
    int move_incr = 0;
    uint64_t move_squares = 0LL;
    uint64_t attack_squares = build_attack_mask(gs, msks);
    *new_gs = *gs;
    char piece = line[0]; 
    int  index = set_index(piece);
    uint64_t pieces = set_pieces(gs, index);
    int check_index = index + 1;
    int move_square= ((int) (line[4] - 49)) * 8 + ((int) (line[3]-97)); 
    int rank = ((int) (line[1]-49));
    while (moves_generator(gs, new_gs, msks, &index, &piece_incr, 
                    &move_incr, &pieces, &move_squares,
                        &attack_squares, cs_msk)
                        && 
                        index < check_index){

             if ( move_square == (move_incr - 1) && 
                        
                        pin_mask_safe(&attack_squares, piece_incr)
                        
                        && ((piece_incr -1) / 8) == rank )
                
                return 1;
            *new_gs = *gs;
        }
    return 0;
}

 int ambiguous_piece_capture_col(char * line, GS * gs, GS * new_gs, 
                    uint64_t * msks, CS_mask * cs_msk){
    printf("MATCHED AMBI PIECE CAPTURE COL\n");
    int piece_incr=0;
    int move_incr = 0;
    uint64_t move_squares = 0LL;
    uint64_t attack_squares = build_attack_mask(gs, msks);
    *new_gs = *gs;
    char piece = line[0]; 
    int  index = set_index(piece);
    uint64_t pieces = set_pieces(gs, index);
    int check_index = index + 1;
    int move_square= ((int) (line[4] - 49)) * 8 + ((int) (line[3]-97)); 
    int col = ((int) (line[1]-97));

    while (moves_generator(gs, new_gs, msks, &index, &piece_incr, 
                    &move_incr, &pieces, &move_squares,
                        &attack_squares, cs_msk)
                        && index < check_index   ){
             
             if ( move_square == (move_incr - 1) && 
                        pin_mask_safe(&attack_squares, piece_incr)
                        && ((piece_incr -1) % 8) == col   )
                return 1;

            *new_gs = *gs;
        }
    return 0;
}

int ambiguous_piece_move_rank(char * line, GS * gs, GS * new_gs, 
                    uint64_t * msks, CS_mask * cs_msk){
printf("MATCHED AMBIG PIECE MV RANK\n");
    int piece_incr=0;
    int move_incr = 0;
    uint64_t move_squares = 0LL;
    uint64_t attack_squares = build_attack_mask(gs, msks);
    *new_gs = *gs;
    char piece = line[0]; 
    int  index = set_index(piece);
    uint64_t pieces = set_pieces(gs, index);
    int check_index = index + 1;
    int move_square= ((int) (line[3] - 49)) * 8 + ((int) (line[2]-97)); 
    int rank = ((int) (line[1]-49));
    while (moves_generator(gs, new_gs, msks, &index, &piece_incr, 
                    &move_incr, &pieces, &move_squares,
                        &attack_squares, cs_msk)
                        && 
                        index < check_index){

             if ( move_square == (move_incr - 1) && 
                        
                        pin_mask_safe(&attack_squares, piece_incr)
                        
                        && ((piece_incr -1) / 8) == rank )
                
                return 1;
            *new_gs = *gs;
        }
    return 0;
}

int ambiguous_piece_move_col(char * line, GS * gs, GS * new_gs, 
                    uint64_t * msks, CS_mask * cs_msk){
printf("MATCHED AMBIG PIECE MOVE COL \n");
    int piece_incr=0;
    int move_incr = 0;
    uint64_t move_squares = 0LL;
    uint64_t attack_squares = build_attack_mask(gs, msks);
    *new_gs = *gs;
    char piece = line[0]; 
    int  index = set_index(piece);
    uint64_t pieces = set_pieces(gs, index);
    int check_index = index + 1;
    int move_square= ((int) (line[3] - 49)) * 8 + ((int) (line[2]-97)); 
    int col = ((int) (line[1]-97));

    while (moves_generator(gs, new_gs, msks, &index, &piece_incr, 
                    &move_incr, &pieces, &move_squares,
                        &attack_squares, cs_msk)
                        && index < check_index   ){
             
             if ( move_square == (move_incr - 1) && 
                        pin_mask_safe(&attack_squares, piece_incr)
                        && ((piece_incr -1) % 8) == col   )
                return 1;

            *new_gs = *gs;
        }
    return 0;
}

int promotion_move (char * line, GS * gs, GS * new_gs, 
                    uint64_t * msks, CS_mask * cs_msk){
printf("MATCHED PROMOTION MV\n");
        uint64_t pieces = gs->pawns[gs->color];
        int piece_incr=0;
        int move_incr = 0;
        uint64_t move_squares = 0LL;
        uint64_t attack_squares = build_attack_mask(gs, msks);
        int move_square = ((int) (line[1]-49)) * 8 + ((int) line[0] -97);
        int index = 1;
        while (moves_generator(gs, new_gs, msks, &index, &piece_incr, 
                    &move_incr, &pieces, &move_squares,
                        &attack_squares, cs_msk)
                        && index < 2){
            
            if (index == 1 && move_square == (move_incr - 1)
                && pin_mask_safe(&attack_squares, piece_incr) ){
                // this should be handled in the game state logic, not here
                int color = gs->color;
                new_gs->pawns[color] &= ~(1LL << (move_incr -1));
                if (line[4] == 'Q')
                    new_gs->queens[color] |= (1LL << (move_incr -1)); 
                if (line[4] == 'R')
                    new_gs->rooks[color] |= (1LL << (move_incr -1)); 
                if (line[4] == 'K')
                    new_gs->knights[color] |= (1LL << (move_incr -1));
                if (line[4] == 'B')
                    new_gs->bishops[color] |= (1LL << (move_incr -1));  
                return 1;

                }
            *new_gs = *gs;
        }
    return 0;
    }
int promotion_capture (char * line, GS * gs, GS * new_gs, 
                    uint64_t * msks, CS_mask * cs_msk){
printf("MATCHED PROMOTION CAPTUR\n");
        uint64_t pieces = gs->pawns[gs->color];
        int piece_incr=0;
        int move_incr = 0;
        uint64_t move_squares = 0LL;
        uint64_t attack_squares = build_attack_mask(gs, msks);
        int move_square = ((int) (line[3]-49)) * 8 + ((int) line[2] -97);
        int col = ((int) (line[0]-97));
        int index = 1;
        while (moves_generator(gs, new_gs, msks, &index, &piece_incr, 
                    &move_incr, &pieces, &move_squares,
                        &attack_squares, cs_msk)
                        && index < 2){
            
            if ( move_square == (move_incr - 1)
                && pin_mask_safe(&attack_squares, piece_incr) && ((piece_incr -1) % 8 == col)){
                // this should be handled in the game state logic, not here
                int color = gs->color;
                new_gs->pawns[color] &= ~(1LL << (move_incr -1));
                if (line[5] == 'Q')
                    new_gs->queens[color] |= (1LL << (move_incr -1)); 
                if (line[5] == 'R')
                    new_gs->rooks[color] |= (1LL << (move_incr -1)); 
                if (line[5] == 'K')
                    new_gs->knights[color] |= (1LL << (move_incr -1));
                if (line[5] == 'B')
                    new_gs->bishops[color] |= (1LL << (move_incr -1));  
                return 1;

                }
            *new_gs = *gs;
        }
    return 0;
    }

int kscastle_parse (char * line, GS * gs, GS * new_gs, 
                    uint64_t * msks, CS_mask * cs_msk){
printf("MATCHED KSIDE CASTLE\n");
    uint64_t pieces = gs->pawns[gs->color];
    int piece_incr=0;
    int move_incr = 0;
    uint64_t move_squares = 0LL;
    uint64_t attack_squares = build_attack_mask(gs, msks);
    int index = 7;
    int check_index = 8;
     while (moves_generator(gs, new_gs, msks, &index, &piece_incr, 
                    &move_incr, &pieces, &move_squares,
                        &attack_squares, cs_msk)
                    && index < check_index){
             
                return 1;
                    }
    return 0;

}
int qscastle_parse (char * line, GS * gs, GS * new_gs, 
                    uint64_t * msks, CS_mask * cs_msk){
printf("MATCHED QSIDE CASTLE\n");
    uint64_t pieces = gs->pawns[gs->color];
    int piece_incr=0;
    int move_incr = 0;
    uint64_t move_squares = 0LL;
    uint64_t attack_squares = build_attack_mask(gs, msks);
    int index = 8;
    int check_index = 9;
     while (moves_generator(gs, new_gs, msks, &index, &piece_incr, 
                    &move_incr, &pieces, &move_squares,
                        &attack_squares, cs_msk)
                    && index < check_index){
             
                return 1;
                    }
    return 0;

}








                    








