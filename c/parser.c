#include "everything.c"
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
regex_t pforwards;
reti = recomp(&pforwards, pawnforwards, 0);
regex_t pcapturess
reti = recomp(&pcaputress, pawncapturesimple, 0);



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




//maybe its time to learn regx?
GS * parse_line(char * line, GS * gs, uint64_t * msks) {

    int result;

    //try to match with a pawn forwards move.
    result = regexec(&pforwards, line, 0, NULL, 0)
    if (!result){

        int position = ((int) (line[1] - 48)) * 8 + ((int) (line[0]-97)); 
        while (gs= pawn_generator(gs, &piece_incr, &move_incr, &pices, &move_squares, msks)){
            if ( (move_incr - 1) == position ){
                return gs;
            }

        }





    }
    //try to match with pawn captures
    result = regexec(&pforwards, line, 0, NULL, 0)
    if (!result){
        int position = ((int) (line[3] - 48)) * 8 + ((int) (line[2]-97)); 
        int rank = ((int) (line[0]-97));
        int position = ((int) (line[1] - 48)) * 8 + ((int) (line[0]-97)); 
        while (gs= pawn_generator(gs, &piece_incr, &move_incr, &pices, &move_squares, msks)){
            if ( (move_incr - 1) == position && (piece_incr-1) / 8 = rank ){
                return gs;
            }

        }



        
    }


    

    return NULL;

}


