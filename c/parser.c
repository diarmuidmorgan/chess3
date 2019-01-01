#include "move_generators2.c"
#include <regex.h>
/* There are seemingly so many cases and so much code repettition to get through here.
    Just feels like a massive head ache and I would rather do this in python :(
*
*/

char * pawnforwards = "^[a-z]\d(\n|\+)";
char * pawncapturesimple = "^[a-z]x[a-z]\d(\n|\+)";
char * pawncapturescomplex = "^[a-z]\dx[a-z]\d(\n|\+)";
char * piecemovesimple = "^[A-Z][a-z]\d[\n|\+]";
char * piececapturesimple = "^[A-Z]x[a-z]\d[\n|\+]";
char * ambiguouspiececapturecol= "^[A-Z][a-z]x[a-z]\d[\n|\+]";
char * ambiguouspiececapturerow = "^[A-Z]\dx[a-z]\d[\n|\+]";
char * ambiguouspiecemovecol= "^[A-Z][a-z][a-z]\d[\n|\+]";
char * ambiguouspiecemoverow = "^[A-Z]\d[a-z]\d[\n|\+]";
char * promotion = "[a-z]\d\=[A-Z]";
char * kingsidecastle = "0-0";
char * queensidecastle = "0-0-0";
int reti;



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

int check_pin_mask_safe (uint64_t * pin_mask, int piece_incr){

    if ( (pin_mask | 1LL << (piece_incr - 1)) == pin_mask) return 0;
    return 1;

}



// this is not going to work, I don't think.
void grab_game_string (char * s) {

    char filepath[] = "../data/cfriendly";
    FILE *fp;
	
	fp =fopen(filepath,"r");
	fgets(s,1000, fp);
    fclose(fp);
}

void play_game_string (char * game_str){
    GS gs = init_game_state();
    uint64_t * msks = build_mask_object();
    int i = 0;
    while (game_str[i]){
        print_game_state(&gs);
        int j = 0;
        char m[7];
        while(game_str[i] != ' '){
            m[j] = game_str[i];
            i++;
            j++;
        }
        m[j] = '\n';
        gs = parse_line(&m, &gs, msks);
        i++;
    }
}


