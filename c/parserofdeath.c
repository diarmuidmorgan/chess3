#include "parsercases.c"


//just missing enpassants
int real_parse_move(char * line, GS * gs, GS * new_gs, 
                    uint64_t * msks, CS_mask * cs_msk, parser_cases * pc){

    int result = regexec(&pc->pforwards, line, 0, NULL, 0);
    if (!result)
        return simple_pawn_move(line, gs, new_gs, msks, cs_msk);
    result = regexec(&pc->pcapturess, line, 0, NULL, 0);
    if (!result)
        return simple_pawn_capture(line, gs, new_gs, msks, cs_msk);
    result = regexec(&pc->pcapturesc, line, 0, NULL, 0);
    if (!result)
        return complex_pawn_capture(line, gs, new_gs, msks, cs_msk);
    result = regexec(&pc->pcmovess, line, 0, NULL, 0);
    if (!result)
        return simple_piece_move(line, gs, new_gs, msks, cs_msk);
    result = regexec(&pc->pccapturess, line, 0, NULL, 0);
    if (!result)
        return simple_piece_capture(line, gs, new_gs, msks, cs_msk);
    result = regexec(&pc->ampccapturerw, line, 0, NULL, 0);
    if (!result)
        return ambiguous_piece_capture_rank(line, gs, new_gs, msks, cs_msk);
     result = regexec(&pc->ampccapturecl, line, 0, NULL, 0);
     if (!result)
        return ambiguous_piece_capture_col(line, gs, new_gs, msks, cs_msk);
    result = regexec(&pc->ampcemovecl, line, 0, NULL, 0);
    if (!result)
        return ambiguous_piece_move_col(line, gs, new_gs, msks, cs_msk);
    result = regexec(&pc->ampcemoverw, line, 0, NULL, 0);
    if (!result)
        return ambiguous_piece_move_rank(line, gs, new_gs, msks, cs_msk);
    result = regexec(&pc->promom, line, 0, NULL, 0);
    if (!result)
        return promotion_move(line, gs, new_gs, msks, cs_msk);
    result = regexec(&pc->promoc, line, 0, NULL, 0);
    if (!result)
        return promotion_capture(line, gs, new_gs, msks, cs_msk);
    result = regexec(&pc->kscastle, line, 0, NULL, 0);
     if (!result)
        return kscastle_parse(line, gs, new_gs, msks, cs_msk);
    result = regexec(&pc->qscastle, line, 0, NULL, 0);  
     if (!result)
        return qscastle_parse(line, gs, new_gs, msks, cs_msk);      

    return 0;
                    }

int play_game_string (char * game_str, parser_cases * pc){
    GS gs = init_game_state();
    GS new_gs = gs;
    uint64_t * msks = build_mask_object();
    CS_mask * cs_msk = build_castle_masks();
    char s1[1000];
    int i = 0;
    char * m;
    while (game_str[i]){
        print_game_state(&gs);
        //scanf("%s",&s1);
        new_gs = gs;
        int j = 0;
        m = malloc(7 * sizeof(char));
        while(game_str[i] && game_str[i] != ' ' && game_str[i] != '\n'){
            m[j] = game_str[i];
            i++;
            j++;
        }
        m[j] = '_';
        m[j+1] = '\0';
        printf("%d\n", j);
        printf("%s\n", m);
        if (real_parse_move(m, &gs, &new_gs, msks, cs_msk, pc))
            printf("MATCH\n");
        else {
            printf("NO MATCH\n");
            free(m);
            return 0;

        }
        i++;
        gs = new_gs;
        free(m);
    }
    return 1;
}


int main () {

   
    //grab_game_string(s);
    //play_game_string(s);

    char * s = malloc(2000 * sizeof(char));
    char filepath[] = "../data/cfriendly";
    FILE *fp;
	parser_cases * pc = build_regex();
	fp =fopen(filepath,"r");
    int games_played = 0;
	while(fgets(s,1000, fp)){

        if(!play_game_string(s, pc))
            return 0;
            
        free(s);
        s=malloc(2000 * sizeof(char));
        games_played ++;
        fgets(s,1000, fp);
        free(s);
        s=malloc(2000 * sizeof(char));
    }
    
    printf("%d\n", games_played);
    
    fclose(fp);
    return 1;




}