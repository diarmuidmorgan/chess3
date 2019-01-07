#include "gamestate_generator.c"
// Abstract the parser of death 
#include <regex.h>


//write the parser of death :(

// this is like the worst code ever :(

/* There are seemingly so many cases and so much code repettition to get through here.
    Just feels like a massive head ache and I would rather do this in python :(
*
*/





int string_length (char * s){
    int length = 0;
    int i=0;
    while(s[i++]) length++;
    return length;

}
int chars_to_pos(char letter, char number){

    int j = (int) (number - 49);
    int i = (int) (letter - 97);
    return (j*8) + i;
}
int is_digit(char c){
    if (c>=48 && c <=57) return 1;
    return 0;
}
int is_character(char c){

    if (c>=97 && c <=122) return 1;
    return 0;
}

int set_index (char c){

    static char pieces[] = {'Z', 'P', 'R', 'N', 'B', 'Q', 'K'};
    for (int i=0; i<7; i++){
        if (pieces[i] == c){
            //printf("%c %d\n", c, i);
            return i;
        }

    }
    return -1;
}

uint64_t set_pieces (GS * gs, int index){

    switch(index) {

        case(1) :
            return gs->pawns[gs->color];
            break;
        case(2) :
            return gs->rooks[gs->color];
            break;
        case(3) :
           
            return gs->knights[gs->color];
            break;
        case(4) :
            return gs->bishops[gs->color];
            break;
        case(5) :
            return gs->queens[gs->color];
            break;
        case(6) :
            return gs->kings[gs->color];
            break;

        default :
            return 0ULL;


    }


}

int pin_mask_safe (uint64_t * pin_mask, int piece_incr){

    if ( (*pin_mask | (1LL << (piece_incr - 1)))  == *pin_mask) return 0;
    return 1;

}


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
    regex_t fullpmove;
    regex_t fullpcapture;
} parser_cases;

parser_cases * build_regex() {

    static char pawnforwards [] = "^[a-z][1-8](_|\\+|#)";
    static char pawncapturesimple [] = "^[a-z]x[a-z][1-8](_|\\+|#)";
    static char pawncapturescomplex [] = "^[a-z][1-8]x[a-z][1-8](_|\\+|#)";
    static char piecemovesimple [] = "^[A-Z][a-z][1-8](_|\\+|#)";
    static char piececapturesimple [] = "^[A-Z]x[a-z][1-8](_|\\+|#)";
    static char ambiguouspiececapturecol [] = "^[A-Z][a-z]x[a-z][1-8](_|\\+|#)";
    static char ambiguouspiececapturerow [] = "^[A-Z][1-8]x[a-z][1-8](_|\\+|#)";
    static char ambiguouspiecemovecol [] = "^[A-Z][a-z][a-z][1-8](_|\\+|#)";
    static char ambiguouspiecemoverow [] = "^[A-Z][1-8][a-z][1-8](_|\\+|#)";
    static char fullpiecemove [] = "^[A-Z][a-z][1-8][a-z][1-8](_|\\+|#)";
    static char fullpiececapture [] = "^[A-Z][a-z][1-8]x[a-z][1-8](_|\\+|#)";
    static char promotionm [] = "^[a-z][1-8]\\=[A-Z]";
    static char promotionc [] = "^[a-z]x[a-z][1-8]\\=[A-Z]";
    static char kingsidecastle [] = "^O-O(_|\\+|#)";
    static char queensidecastle [] = "^O-O-O(_|\\+|#)";
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
    reti = regcomp(&pc->fullpmove, fullpiecemove, REG_EXTENDED);
    reti = regcomp(&pc->fullpcapture, fullpiececapture, REG_EXTENDED);
    return pc;

}

