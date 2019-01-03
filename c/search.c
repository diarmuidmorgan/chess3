#include "transpose_table.c"
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

int max(int a, int b){

    return (a > b) ? a : b;

}
int min (int a, int b){

    return (a<b) ? a : b;

}




/* alpha beta done as per wikipedia. Reckons you should get to the same depth while only looking at
    a square root of the total positions. But I doubt that will work here to be honest. 
    Also we actually have to evaluate the nodes now, rather than just generate them, so that will
    likely cause a considerable slow down :(

*/
int search (GS *gs, int depth, int alpha, 
            int beta, int maximizingPlayer, 
            uint64_t * msks, CS_mask * cs_msk, 
            int * position_evals, uint64_t * zob_vals,
             int * zob_dict, table_entry * transpose_table,
            int size_of_table, int * collisions, int * lookup_success) {

   // print_game_state(gs);
   // printf(" COLOR %d\n", gs->color);
    //char s[1000];
    //scanf("%s",&s);
    //we'll interpret terminal node as the king has been captured.
    //don't actually evaluate for checkmate, at least not at this stage.
    if (depth == 0 || gs->kings[gs->color] == 0LL){
        *position_evals = *position_evals + 1;
        //printf("%d\n", gs->score);
	return gs->score;
    }
    //set all of the values that are manipulated by the move generator.
   //char s[1000];
  //print_game_state(gs);
 //scanf("%s", &s); 
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
    //printf("\nGOT THIS FAR\n");
    // char s[1000];
    // scanf("%s", &s);
    //look up this piece.
    if ( find_in_table(gs, transpose_table, &value, size_of_table, zob_vals, zob_dict) ){
       	//:wq
	//printf("FOUND IN TABLE\n");	
        *lookup_success = *lookup_success + 1;
        return value;
    }
    //printf("\nGOT FURTHER\n"); 
    // scanf("%s", &s);
     
     if (maximizingPlayer){
           value = -100000;
        //c doesn't implement the max and min functions?
        while (moves_generator(gs, &new_gs, msks, &index, &piece_incr, &move_incr, &pieces, &move_squares, &attack_squares, cs_msk)){

          
        search_return = search(&new_gs, depth-1, alpha, beta, 0, msks, cs_msk,
                                 position_evals, zob_vals, zob_dict, transpose_table, size_of_table, collisions, lookup_success);
        value = max(value, search_return);
        alpha = max(alpha, value);

        if (alpha >= beta)
            break;

        new_gs = *gs;
        } 
     

        add_to_table(transpose_table,size_of_table, gs, value, zob_vals, zob_dict, collisions);
        return value;

    }
    else {

        value = 100000;
        while (moves_generator(gs, &new_gs, msks, &index, &piece_incr, &move_incr, 
                        &pieces, &move_squares, &attack_squares, cs_msk )){

          
        search_return = search(&new_gs, depth-1, alpha, beta, 1, msks, cs_msk, position_evals,
                                zob_vals, zob_dict, transpose_table, size_of_table, collisions, lookup_success);
        value = min(value, search_return);
        beta = min(beta, value);
        if (alpha >= beta)
            break;

        
        new_gs=*gs;
        }  
        add_to_table(transpose_table,size_of_table, gs, value, zob_vals, zob_dict, collisions);
        return value;




    }



}

GS find_best_move (GS gs, int depth, uint64_t * msks,
                 CS_mask * cs_msk, int * position_evals,
                 uint64_t * zob_vals, int * zob_dict, 
                 table_entry * transpose_table, int size_of_table, 
                 int * collisions, int * lookup_success ){
  //  print_game_state(&gs);
  //  char s[1000];
   // scanf("%s",&s);
    int alpha = -100000;
    int beta = 100000;
    //*position_evals = *position_evals + 1;
    uint64_t pieces = 0LL;
    int index = 0;
    int piece_incr = 0;
    int move_incr = 0;
    int value;
    int search_return;
    uint64_t move_squares = 0LL;
    uint64_t attack_squares = 0LL;
    GS new_gs = gs;
    GS best_gs;
    int best = -10000;
     while (moves_generator(&gs, &new_gs, msks, &index, &piece_incr, &move_incr, &pieces, &move_squares, &attack_squares, cs_msk)){

          
        search_return = search(&new_gs, depth-1, alpha, beta, 1, msks, 
                        cs_msk, position_evals, zob_vals, zob_dict, 
                        transpose_table, size_of_table, collisions, lookup_success);
        if (search_return > best){

            best = search_return;
            best_gs = new_gs;
        }
        new_gs = gs;
     }


    return best_gs;

}
int main () {
    uint64_t * msks = build_mask_object();
    CS_mask * cs_msk = build_castle_masks();
    printf("\nbuild base table\n");
    int size_of_table = 100000000;
    table_entry * transpose_table = make_hash_table( &size_of_table);
    int * zob_dict = malloc(128 * sizeof(int));
    int collisions = 0;
    printf("\ntable memory allocated\n");
    
    make_zobrist_dict(zob_dict);
    uint64_t * zob_vals = malloc((64*12 + 1) * sizeof(uint64_t));
    printf("\nMADE ZOBRIST DICT\n");
    zobrist_values(zob_vals);
    printf("table built\n");
    int position_evals = 0;
    char s[1000];
    int lookup_success = 0;
    
    GS gs = init_game_state();
    
    printf("\ninitializing game state");
    printf("\nlooking for best move...");
    gs = find_best_move(gs, 7, msks, cs_msk, &position_evals, 
                    zob_vals, zob_dict, transpose_table, size_of_table, 
                    &collisions, &lookup_success);
    print_game_state(&gs);

    printf("\n REACHED %d TERMINAL NODES\n", position_evals);
    printf("\n SEMI COLLISIONS : %d", collisions);
    printf("successful lookups: %d \n", lookup_success);
}
