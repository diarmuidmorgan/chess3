#include "search.c"

int turn (GS * gs, GS * new, uint64_t * msks, CS_mask * cs_msk,
         parser_cases * pc, Zob * z, table_entry * table, int size_of_table) {

    *new = *gs;
    char s[1000];
    
    printf("THINKING...\n");
    *new = find_best_move(*new, 5, msks, cs_msk,  table, size_of_table,z);
   
    *gs = *new;
print_game_state(gs);
    
    int matched = 0;
    while(!matched){
        printf("ENTER A MOVE\n");
        scanf("%s", &s);
        if (real_parse_move(s, gs, new, msks, cs_msk, pc, z))
            matched = 1;
        else
            printf("MOVE NOT RECOGNIZED\n");
    }
    print_game_state(new);

    *gs = *new;
}


int main () {

    Zob * z = zob_from_file("../data/zobrist");
    int size_of_table = 100000000;
    printf("Allocating hash table and getting opening book from file\n");
    //table_entry * t = make_opening_book(&size_of_table);
    table_entry * t = make_opening_book(size_of_table);
    uint64_t * msks = build_mask_object();
    CS_mask * cs_msk = build_castle_masks();
    GS gs = hashed_initial_game_state(z);
    parser_cases * pc = build_regex();
    GS new = gs;
  
    while(1)
        turn(&gs, &new, msks, cs_msk,pc, z, t, size_of_table);


}