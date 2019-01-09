#include "c/propersearch.c"
#include <unistd.h>

int turn (GS * gs, GS * new, uint64_t * msks, CS_mask * cs_msk,
         parser_cases * pc, Zob * z, table_entry * table, int size_of_table, int * move_number) {

    *new = *gs;
    char s[1000];
    *move_number = *move_number +1;
    
   int matched = 0;
    while(!matched){
        printf("ENTER A MOVE\n");
        scanf("%s", &s);
        int i;
        while(s[i]) i++;
        s[i] = '_';
        s[i+1]='\0'; 
        if (real_parse_move(s, gs, new, msks, cs_msk, pc, z))
            matched = 1;
        else
            printf("MOVE NOT RECOGNIZED. Use pgni e.g e4\n");
    }

    print_game_state(new);

    
    *gs = *new;
    
    *move_number = *move_number +1;
    printf("THINKING..\n");
    *new = iterative_deepen(*gs, 6, msks, cs_msk, z, table, size_of_table, *move_number);
    sleep(2);

    print_game_state(new);

    *gs = *new;
}


int main () {

    
    int size_of_table = 100000000;
    printf("Allocating hash table...\n");
     Zob * z = zob_from_file("data/zobrist");
    table_entry * t = make_opening_book(&size_of_table, z, "data/openings");
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
        turn(&gs, &new, msks, cs_msk,pc, z, t, size_of_table, &move_number);


}