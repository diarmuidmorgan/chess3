#include "../fullparser.c"




int main () {

   
    //grab_game_string(s);
    //play_game_string(s);

    char * s = malloc(4000 * sizeof(char));
    char filepath[] = "../data/cfriendly";
    Zob * z = make_zob_struct();
    FILE *fp;
	parser_cases * pc = build_regex();
	fp =fopen(filepath,"r");
    int games_played = 0;
    int count = 0;
    printf("STARTING\n");
	while(fgets(s,3000, fp) != NULL){
		count++;
        printf("GAME :::: %d\n", count++);
        
	if (count > 0)
        if(!play_game_string(s, pc, z)){
            printf("BROKE ON GAME : %d\n", count);
            return 0;
        }
        free(s);
        s=malloc(2000 * sizeof(char));
        games_played ++;
        if(fgets(s,1000, fp)!=NULL){

        free(s);
        s=malloc(2000 * sizeof(char));
        }
    }
    
    printf("PLAYED %d  GAMES\n", count);
    
    //fclose(fp);
    return 1;




}