int legal_move_search(GS * gs, uint64_t * msks, CS_mask * cs_msk, Zob * z){
    //printf("STARRTING SEARCH\n");
    uint64_t pieces = 0LL;
    int index = 0;
    int piece_incr = 0;
    int move_incr = 0;
    int value;
    int search_return;
    uint64_t attack_squares = 0LL;
    uint64_t move_squares = 0LL;
    //printf("GLOBAL VARS ALL FLUSHED\n");
    GS new_gs = *gs;
    int r_color = (gs->color + 1) % 2;
    //new_gs->enpassants[color] = 0LL;
	new_gs.enpassants[r_color] = 0LL;
    
    
     while (index < 6 && moves_generator(gs, &new_gs, msks, &index, &piece_incr,
      &move_incr, &pieces, &move_squares, &attack_squares, cs_msk, z)){
         //printf("INDEX %d\n", index);
          uint64_t king = new_gs.kings[new_gs.color];
          if (king == 0LL) return 0;
          new_gs = *gs;
     }
     return 1;



}

int simple_pawn_move(char * line, GS * gs, GS * new_gs, 
                    uint64_t * msks, CS_mask * cs_msk, Zob * z){
       // printf("MATCHED SIMPLE PAWN MOVE\n");
        int piece_incr=0;
        int move_incr = 0;
        uint64_t move_squares = 0LL;
        int move_square = ((int) (line[1]-49)) * 8 + ((int) line[0] -97);
        int index = 1;
        uint64_t pieces = gs->pawns[gs->color];
        
        uint64_t attack_squares = build_pin_mask(gs, msks);
        int count = 0;
        *new_gs = *gs;
        //printf("BUILT AND READY TO GO\n");
        //binary_print_board(pieces);   
        while (  moves_generator(gs, new_gs, msks, &index, &piece_incr, 
                        &move_incr, &pieces, &move_squares,
                            &attack_squares, cs_msk, z) && index < 2){
               // printf("ITERATION %d PC INCR %d MV INCR %d\n", count++, piece_incr, move_incr);
                if ( move_square == (move_incr - 1) 
                && 
               legal_move_search(new_gs, msks, cs_msk, z) ) 
                    
                            return 1;
               

                *new_gs = *gs;
            }
            return 0;      
        }

int simple_pawn_capture(char * line, GS * gs, GS * new_gs, 
                    uint64_t * msks, CS_mask * cs_msk, Zob * z){
       // printf("MATCHED SIMPLE PAWN CAPTURE\n");
        int move_square = ((int) (line[3] - 49)) * 8 + ((int) (line[2]-97)); 
        int col = ((int) (line[0]-97));
        int index = 1;
        int piece_incr=0;
        int move_incr = 0;
        //binary_print_board(gs->rooks[(gs->color + 1) % 2]);
        uint64_t move_squares = 0LL;
        uint64_t pieces = gs->pawns[gs->color];
        uint64_t attack_squares = build_pin_mask(gs, msks);
        *new_gs = *gs;
       
        while (moves_generator(gs, new_gs, msks, &index, &piece_incr, 
                    &move_incr, &pieces, &move_squares,
                        &attack_squares, cs_msk, z)
                 &&
                        index < 2  ){
            
            
             if ( move_square == (move_incr - 1) && 
             (piece_incr -1) % 8 == col   
              && legal_move_search(new_gs, msks, cs_msk,z)

             )
                
                return 1;

            *new_gs = *gs;
        }
        *new_gs = *gs;
       // binary_print_board(gs->enpassants[gs->color]);
        uint64_t pawn_square = 0LL;
        int pawn_incr = 0;
        uint64_t target_square = 0LL;
        uint64_t enpassant_square = 0LL;
        int pawn_square_number = 0;
        uint64_t pawns = *gs->pawns;
        int target_square_number;
        while (enpassant_generator_has_next(gs, &pawn_incr, &pawn_square_number, &target_square_number, &pawns, &pawn_square,
            &enpassant_square, &target_square)) {
            enpassant_generator_next(new_gs, &pawn_square, 
                                &target_square, &enpassant_square);
               // printf("GOT ONE\n");
               // printf("%d %d %d %d\n", target_square_number, move_square, pawn_square_number % 8, col);
                if (target_square_number == move_square && pawn_square_number % 8 == col) return 1;
                else
                    *new_gs = *gs;
            }




        return 0;
    }

