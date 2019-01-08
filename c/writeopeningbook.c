#include "fullparser.c"
#define opening_depth 10

/* Do we just save the hashes, or the actual game states? Maybe the hashes would be better.


// to be honest, I think this is a really dumb way to do the opening book


*/



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
