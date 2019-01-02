#include <everything>
#include <math.h>

/* FROM THE WIKIPEDIA PAGE

function alphabeta(node, depth, α, β, maximizingPlayer) is
    if depth = 0 or node is a terminal node then
        return the heuristic value of node
    if maximizingPlayer then
        value := −∞
        for each child of node do
            value := max(value, alphabeta(child, depth − 1, α, β, FALSE))
            α := max(α, value)
            if α ≥ β then
                break (* β cut-off *)
        return value
    else
        value := +∞
        for each child of node do
            value := min(value, alphabeta(child, depth − 1, α, β, TRUE))
            β := min(β, value)
            if α ≥ β then
                break (* α cut-off *)
        return value
*/

/* alpha beta done as per wikipedia. Reckons you should get to the same depth while only looking at
    a square root of the total positions. But I doubt that will work here to be honest. 
    Also we actually have to evaluate the nodes now, rather than just generate them, so that will
    likely cause a considerable slow down :(

*/
int search (GS *gs, int depth, int alpha, 
            int beta, int maximizingPlayer, 
            uint64_t * msks, CS_Mask * cs_msk, 
                            int * position_evals) {

    //we'll interpret terminal node as the king has been captured.
    //don't actually evaluate for checkmate, at least not at this stage.
    if (depth == 0 || gs->kings[gs->color] == 0LL)
        return simple_evaluate_game_state(&gs);

    //set all of the values that are manipulated by the move generator.
    *position_evals = *position_evals + 1;
    uint64_t pieces = 0LL;
    int index = 0;
    int piece_incr = 0;
    int move_incr = 0;
    int value;
    int search_return;
    uint64_t move_squares = 0LL;
    GS new_gs = *gs;
    int r_color = (gs->color + 1) % 2;
    //new_gs->enpassants[color] = 0LL;
	new_gs->enpassants[r_color] = 0LL;

     
     if (maximizingPlayer){
           value = -100000;
        //c doesn't implement the max and min functions?
        while (moves_generator(gs, &new_gs, msks, &index, &piece_incr, &move_incr, &pieces, &move_squares)){

          
        search_return = search(new_gs, depth-1, alpha, beta, 0, msks, cs_msk, position_evals);
        value = max(value, search_return);
        alpha = max(alpha, value);
        if (alpha >= beta)
            break;

        new_gs = *gs;
        } 
     

        
        return value;

    }
    else {

        value = 100000;
        while (moves_generator(gs, &new_gs, msks, &index, &piece_incr, &move_incr, &pieces, &move_squares)){

          
        search_return = search(new_gs, depth-1, alpha, beta, 0, msks, cs_msk, position_evals);
        value = min(value, search_return);
        beta = min(beta, value);
        if (alpha >= beta)
            break;

        
        new_gs=*gs;
        }  
        
        return value;




    }



}

GS find_best_move (GS gs, int depth, uint64_t * msks, CS_mask msk, int * position_evals){


    *position_evals = *position_evals + 1;
    uint64_t pieces = 0LL;
    int index = 0;
    int piece_incr = 0;
    int move_incr = 0;
    int value;
    int search_return;
    uint64_t move_squares = 0LL;
    GS new_gs = *gs;
    GS best_gs;
    int best = -10000
     while (moves_generator(gs, &new_gs, msks, &index, &piece_incr, &move_incr, &pieces, &move_squares)){

          
        search_return = search(new_gs, depth-1, alpha, beta, 1, msks, cs_msk, position_evals);
        if (search_return > best){

            best = search_return;
            best_gs = new_gs;
        }
        new_gs = *gs;
     }


    return best_gs;

}