int  complex_pawn_capture(char * line, GS * gs, GS * new_gs, 
                    uint64_t * msks, CS_mask * cs_msk, Zob * z){
//printf("MATCHED COMPLEX PAWN CAPTURE\n");
    int index = 1;
    int piece_incr=0;
    int move_incr = 0;
    uint64_t move_squares = 0LL;
    uint64_t pieces = gs->pawns[gs->color];
    uint64_t attack_squares = build_pin_mask(gs, msks);
    *new_gs = *gs;
    int move_square= ((int) (line[3] - 49)) * 8 + ((int) (line[2]-97)); 
    int position =((int) (line[0] - 49)) * 8 + ((int) (line[1]-97)); 

    while (moves_generator(gs, new_gs, msks, &index, &piece_incr, 
                    &move_incr, &pieces, &move_squares,
                        &attack_squares, cs_msk, z)
                        &&
                     index < 2    ){
        
        if (move_square == (move_incr - 1) && 
                (piece_incr -1) == position &&
            legal_move_search(new_gs, msks, cs_msk, z) )
                
                    return 1;
            
            *new_gs = *gs;
              
        }
    return 0;
}


int simple_piece_move (char * line, GS * gs, GS * new_gs, 
                    uint64_t * msks, CS_mask * cs_msk, Zob * z){

   // printf("MATCHED SIMPLE PIECE MOVE\n");
    int piece_incr=0;
    int move_incr = 0;
    uint64_t move_squares = 0LL;
    uint64_t attack_squares = build_pin_mask(gs, msks);
    
    *new_gs = *gs;
    char piece = line[0]; 
    
    int  index = set_index(piece);
    
    int check_index = index + 1;
    uint64_t pieces = set_pieces(gs, index);
    int move_square = ((int) (line[2]-49)) * 8 + ((int) line[1] -97); 
   // binary_print_board(pieces);
  // printf("WORKING\n");
    while (index < check_index && moves_generator(gs, new_gs, msks, &index, &piece_incr, 
                    &move_incr, &pieces, &move_squares,
                        &attack_squares, cs_msk, z)
    
                      ){
            //printf("ITERATING\n");
           //printf("%d %d\n", move_square, move_incr);

            if ( move_square == (move_incr - 1) 
                 && legal_move_search(new_gs, msks, cs_msk, z ) ) {
               // printf("SUCCESS\n");
                return 1;
                 }
            *new_gs = *gs;
        }
        return 0;
 }

