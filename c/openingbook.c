#include "fullparser.c"
#define table_size 100000000



int parse_string_to_table (char * game_str, parser_cases * pc, Zob * z, 
                            int value, table_entry * table, int * collisions, int * size_of_table){
    GS gs = hashed_initial_game_state(z);
    GS new_gs = gs;
    uint64_t * msks = build_mask_object();
    CS_mask * cs_msk = build_castle_masks();
    char s1[1000];
    int i = 0;
    char * m;
    //printf("READY TO PARSE A GAME\n");
    while (game_str[i]){
        //print_game_state(&gs);
        //scanf("%s",&s1);
        new_gs = gs;
        new_gs.enpassants[0] = 0LL;
        new_gs.enpassants[1] = 0LL;
        int j = 0;
        m = malloc(7 * sizeof(char));
        while(game_str[i] && game_str[i] != ' ' && game_str[i] != '\n'){
            m[j] = game_str[i];
            i++;
            j++;
        }
        m[j] = '_';
        m[j+1] = '\0';
        //printf("%d\n", j);
        //printf("%s\n", m);
        if (!real_parse_move(m, &gs, &new_gs, msks, cs_msk, pc, z)){
            
        
            printf("NO MATCH\n");
            free(m);
            return 0;
        }
        
        i++;
        add_to_table(table, &size_of_table, &gs, value, z, collisions);
        gs = new_gs;
        free(m);
    }
    
    return 1;
}

table_entry * make_opening_book (int * size_of_table) {

    char * filename = "../data/openings";
    
    table_entry * table = make_hash_table(size_of_table);
    Zob * z = zob_from_file("../data/zobrist");
    FILE * FP = fopen(filename, "r");
    int reading = 1;
    int value;
    uint64_t hashcode;
    int count = 1;
    while(reading){
        count ++;
        if(count % 10000 == 0)
            
        if (fscanf(FP, "%" PRIu64 "\n", &hashcode) != EOF && fscanf(FP, "%d\n", &value) != EOF){
            value = value;
            add_to_table_hash(table,*size_of_table,hashcode,value);

        }
        else{
            reading = 0;
        }


    }

    return table;
}
/*
int main () {
    int size_of_table = table_size;
    table_entry * t = make_opening_book(&size_of_table);
    return 0;


    
}

*/
