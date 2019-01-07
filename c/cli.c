#include "search.c"

int turn (GS * gs, GS * new, uint64_t * msks, CS_mask * cs_msk,
         parser_cases * pc, Zob * z, table_entry * table, int size_of_table) {

    *new = *gs;
    char s[1000];
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
    printf("THINKING...\n");
    *new = find_best_move(*new, 10, msks, cs_msk,  table, size_of_table,z);
   
    *gs = *new;

}


int main () {

    Zob * z = make_zob_struct();
      int size_of_table = 1000000;
    table_entry * t = make_hash_table(&size_of_table);
    uint64_t * msks = build_mask_object();
    CS_mask * cs_msk = build_castle_masks();
    GS gs = init_game_state();
    parser_cases * pc = build_regex();
    GS new = gs;
  
    while(1)
        turn(&gs, &new, msks, cs_msk,pc, z, t, size_of_table);


}