int simple_piece_capture (char * line, GS * gs, GS * new_gs, 
                    uint64_t * msks, CS_mask * cs_msk, Zob * z){

   // printf("MATCHED SIMPLE PIECE CAPTURE\n");
    int piece_incr=0;
    int move_incr = 0;
    uint64_t move_squares = 0LL;
    uint64_t attack_squares = build_pin_mask(gs, msks);
    *new_gs = *gs;
    char piece = line[0]; 
    int  index = set_index(piece);
    int check_index = index + 1;
    uint64_t pieces = set_pieces(gs, index);
    int move_square= ((int) (line[3] - 49)) * 8 + ((int) (line[2]-97));
    while (moves_generator(gs, new_gs, msks, &index, &piece_incr, 
                    &move_incr, &pieces, &move_squares,
                        &attack_squares, cs_msk, z)
                    && 
                    index < check_index  ){
            
             if ( move_square == (move_incr - 1) && 
                        legal_move_search(new_gs, msks, cs_msk, z) )
                return 1;
            
            *new_gs = *gs;
        }
        return 0;
 }

 int ambiguous_piece_capture_rank(char * line, GS * gs, GS * new_gs, 
                    uint64_t * msks, CS_mask * cs_msk, Zob * z){
  //  printf("MATCHED AMBIGUOUS PIECE CAPTURE RANK\n");
    int piece_incr=0;
    int move_incr = 0;
    uint64_t move_squares = 0LL;
    uint64_t attack_squares = build_pin_mask(gs, msks);
    *new_gs = *gs;
    char piece = line[0]; 
    int  index = set_index(piece);
    uint64_t pieces = set_pieces(gs, index);
    int check_index = index + 1;
    int move_square= ((int) (line[4] - 49)) * 8 + ((int) (line[3]-97)); 
    int rank = ((int) (line[1]-49));
    while (moves_generator(gs, new_gs, msks, &index, &piece_incr, 
                    &move_incr, &pieces, &move_squares,
                        &attack_squares, cs_msk, z)
                        && 
                        index < check_index){

             if ( move_square == (move_incr - 1) && 
                         ((piece_incr -1) / 8) == rank &&
                        legal_move_search(new_gs, msks, cs_msk, z)
                        
                       )
                
                return 1;
            *new_gs = *gs;
        }
    return 0;
}

 int ambiguous_piece_capture_col(char * line, GS * gs, GS * new_gs, 
                    uint64_t * msks, CS_mask * cs_msk, Zob * z){
   // printf("MATCHED AMBI PIECE CAPTURE COL\n");
    int piece_incr=0;
    int move_incr = 0;
    uint64_t move_squares = 0LL;
    uint64_t attack_squares = build_pin_mask(gs, msks);
    *new_gs = *gs;
    char piece = line[0]; 
    int  index = set_index(piece);
    uint64_t pieces = set_pieces(gs, index);
    int check_index = index + 1;
    int move_square= ((int) (line[4] - 49)) * 8 + ((int) (line[3]-97)); 
    int col = ((int) (line[1]-97));

    while (moves_generator(gs, new_gs, msks, &index, &piece_incr, 
                    &move_incr, &pieces, &move_squares,
                        &attack_squares, cs_msk, z)
                        && index < check_index   ){
             
             if ( move_square == (move_incr - 1) && 
                        
                        ((piece_incr -1) % 8) == col &&
                         legal_move_search(new_gs, msks, cs_msk, z)  )
                return 1;

            *new_gs = *gs;
        }
    return 0;
}

int ambiguous_piece_move_rank(char * line, GS * gs, GS * new_gs, 
                    uint64_t * msks, CS_mask * cs_msk, Zob * z){
//printf("MATCHED AMBIG PIECE MV RANK\n");
    int piece_incr=0;
    int move_incr = 0;
    uint64_t move_squares = 0LL;
    uint64_t attack_squares = build_pin_mask(gs, msks);
    *new_gs = *gs;
    char piece = line[0]; 
    int  index = set_index(piece);
    uint64_t pieces = set_pieces(gs, index);
    int check_index = index + 1;
    int move_square= ((int) (line[3] - 49)) * 8 + ((int) (line[2]-97)); 
    int rank = ((int) (line[1]-49));
    while (moves_generator(gs, new_gs, msks, &index, &piece_incr, 
                    &move_incr, &pieces, &move_squares,
                        &attack_squares, cs_msk, z)
                        && 
                        index < check_index){

             if ( move_square == (move_incr - 1) && 
                        
                        
                        
                     ((piece_incr -1) / 8) == rank && 
                      legal_move_search(new_gs, msks, cs_msk, z))
                
                return 1;
            *new_gs = *gs;
        }
    return 0;
}

int ambiguous_piece_move_col(char * line, GS * gs, GS * new_gs, 
                    uint64_t * msks, CS_mask * cs_msk, Zob * z){
//printf("MATCHED AMBIG PIECE MOVE COL \n");
    int piece_incr=0;
    int move_incr = 0;
    uint64_t move_squares = 0LL;
    uint64_t attack_squares = build_pin_mask(gs, msks);
    *new_gs = *gs;
    char piece = line[0]; 
    int  index = set_index(piece);
    uint64_t pieces = set_pieces(gs, index);
    int check_index = index + 1;
    int move_square= ((int) (line[3] - 49)) * 8 + ((int) (line[2]-97)); 
    int col = ((int) (line[1]-97));

    while (moves_generator(gs, new_gs, msks, &index, &piece_incr, 
                    &move_incr, &pieces, &move_squares,
                        &attack_squares, cs_msk, z)
                        && index < check_index   ){
             
             if ( move_square == (move_incr - 1) && 
                       ((piece_incr -1) % 8) == col &&
                        legal_move_search(new_gs, msks, cs_msk, z)  )
                return 1;

            *new_gs = *gs;
        }
    return 0;
}

