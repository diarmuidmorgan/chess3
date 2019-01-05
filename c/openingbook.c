#include "fullparser.c"
#define table_size 100000000



int parse_string_to_table (char * game_str, parser_cases * pc, Zob * z, 
                            int value, table_entry * table, int * collisions, int size_of_table){
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
        add_to_table(table, size_of_table, &gs, value, z, collisions);
        gs = new_gs;
        free(m);
    }
    
    return 1;
}

table_entry * make_opening_book () {

    char * s = malloc(4000 * sizeof(char));
    char filepath [] = "../data/cfriendly2";
    printf("ALL GOOD");
    int collisions = 0;
    Zob * z = make_zob_struct();
    FILE *fp;
    fp =fopen(filepath,"r");
	parser_cases * pc = build_regex();
    int size_of_table = table_size;
    table_entry * table = make_hash_table(&size_of_table);
    
    char * v = malloc(20 * sizeof(char));
    int count = 0;
    int value;
    while(fgets(s,3000, fp) != NULL && count < 19000){
        printf("%d\n", count++);
        fgets(v, 3000, fp);
        if (v[0] == '-')
            value = -1 * 100;
        else if (v[1] == '0')
            value=0;
        else
            value = 1 * 1000;

        if (!parse_string_to_table(s, pc, z, value, table, &collisions, size_of_table))
            return NULL;
        
        free(s);
        s=malloc(2000 * sizeof(char));
        free(v);
        v=malloc(300 * sizeof(char));



    }


    return table;

}

int main () {
    table_entry * t = make_opening_book();
    return 0;


    
}


