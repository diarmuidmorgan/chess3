#include "fullparser.c"
#define opening_depth 10

/* Do we just save the hashes, or the actual game states? Maybe the hashes would be better.



*/

int play_write_game_string (char * game_str, parser_cases * pc, Zob * z, int value, FILE * fp){
    GS gs = hashed_initial_game_state(z);
    GS new_gs = gs;
    uint64_t * msks = build_mask_object();
    CS_mask * cs_msk = build_castle_masks();
    char s1[1000];
    int i = 0;
    char * m;
    //printf("READY TO PARSE A GAME\n");
    
    while (game_str[i]){
       // print_game_state(&gs);
        //scanf("%s",&s1);
        //printf("%" PRIu64 "\n", gs.hash);
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
        gs = new_gs;
        uint64_t hashcode = gs.hash;
        fprintf(fp, "%" PRIu64 "\n", hashcode);
        fprintf(fp, "%d\n", value);
        free(m);
    }
    
    return 1;
}



int main () {

   
    //grab_game_string(s);
    //play_game_string(s);

    char * s = malloc(4000 * sizeof(char));
    char * v = malloc(300 * sizeof(char));
    char filepath[] = "../data/cfriendly2";
    char outfilepath[] = "../data/openings";
    Zob * z = zob_from_file("../data/zobrist");
    FILE *fp;
    FILE * op;
    op = fopen(outfilepath, "a");
	parser_cases * pc = build_regex();
	fp =fopen(filepath,"r");
    int games_played = 0;
    int count = 1;
    int value=0;
    printf("STARTING\n");
	while(fgets(s,3000, fp) != NULL){
		count++;
        printf("GAME :::: %d\n", count++);
        if(fgets(v,1000, fp)==NULL)
            break;

        if(v[0] == '-')
            value = -1000;
        else if (v[0] == 0)
            value = 0;
        else
            value = 1000;
        
        
	if (count > 0)
        if(!play_write_game_string(s, pc, z, value,op)){
            printf("BROKE ON GAME : %d\n", count);
            return 0;
        }
        free(s);
        s=malloc(2000 * sizeof(char));
        free(v);
        v = malloc(300 * sizeof(char));
        games_played ++;
       
    }
    
    printf("PLAYED %d  GAMES\n", count);
    
    //fclose(fp);
    return 1;




}
