#include "gamestate_generator.c"
#include <regex.h>


//write the parser of death :(
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

    static char pieces[] = {'Z', 'P', 'R', 'K', 'B', 'Q', 'K'};
    for (int i=0; i<6; i++){
        if (pieces[i] == c){
            return i;
        }

    }
    return -1;
}

int pin_mask_safe (uint64_t * pin_mask, int piece_incr){

    if ( (*pin_mask | 1LL << (piece_incr - 1)) == *pin_mask) return 0;
    return 1;

}



// this is not going to work, I don't think.




int parse_line(char * line, GS * gs, GS * new_gs, uint64_t * msks, CS_mask * cs_msk) {

    static char pawnforwards [] = "^[a-z][1-7](_|\\+)";
    static char pawncapturesimple [] = "^[a-z]x[a-z][1-7](_|\\+)";
    static char pawncapturescomplex [] = "^[a-z][1-7]x[a-z][1-7](_|\\+)";
    static char piecemovesimple [] = "^[A-Z][a-z][1-7][_|\\+]";
    static char piececapturesimple [] = "^[A-Z]x[a-z][1-7][_|\\+]";
    static char ambiguouspiececapturecol [] = "^[A-Z][a-z]x[a-z][1-7][_|\\+]";
    static char ambiguouspiececapturerow [] = "^[A-Z][1-7]x[a-z][1-7][_|\\+]";
    static char ambiguouspiecemovecol [] = "^[A-Z][a-z][a-z][1-7][_|\\+]";
    static char ambiguouspiecemoverow [] = "^[A-Z][1-7][a-z][1-7][_|\\+]";
    static char promotion [] = "[a-z][1-7]\\=[A-Z]";
    static char kingsidecastle [] = "0-0_";
    static char queensidecastle [] = "0-0-0_";
    int reti;

    regex_t pforwards;
    reti = regcomp(&pforwards, pawnforwards, 0);
    regex_t pcapturess;
    reti = regcomp(&pcapturess, pawncapturesimple, 0);
    regex_t pcapturesc;
    reti = regcomp(&pcapturesc, pawncapturescomplex, 0);
    regex_t pcmovess;
    reti = regcomp(&pcmovess, piecemovesimple, 0);
    regex_t pccapturess;
    reti = regcomp(&pccapturess, piececapturesimple, 0);
    regex_t ampccapturecl;
    reti = regcomp(&ampccapturecl, ambiguouspiececapturecol, 0);
    regex_t ampccapturerw;
    reti = regcomp(&ampccapturerw, ambiguouspiececapturerow, 0);
    regex_t ampcemovecl; 
    reti = regcomp(&ampcemovecl, ambiguouspiecemovecol, 0);
    regex_t ampcemoverw;
    reti = regcomp(&ampcemoverw, ambiguouspiecemoverow, 0);
    regex_t promo;
    reti = regcomp(&promo, promotion, 0);
    regex_t kscastle;
    reti = regcomp(&kscastle, kingsidecastle, 0);
    regex_t qscastle;
    reti = regcomp(&qscastle, queensidecastle, 0);

    int result;
    *new_gs = *gs;
    uint64_t move_squares = 0LL;
    int piece_incr = 0;
    int move_incr = 0;
    int index = 0;
    uint64_t attack_squares = build_pin_mask(gs, msks);
    
    uint64_t pieces = 0LL;
    
   

    //try to match with a pawn forwards move. Actually fuck this just modify the generator.
    // e4, e3
    result = regexec(&pforwards, line, 0, NULL, 0);
    if (!result){
        int move_square = ((int) (line[1]-48)) * 8 + ((int) line[0] -97);
        index = 1;
        while ( moves_generator(gs, new_gs, msks, &index, &piece_incr, 
                    &move_incr, &pieces, &move_squares,
                        &attack_squares, cs_msk)
                    && index < 2){

            if (index == 1 && 
                        move_square == (move_incr - 1)
                             && ( (piece_incr-1) % 8 == ((int) line[0] -97) 
                     && pin_mask_safe(&attack_squares, piece_incr) ) )
                return 1;

            *new_gs = *gs;
        }
    }
    
    //try to match with pawn captures
    // exe4
    result = regexec(&pcapturess, line, 0, NULL, 0);
    if (!result){
        int move_square = ((int) (line[3] - 48)) * 8 + ((int) (line[2]-97)); 
        int col = ((int) (line[0]-97));
        index = 1;
        
        while (moves_generator(gs, new_gs, msks, &index, &piece_incr, 
                    &move_incr, &pieces, &move_squares,
                        &attack_squares, cs_msk)
                    &&
                      index < 2  ){
             if (index == 1 && move_square == (move_incr - 1) && (piece_incr -1) % 8 == col &&
                pin_mask_safe(&attack_squares, piece_incr) )
                return 1;

            *new_gs = *gs;
        }
    }
    
    //e.g e3xe4
    result = regexec(&pcapturesc, line, 0, NULL, 0);
    if (!result){
        int move_square= ((int) (line[3] - 48)) * 8 + ((int) (line[2]-97)); 
       int position =((int) (line[0] - 48)) * 8 + ((int) (line[1]-97)); 
       index = 1;
        
        while (moves_generator(gs, new_gs, msks, &index, &piece_incr, 
                    &move_incr, &pieces, &move_squares,
                        &attack_squares, cs_msk)
                        &&
                     index < 2    ){
             if (index == 1 && move_square == (move_incr - 1) && (piece_incr -1) == position &&
                pin_mask_safe(&attack_squares, piece_incr) )
                return 1;
            
            *new_gs = *gs;
        }
    }
    //e.g Ne4
    result = regexec(&pcmovess, line, 0, NULL, 0);
    if (!result){
        int move_square= ((int) (line[2] - 48)) * 8 + ((int) (line[1]-97)); 
        char piece = line[0]; 
        int check_index = set_index(piece);
        index = check_index;

        while (moves_generator(gs, new_gs, msks, &index, &piece_incr, 
                    &move_incr, &pieces, &move_squares,
                        &attack_squares, cs_msk)
                    && 
                    index < check_index + 1  ){

             if (index == check_index && move_square == (move_incr - 1) && 
                pin_mask_safe(&attack_squares, piece_incr) )
                return 1;
            
            *new_gs = *gs;
        }
    }
    // e.g Nxe4
    result = regexec(&pccapturess, line, 0, NULL, 0);
    if (!result){
        int move_square= ((int) (line[3] - 48)) * 8 + ((int) (line[2]-97)); 
        char piece = line[0]; 
        int check_index = set_index(piece);
        index = check_index;

        while (moves_generator(gs, new_gs, msks, &index, &piece_incr, 
                    &move_incr, &pieces, &move_squares,
                        &attack_squares, cs_msk)
                        && 
                        index < check_index + 1 ){

             if (index == check_index && move_square == (move_incr - 1) && 
                        pin_mask_safe(&attack_squares, piece_incr) )
                return 1;
            
            *new_gs = *gs;
        }
    }
    //e.g N4xd3
    result = regexec(&ampccapturerw, line, 0, NULL, 0);
    if (!result){
        int move_square= ((int) (line[4] - 48)) * 8 + ((int) (line[3]-97)); 
        char piece = line[0];
        int rank = ((int) (line[1]-48));
        int check_index = set_index(piece);
        index = check_index;

        while (moves_generator(gs, new_gs, msks, &index, &piece_incr, 
                    &move_incr, &pieces, &move_squares,
                        &attack_squares, cs_msk)
                        && 
                        index < check_index + 1){
             if ( move_square == (move_incr - 1) && 
                        
                        pin_mask_safe(&attack_squares, piece_incr)
                        
                        && ((piece_incr -1) / 8) == rank )
                
                return 1;
            *new_gs = *gs;
        }
    }
    //e.g Nexd5
     result = regexec(&ampccapturecl, line, 0, NULL, 0);
    if (!result){
        int move_square= ((int) (line[4] - 48)) * 8 + ((int) (line[3]-97)); 
        char piece = line[0];
        int col = ((int) (line[1]-97));
        int check_index = set_index(piece);
        index = check_index;

        while (moves_generator(gs, new_gs, msks, &index, &piece_incr, 
                    &move_incr, &pieces, &move_squares,
                        &attack_squares, cs_msk)
                        && index < check_index + 1  ){
             
             if (index == check_index && move_square == (move_incr - 1) && 
                        pin_mask_safe(&attack_squares, piece_incr)
                        && ((piece_incr -1) % 8) == col  
                        &&index < check_index + 1  )
                return 1;

            *new_gs = *gs;
        }
    }

    //e.g Nee4
    result = regexec(&ampcemovecl, line, 0, NULL, 0);
    if (!result){
        int move_square= ((int) (line[3] - 48)) * 8 + ((int) (line[2]-97)); 
        char piece = line[0];
        int col = ((int) (line[1]-97));
        int check_index = set_index(piece);
        index = check_index;

        while (moves_generator(gs, new_gs, msks, &index, &piece_incr, 
                    &move_incr, &pieces, &move_squares,
                        &attack_squares, cs_msk) 
                        && index < check_index + 1){
             if ( move_square == (move_incr - 1) && 
                        pin_mask_safe(&attack_squares, piece_incr)
                        && ((piece_incr -1) % 8) == col )
                return 1;
            
            *new_gs = *gs;
        }
        
    }
    //e.g N4e4
    result = regexec(&ampcemoverw, line, 0, NULL, 0);
    if (!result){
        int move_square= ((int) (line[3] - 48)) * 8 + ((int) (line[2]-97)); 
        char piece = line[0];
        int rank = ((int) (line[1]-48));
        int check_index = set_index(piece);
        index = check_index;

        while (moves_generator(gs, new_gs, msks, &index, &piece_incr, 
                    &move_incr, &pieces, &move_squares,
                        &attack_squares, cs_msk)
                    && index < check_index + 1){
             if ( move_square == (move_incr - 1) && 
                        pin_mask_safe(&attack_squares, piece_incr)
                        && ((piece_incr -1) / 8) == rank )
                return 1;
            *new_gs = *gs;
        }
        
    }

    // Promotion
    //e.g e4=Q
    // this should really be handled in the game state logic not here . . . .
    result = regexec(&promo, line, 0, NULL, 0);
    if (!result){
        int move_square = ((int) (line[1]-48)) * 8 + ((int) line[0] -97);
        index = 1;
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
    }

    //add castling here
    result = regexec(&kscastle, line, 0, NULL, 0);
    if (!result){
        int move_square= ((int) (line[3] - 48)) * 8 + ((int) (line[2]-97)); 
        char piece = line[0];
        int rank = ((int) (line[1]-48));
        int check_index = 7;
        index = check_index;

        while (moves_generator(gs, new_gs, msks, &index, &piece_incr, 
                    &move_incr, &pieces, &move_squares,
                        &attack_squares, cs_msk)
                    && index < check_index + 1){
             
                return 1;
        }
        *new_gs = *gs;
    }
    
    result = regexec(&qscastle, line, 0, NULL, 0);
    if (!result){
        int move_square= ((int) (line[3] - 48)) * 8 + ((int) (line[2]-97)); 
        char piece = line[0];
        int rank = ((int) (line[1]-48));
        int check_index = 7;
        index = check_index;

        while (moves_generator(gs, new_gs, msks, &index, &piece_incr, 
                    &move_incr, &pieces, &move_squares,
                        &attack_squares, cs_msk)
                    && index < check_index + 1){
             
                return 1;
        }
        *new_gs = *gs;
    }


    //add enpassants here...
    

    return 0;

}

void grab_game_string (char * s) {

    char filepath[] = "../data/cfriendly";
    FILE *fp;
	
	fp =fopen(filepath,"r");
	fgets(s,1000, fp);
    fclose(fp);
}

void play_game_string (char * game_str){
    GS gs = init_game_state();
    GS new_gs = gs;
    uint64_t * msks = build_mask_object();
    CS_mask * cs_msk = build_castle_masks();
    int i = 0;
    while (game_str[i]){
        print_game_state(&gs);
        new_gs = gs;
        int j = 0;
        char * m = malloc(7 * sizeof(char));
        while(game_str[i] != ' '){
            m[j] = game_str[i];
            i++;
            j++;
        }
        m[j] = '_';
        parse_line(m, &gs, &new_gs, msks, cs_msk);
        i++;
        gs = new_gs;
    }
}


int main () {

    char * s = malloc(1000 * sizeof(char));
    grab_game_string(s);
    play_game_string(s);




}