int full_piece_move(char * line, GS * gs, GS * new_gs, 
                    uint64_t * msks, CS_mask * cs_msk, Zob * z){
//printf("MATCHED AMBIG PIECE MOVE COL \n");
    int piece_incr=0;
    int move_incr = 0;
    uint64_t move_squares = 0LL;
    uint64_t attack_squares = build_pin_mask(gs, msks);
    *new_gs = *gs;
    char piece = line[0]; 
    int  index = set_index(piece);
    uint64_t pieces = set_pieces(gs, index);
    int check_index = index + 1;
    int move_square= ((int) (line[4] - 49)) * 8 + ((int) (line[3]-97)); 
    int col = ((int) (line[1]-97));
    int rank = ((int) (line[2]) - 49);
    int origin_square = rank * 8 + col;

    while (moves_generator(gs, new_gs, msks, &index, &piece_incr, 
                    &move_incr, &pieces, &move_squares,
                        &attack_squares, cs_msk, z)
                        && index < check_index   ){
             
             if ( move_square == (move_incr - 1) && 
                       (piece_incr -1)  == origin_square &&
                        legal_move_search(new_gs, msks, cs_msk, z)  )
                return 1;

            *new_gs = *gs;
        }
    return 0;
}

int full_piece_capture(char * line, GS * gs, GS * new_gs, 
                    uint64_t * msks, CS_mask * cs_msk, Zob * z){
    printf("MATCHED AMBIG PIECE MOVE COL \n");
    int piece_incr=0;
    int move_incr = 0;
    uint64_t move_squares = 0LL;
    uint64_t attack_squares = build_pin_mask(gs, msks);
    *new_gs = *gs;
    char piece = line[0]; 
    int  index = set_index(piece);
    uint64_t pieces = set_pieces(gs, index);
    int check_index = index + 1;
    int move_square= ((int) (line[5] - 49)) * 8 + ((int) (line[4]-97)); 
    int col = ((int) (line[1]-97));
    int rank = ((int) (line[2]) - 49);
    int origin_square = rank * 8 + col;

    while (moves_generator(gs, new_gs, msks, &index, &piece_incr, 
                    &move_incr, &pieces, &move_squares,
                        &attack_squares, cs_msk, z)
                        && index < check_index   ){
             
             if ( move_square == (move_incr - 1) && 
                       (piece_incr -1)  == origin_square &&
                        legal_move_search(new_gs, msks, cs_msk, z)  )
                return 1;

            *new_gs = *gs;
        }
    return 0;
}


int promotion_move (char * line, GS * gs, GS * new_gs, 
                    uint64_t * msks, CS_mask * cs_msk, Zob * z){
//printf("MATCHED PROMOTION MV\n");
        uint64_t pieces = gs->pawns[gs->color];
        int piece_incr=0;
        int move_incr = 0;
        uint64_t move_squares = 0LL;
        uint64_t attack_squares = build_pin_mask(gs, msks);
        int move_square = ((int) (line[1]-49)) * 8 + ((int) line[0] -97);
        int index = 1;
        while (moves_generator(gs, new_gs, msks, &index, &piece_incr, 
                    &move_incr, &pieces, &move_squares,
                        &attack_squares, cs_msk, z)
                        && index < 2){
            
            if (move_square == (move_incr - 1)
                && pin_mask_safe(&attack_squares, piece_incr) ){
                // this should be handled in the game state logic, not here
                int color = gs->color;
                new_gs->pawns[color] &= ~(1LL << (move_square));
                new_gs->pieces[color] |= (1LL << move_square);
                new_gs->all_pieces |= (1LL << move_square);
                if (line[3] == 'Q')
                    new_gs->queens[color] |= (1LL << (move_incr -1)); 
                if (line[3] == 'R')
                    new_gs->rooks[color] |= (1LL << (move_incr -1)); 
                if (line[3] == 'N')
                    new_gs->knights[color] |= (1LL << (move_incr -1));
                if (line[3] == 'B')
                    new_gs->bishops[color] |= (1LL << (move_incr -1));
               
                //binary_print_board(new_gs->queens[gs->color]);
                return 1;

                }
            *new_gs = *gs;
        }
    return 0;
    }
