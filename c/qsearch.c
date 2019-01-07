#include "openingbook.c"

/*  From the chess programming wiki

int Quiesce( int alpha, int beta ) {
    int stand_pat = Evaluate();
    if( stand_pat >= beta )
        return beta;
    if( alpha < stand_pat )
        alpha = stand_pat;

    until( every_capture_has_been_examined )  {
        MakeCapture();
        score = -Quiesce( -beta, -alpha );
        TakeBackMove();

        if( score >= beta )
            return beta;
        if( score > alpha )
           alpha = score;
    }
    return alpha;
}
 






*/
int q_search (GS *gs, int alpha, 
            int beta, 
            uint64_t * msks, CS_mask * cs_msk, 
         table_entry * transpose_table,
            int size_of_table, 
            Zob * z) {


    int score = gs->score;
    if (score >= beta)
        return beta;
    if (alpha < score)
        alpha = score;

    uint64_t pieces = 0LL;
    int index = 0;
    int piece_incr = 0;
    int move_incr = 0;
    int value;
    int search_return;
    uint64_t attack_squares = 0LL;
    uint64_t move_squares = 0LL;
    GS new_gs = *gs;
    int r_color = (gs->color + 1) % 2;
    //new_gs->enpassants[color] = 0LL;
	new_gs.enpassants[r_color] = 0LL;
    int new_score;
     while (moves_generator(gs, &new_gs, msks, &index, &piece_incr, &move_incr, 
                        &pieces, &move_squares, &attack_squares, cs_msk, z)){
        if (new_gs.score != gs->score){


            new_score = -1 * q_search(&new_gs, -1 * alpha, -1 * beta, msks, cs_msk,
                                 transpose_table, size_of_table, z);
                if (new_score >= beta)
                    return new_score;
                if (new_score > alpha)
                    alpha = score;

        }

        new_gs = *gs;


            }
        return alpha;

    }