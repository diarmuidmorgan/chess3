#include "everything.c"
#include <regex.h>

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
GS * parse_line(char * line, GS * gs) {

    if (string_length(line) == 2){





    }

    if (string_length(line) == 3){



    }


    if (string_length(line)== 4){





    }



}