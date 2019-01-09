#include "openingbook.c"
#define QS_SEARCH_DEPTH  3
#define MOVE_LIST_SIZE 100
int max(int a, int b){

    return (a > b) ? a : b;

}
int min (int a, int b){

    return (a<b) ? a : b;

}

typedef struct {

    int value;
    int retrieved;
    int playable;
    GS gs;
} Move;

typedef struct {

    Move list[MOVE_LIST_SIZE];
    int top;

} Move_list;

int max_move(Move_list * list, Move * new_move){
    int best = -1000000000;
    int best_index=-1;
    for (int i=0; i<list->top; i++){
        if(list->list[i].value > best && !list->list[i].retrieved){
            best = list->list[i].value;
            best_index = i;
        }

    }
    if(best_index < 0)
        return 0;
    list->list[best_index].retrieved=1;
    *new_move = list->list[best_index];
    return 1;


}

int min_move(Move_list * list, Move * new_move){
    int best = 1000000000;
    int best_index=-1;
    for (int i=0; i<list->top; i++){
        if(list->list[i].value < best && !list->list[i].retrieved){
            best = list->list[i].value;
            best_index = i;
        }

    }
    if(best_index < 0)
        return 0;
    list->list[best_index].retrieved=1;
    *new_move = list->list[best_index];
    return 1;


}



int alpha_beta (GS *gs, int depth, int alpha, 
            int beta,
            uint64_t * msks, CS_mask * cs_msk, 
         table_entry * transpose_table,
            int size_of_table, 
            Zob * z, int iteration, int move_number ) {
    
    
    //early stopping conditions
    if (depth == 0)
        return gs->score;
     //   return qs_search(gs, QS_SEARCH_DEPTH, alpha, beta, msks, 
     //                       cs_msk, transpose_table, size_of_table, z);

    else if (gs->kings[gs->color] == 0LL)
        return gs->score;



    //our iterator values.
    uint64_t pieces = 0LL;
    int index = 0;
    int piece_incr = 0;
    int move_incr = 0;
    uint64_t attack_squares = 0LL;
    uint64_t move_squares = 0LL;
    


    Move_list move_list;
    move_list.top = 0;
    
    //copy gamestate
    GS new_gs = *gs;
    int r_color = (gs->color + 1) % 2;
    
    //zero out enpassants for the next iteration
	new_gs.enpassants[r_color] = 0LL;


   
    int value=0;
    //get all moves from the tranpose table of just set their value to 0
    while (moves_generator(gs, &new_gs, msks, &index, &piece_incr, &move_incr, 
                        &pieces, &move_squares, &attack_squares, cs_msk, z)){

        int reti = find_in_table(transpose_table, size_of_table, new_gs.hash, 
                    &value, iteration, move_number);
        Move m;
        m.gs = new_gs;
        m.value = value;
        m.retrieved = 0;
        m.playable = reti;
        move_list.list[move_list.top]=m;
        new_gs = *gs;
        move_list.top = move_list.top + 1;
        //run out of slots.
        if (move_list.top == MOVE_LIST_SIZE)
            break;
        new_gs = *gs;
     }
    

    //player is maximizing.
    Move m;
    int s_return;
    if (gs->color==0){
        value = -100000000;
        
        while (max_move(&move_list,&m)){
           
            if (m.playable)
                value = max(value, m.value);
            else {
                s_return =alpha_beta(&m.gs, depth-1, alpha, beta, msks, cs_msk,
                                        transpose_table, size_of_table, z, iteration, move_number); 
                value = max(value, s_return);
            
            add_to_table_hash(transpose_table, size_of_table, m.gs.hash,s_return, iteration, move_number);
            
            }
            alpha = max(alpha, value);
            if (alpha>=beta)
                break;

        }
        
        return value;

    } 
    //player is minimizing
    else {

        value = 100000000;
        int i=0;
        while (min_move(&move_list, &m)){
          
            if (m.playable)
                value = min(value, m.value );
            else {
                s_return =alpha_beta(&m.gs, depth-1, alpha, beta, msks, cs_msk,
                                 
                                  transpose_table, size_of_table, z, iteration, move_number);
                value = min(value, s_return);
           
            add_to_table_hash(transpose_table, size_of_table, m.gs.hash,s_return, iteration, move_number);
            }
            beta = min(beta, value);

            if (alpha>=beta)
                break;

        }
        return value;
      }
}

