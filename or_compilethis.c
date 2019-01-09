#include "c/propersearch.c"
#include <unistd.h>

int turn (GS * gs, GS * new, uint64_t * msks, CS_mask * cs_msk,
         parser_cases * pc, Zob * z, table_entry * table, table_entry * opening_book, int size_of_table,
                        int opening_book_size, int * move_number) {

    *new = *gs;
    *move_number = *move_number + 1;
    printf("THINKING\n");
    *new = iterative_deepen(*gs, 7, msks, cs_msk, z, table, opening_book, size_of_table,opening_book_size, *move_number);
    sleep(1);
    print_game_state(new);

    
    *gs = *new;
    
    *move_number = *move_number +1;
    printf("THINKING..\n");
    *new = iterative_deepen(*gs, 4, msks, cs_msk, z, table, opening_book, size_of_table,opening_book_size, *move_number);
    sleep(1);

    print_game_state(new);

    *gs = *new;
}


int main () {

    
    int size_of_table = 100000000;
    int opening_book_size = 1000000;
    printf("Allocating hash table and building opening book...\n");
     Zob * z = zob_from_file("data/zobrist");
    table_entry * opening_book = make_opening_book(&opening_book_size, z, "data/openings");
    table_entry * t = make_hash_table(&size_of_table);
    printf("Initializing...\n");
   
    //table_entry * t = make_hash_table(&size_of_table);
    uint64_t * msks = build_mask_object();
    CS_mask * cs_msk = build_castle_masks();
    GS gs = hashed_initial_game_state(z);
    print_game_state(&gs);
    parser_cases * pc = build_regex();
    GS new = gs;
    int move_number = 0;
    printf("Ready...\n");
    while(1)
        turn(&gs, &new, msks, cs_msk,pc, z, t, opening_book, size_of_table, opening_book_size, &move_number);


}