int promotion_capture (char * line, GS * gs, GS * new_gs, 
                    uint64_t * msks, CS_mask * cs_msk, Zob * z){
//printf("MATCHED PROMOTION CAPTUR\n");
        uint64_t pieces = gs->pawns[gs->color];
        int piece_incr=0;
        int move_incr = 0;
        uint64_t move_squares = 0LL;
        uint64_t attack_squares = build_pin_mask(gs, msks);
        int move_square = ((int) (line[3]-49)) * 8 + ((int) line[2] -97);
        int col = ((int) (line[0]-97));
        int index = 1;
        while (moves_generator(gs, new_gs, msks, &index, &piece_incr, 
                    &move_incr, &pieces, &move_squares,
                        &attack_squares, cs_msk, z)
                        && index < 2){
            
            if ( move_square == (move_incr - 1)
                && pin_mask_safe(&attack_squares, piece_incr) && ((piece_incr -1) % 8 == col)){
                // this should be handled in the game state logic, not here
               int color = gs->color;
                new_gs->pawns[color] &= ~(1LL << (move_square));
                new_gs->pieces[color] |= (1LL << move_square);
                new_gs->all_pieces |= (1LL << move_square);
                if (line[5] == 'Q')
                    new_gs->queens[color] |= (1LL << (move_incr -1)); 
                if (line[5] == 'R')
                    new_gs->rooks[color] |= (1LL << (move_incr -1)); 
                if (line[5] == 'N')
                    new_gs->knights[color] |= (1LL << (move_incr -1));
                if (line[5] == 'B')
                    new_gs->bishops[color] |= (1LL << (move_incr -1));
               
                //binary_print_board(new_gs->queens[gs->color]);
                return 1;

                }
            *new_gs = *gs;
        }
    return 0;
    }

int kscastle_parse (char * line, GS * gs, GS * new_gs, 
                    uint64_t * msks, CS_mask * cs_msk, Zob * z){
//printf("MATCHED KSIDE CASTLE\n");
    uint64_t pieces = gs->pawns[gs->color];
    int piece_incr=0;
    int move_incr = 0;
    uint64_t move_squares = 0LL;
    uint64_t attack_squares = 0LL;
    int index = 7;
    int check_index = 8;
     while (index < check_index  && moves_generator(gs, new_gs, msks, &index, &piece_incr, 
                    &move_incr, &pieces, &move_squares,
                        &attack_squares, cs_msk, z)
                    ){
             
                return 1;
                    }
    return 0;

}
int qscastle_parse (char * line, GS * gs, GS * new_gs, 
                    uint64_t * msks, CS_mask * cs_msk, Zob * z){
//printf("MATCHED QSIDE CASTLE\n");
    uint64_t pieces = gs->pawns[gs->color];
    int piece_incr=0;
    int move_incr = 0;
    uint64_t move_squares = 0LL;
    uint64_t attack_squares = 0LL;
    int index = 8;
    int check_index = 9;
     while (index < check_index && moves_generator(gs, new_gs, msks, &index, &piece_incr, 
                    &move_incr, &pieces, &move_squares,
                        &attack_squares, cs_msk, z)
                   ){
             
                return 1;
                    }
    return 0;

}








                    