GS root_search (GS gs, int depth, uint64_t * msks,
                 CS_mask * cs_msk,  Zob * z,
                 table_entry * transpose_table, table_entry * opening_book, int size_of_table,
                            int opening_book_size, int iteration, int move_number) {

    uint64_t pieces = 0LL;
    int index = 0;
    int piece_incr = 0;
    int move_incr = 0;
    uint64_t attack_squares = 0LL;
    uint64_t move_squares = 0LL;
    

    
    //copy gamestate
    GS new_gs = gs;
    GS best_gs;
    Move_list move_list;
    move_list.top = 0;
    int value=0;
    int sreturn;
    int found_opening_move = 0;
    //get all moves from the tranpose table of just set their value to 0
    if (move_number <= 10){
        while (moves_generator(&gs, &new_gs, msks, &index, &piece_incr, &move_incr, 
                        &pieces, &move_squares, &attack_squares, cs_msk, z)){

        int reti = find_in_table(opening_book, opening_book_size, new_gs.hash, 
                    &value, iteration, move_number);
        if(reti) 
            found_opening_move = 1;
        
        Move m;
        m.gs = new_gs;
        m.value = value;
        m.retrieved = 0;
        m.playable = reti;
        move_list.list[move_list.top]=m;
        new_gs = gs;
        move_list.top = move_list.top + 1;
        //run out of slots.
        if (move_list.top == MOVE_LIST_SIZE)
            break;
        new_gs = gs;
     }
     if (found_opening_move){
        //printf("FOUND OPENING MOVE\n");
        Move m;
        max_move(&move_list,&m);
        return m.gs;


     }

    }

    while (moves_generator(&gs, &new_gs, msks, &index, &piece_incr, &move_incr, 
                        &pieces, &move_squares, &attack_squares, cs_msk, z)){

        int reti = find_in_table(transpose_table, size_of_table, new_gs.hash, 
                    &value, iteration, move_number);
        Move m;
        m.gs = new_gs;
        m.value = value;
        m.retrieved = 0;
        m.playable = reti;
        move_list.list[move_list.top]=m;
        new_gs = gs;
        move_list.top = move_list.top + 1;
        //run out of slots.
        if (move_list.top == MOVE_LIST_SIZE)
            break;
        new_gs = gs;
     }
     Move m;
    int alpha = -100000000;
    int beta = 100000000;
    if (gs.color==0){
        value = -100000000;
        
        while (max_move(&move_list,&m)){
           
            if (m.playable)
                sreturn = m.value;
            else
                sreturn =alpha_beta(&m.gs, depth-1, alpha, beta, msks, cs_msk,
                                        transpose_table, size_of_table, z, iteration, move_number);
            
            if (sreturn > value){
                value = sreturn;
                best_gs = m.gs;
            }
            alpha = max(alpha, value);

            

        }
        
        

    } 
    //player is minimizing
    else {

        value = 100000000;
        int i=0;
        while (min_move(&move_list, &m)){
          
            if (m.playable)
                sreturn =  m.value;
            else
                sreturn = alpha_beta(&m.gs, depth-1, alpha, beta, msks, cs_msk,
                                 
                                  transpose_table, size_of_table, z, iteration, move_number);
            
            if(sreturn < value){
                best_gs = m.gs;
                value = sreturn;
            }
            
            beta = min(beta, value);

            if (alpha>=beta)
                break;

        }
       
      }



    printf("VALUE: %d\n",value);
    return best_gs;
}


GS iterative_deepen (GS gs, int max_depth, uint64_t * msks,
                 CS_mask * cs_msk,  Zob * z,
                 table_entry * transpose_table, table_entry * opening_book, int size_of_table, 
                            int opening_book_size, int move_number) {
    
    GS best_gs;
    int iteration = 0;
    while (iteration < max_depth){
        
        best_gs = root_search(gs, iteration + 1, msks, cs_msk, z, transpose_table, opening_book,
                                size_of_table, opening_book_size, iteration, move_number);
        iteration = iteration + 1;
    }
    return best_gs;
}
            
                 
                