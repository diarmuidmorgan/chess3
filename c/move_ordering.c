#include "moves_legal.c"
/*
*   This is still a long way off being needed I think.
*   Idea is mainly to have something to encapsulate the seperate game states that we can
*   Add further flags to. The two so far are a 'valid' field, and then fields for the start and
    end squares. Though this doesn't work for castling. We'll have to have a different struct for this
    I guess?

    It seems memory efficient though, as we have to allocate for the game state every time, when we
    Only want to return an invalid move?

    C is a headfuck already.

    Maybe its just faster to pass pointers? Oh god.
*/


typedef struct {

    int valid;
    GS gs;
    int piece_pos;
    int move_pos;

} Move;

Move new_valid_move(GS * gs, int piece_incr, int move_incr ){

    Move m;
    m.valid = 1;
    m.gs =*gs;
    m.piece_pos = piece_incr;
    m.move_pos = move_incr;
    return m;
}

Move invalid_move(){
    Move m;
    m.valid = 0;
    return m;

}

//should use a very simple heuristic to rank the order moves will be played in.
int move_comparator(Move * m){

    return -1;
}

//should sort the moves. Using qsort I guess.
// Or should we bother, and just use simple pass through instead?
void move_sort(){


    return -1

}



