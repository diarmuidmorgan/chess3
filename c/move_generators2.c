#include "move_generators.c"

/*
Want to rewrite everything with static variables. And have gamestate in stack memory rather than
On the heap.

Actually - using statics is going to be a terrible idea. Pass in a base set of pointers instead,
I reckon

*/

int moves_generator(GS * gs, GS * new_gs, uint64_t * msks, int * index, 
                int * piece_incr, int * move_incr, uint64_t * pieces, uint64_t * move_squares ) {

    switch (*index){

        case(0) :
            *pieces = gs->pawns[gs->color];
            *index = *index + 1;
            

        case(1):
            
            if (pawn_generator_has_next(gs, piece_incr, move_incr, pieces, move_squares, msks)){
                pawn_generator_next(gs, new_gs,piece_incr, move_incr, pieces,move_squares, msks);
                return 1;
                break;
            }
            else {
                *pieces = gs->rooks[gs->color]; 
                *piece_incr = 0;
                *move_incr = 0;
                *index = *index+1;

            }
        case(2):
       
            if (rook_generator_has_next(gs, piece_incr, move_incr, pieces, move_squares, msks)){
                rook_generator_next(gs, new_gs,piece_incr, move_incr, pieces,move_squares, msks);
                return 1;
                break;
            }
            else{
                *pieces=gs->knights[gs->color];
                *piece_incr=0;
                *move_incr=0;
                *index = *index + 1;
            }

        
        case(3):
            if (knight_generator_has_next(gs, piece_incr, move_incr, pieces, move_squares, msks)){
                 knight_generator_next(gs, new_gs,piece_incr, move_incr, pieces,move_squares, msks);
                return 1;
               break;
            }
            else{
                *pieces = gs->bishops[gs->color];
                *piece_incr=0;
                *move_incr=0;
                *index = *index + 1;
            }
        case(4):
            if (bishop_generator_has_next(gs, piece_incr, move_incr, pieces, move_squares, msks)){
                 knight_generator_next(gs, new_gs,piece_incr, move_incr, pieces,move_squares, msks);
                return 1;
                
                break;
            }
            else{
                *pieces = gs->queens[gs->color];
                *piece_incr=0;
                *move_incr=0;
                *index = *index + 1;
            }
        case(5):
            if (queen_generator_has_next(gs, piece_incr, move_incr, pieces, move_squares, msks)){
                 queen_generator_next(gs, new_gs,piece_incr, move_incr, pieces,move_squares, msks);
                return 1;
            
                break;
            }
            else {
                *pieces = gs->kings[gs->color];
                *piece_incr=0;
                *move_incr=0;
                *index = *index + 1;
            }
        case (6) :
            if (king_generator_has_next(gs, piece_incr, move_incr, pieces, move_squares, msks)){
                king_generator_next(gs, new_gs,piece_incr, move_incr, pieces,move_squares, msks);
                return 1;
                break;
            }
            else {
                *piece_incr=0;
                *move_incr=0;
                *index= *index + 1;
                return 0;
            }
    }
}


void game_loop (GS * gs, uint64_t * msks, int depth, int * position_evals, int print_state) {
    if (print_state){
        char s[1000];
         scanf("%s", &s);
         print_game_state(gs);
    }
    if (depth == 0) return;
    *position_evals = *position_evals + 1;
    uint64_t pieces = 0LL;
    int index = 0;
    int piece_incr = 0;
    int move_incr = 0;
    uint64_t move_squares = 0LL;
    GS new_gs = *gs;
    
    //move generator should change all the fields that it points to, and we only have to check, that
    //it's true.
    while (moves_generator(gs, &new_gs, msks, &index, &piece_incr, &move_incr, &pieces, &move_squares)){

        //do something with the move;

        //copy gs to new_gs again
        game_loop(&new_gs, msks, depth-1, position_evals, print_state);
        new_gs = *gs;
     }
}

int main () {

    uint64_t * msks = build_mask_object();
    GS gs = init_game_state();
    int depth = 6;
    int position_evals = 0;
    game_loop(&gs, msks, depth, &position_evals, 0 );
    printf("%d\n", position_evals);


}