GS parse_line(char * line, GS * gs, uint64_t * msks) {

    regex_t pforwards;
    reti = recomp(&pforwards, pawnforwards, 0);
    regex_t pcapturess;
    reti = recomp(&pcapturess, pawncapturesimple, 0);
    regex_t pcapturesc;
    reti = recomp(&pcapturesc, pawncapturescomplex, 0);
    regex_t pcmovess;
    reti = recomp(&pcmovess, piecemovesimple, 0);
    regex_t pccapturess;
    reti = recomp(&pccapturess, piececapturesimple, 0);
    regex_t ampccapturecl;
    reti = recomp(&ampccapturecl, ambiguouspiececapturecol, 0);
    regex_t ampccapturerw;
    reti = recomp(&ampccapturerw, ambiguouspiececapturerow, 0);
    regex_t ampcemovecl; 
    reti = recomp(&ampcemovecl, ambiguouspiecemovecol, 0);
    regex_t ampcemoverw;
    reti = recomp(&ampcemoverw, ambiguouspiecemoverow, 0);
    regex_t promo;
    reti = recomp(&promo, promotion, 0);

    int result;
    GS new_gs = *gs;
    uint64_t move_squares = 0LL;
    int piece_incr = 0;
    int move_incr = 0;
    int index = 0;
    uint64_t pin_mask = build_pin_mask(gs, msks);
    uint64_t pieces = 0LL;
   

    //try to match with a pawn forwards move. Actually fuck this just modify the generator.
    // e4, e3
    result = regexec(&pforwards, line, 0, NULL, 0);
    if (!result){
        int move_square = ((int) (line[1]-48)) * 8 + ((int) line[0] -97);
        while (moves_generator(gs, &new_gs, msks, &index, &piece_incr, &move_incr, &pieces, &move_squares)){

            if (index == 1 && move_square == (move_incr - 1 && COLUMNCONDITION //add condition to check this
                                                    //isn't a pawn attack instead
                                                    )
                && pin_mask_safe(&pin_mask, piece_incr) )
                return new_gs
        }
    }
    
    //try to match with pawn captures
    // exe4
    result = regexec(&pcapturess, line, 0, NULL, 0);
    if (!result){
        int move_square = ((int) (line[3] - 48)) * 8 + ((int) (line[2]-97)); 
        int col = ((int) (line[0]-97));
        
        while (gs= pawn_generator(gs, &piece_incr, &move_incr, &pieces, &move_squares, msks)){
             if (index == 1 && move_square == (move_incr - 1) && (piece_incr -1) % 8 == col &&
                pin_mask_safe(&pin_mask, piece_incr) )
                return new_gs
        }
    }
    
    //e.g e3xe4
    result = regexec(&pcapturesc, line, 0, NULL, 0);
    if (!result){
        int move_square= ((int) (line[3] - 48)) * 8 + ((int) (line[2]-97)); 
       int position =((int) (line[0] - 48)) * 8 + ((int) (line[1]-97)); 
        
        while (moves_generator(gs, &piece_incr, &move_incr, &pieces, &move_squares, msks)){
             if (index == 1 && move_square == (move_incr - 1) && (piece_incr -1) == position &&
                pin_mask_safe(&pin_mask, piece_incr) )
                return new_gs
        }
    }
    //e.g Ne4
    result = regexec(&pcmovess, line, 0, NULL, 0);
    if (!result){
        int move_square= ((int) (line[2] - 48)) * 8 + ((int) (line[1]-97)); 
        char piece = line[0]; 
        int check_index = set_index(piece);
        index = check_index;

        while (gs= pawn_generator(gs, &piece_incr, &move_incr, &pieces, &move_squares, msks)){
             if (index == check_index && move_square == (move_incr - 1) && 
                pin_mask_safe(&pin_mask, piece_incr) )
                return new_gs
        }
    }
    // e.g Nxe4
    result = regexec(&pccapturess, line, 0, NULL, 0);
    if (!result){
        int move_square= ((int) (line[3] - 48)) * 8 + ((int) (line[2]-97)); 
        char piece = line[0]; 
        int check_index = set_index(piece);
        index = check_index;

        while (gs= pawn_generator(gs, &piece_incr, &move_incr, &pieces, &move_squares, msks)){
             if (index == check_index && move_square == (move_incr - 1) && 
                        pin_mask_safe(&pin_mask, piece_incr) )
                return new_gs
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

        while (gs= pawn_generator(gs, &piece_incr, &move_incr, &pieces, &move_squares, msks)){
             if (index == check_index && move_square == (move_incr - 1) && 
                        pin_mask_safe(&pin_mask, piece_incr)
                        && ((piece_incr -1) / 8) == rank )
                return new_gs
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

        while (gs= pawn_generator(gs, &piece_incr, &move_incr, &pieces, &move_squares, msks)){
             if (index == check_index && move_square == (move_incr - 1) && 
                        pin_mask_safe(&pin_mask, piece_incr)
                        && ((piece_incr -1) % 8) == col )
                return new_gs
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

        while (gs= pawn_generator(gs, &piece_incr, &move_incr, &pieces, &move_squares, msks)){
             if (index == check_index && move_square == (move_incr - 1) && 
                        pin_mask_safe(&pin_mask, piece_incr)
                        && ((piece_incr -1) % 8) == col )
                return new_gs
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

        while (gs= pawn_generator(gs, &piece_incr, &move_incr, &pieces, &move_squares, msks)){
             if (index == check_index && move_square == (move_incr - 1) && 
                        pin_mask_safe(&pin_mask, piece_incr)
                        && ((piece_incr -1) / 8) == rank )
                return new_gs
        }
    }

    // Promotion
    //e.g e4=Q
    // this should really be handled in the game state logic not here . . . .
    result = regexec(&promo, line, 0, NULL, 0);
    if (!result){
        int move_square = ((int) (line[1]-48)) * 8 + ((int) line[0] -97);
        while (moves_generator(gs, &new_gs, msks, &index, &piece_incr, &move_incr, &pieces, &move_squares)){

            if (index == 1 && move_square == (move_incr - 1)
                && pin_mask_safe(pin_mask, piece_incr) ){
                // this should be handled in the game state logic, not here
                int color = gs->color;
                new_gs.pawns[color] &= ~(1LL << (move_incr -1));
                if (line[4] == 'Q')
                    new_gs.queens[color] |= (1LL << (move_incr -1)); 
                if (line[4] == 'R')
                    new_gs.rooks[color] |= (1LL << (move_incr -1)); 
                if (line[4] == 'K')
                    new_gs.knights[color] |= (1LL << (move_incr -1));
                if (line[4] == 'B')
                    new_gs.bishops[color] |= (1LL << (move_incr -1));  
                return new_gs

                }
        }
    }

    //add castling here


    //add enpassants here...
    

    return NULL;

}


