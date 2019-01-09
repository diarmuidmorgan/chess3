#include "game_state.c"
#include <time.h>
#define RANDDEV "/dev/urandom"
#include <assert.h>

/* Table entry
@attributes
valid - this is set to 1 if this part of the table has been set
hash - the hashcode for the move
value - the value the move has bee given
iteration - what depth this move was searched to
move_number - what move of the game was this found from

moves from previous iterations can be used for move ordering, but need to be updated when
we search to greater depth.

*/
typedef struct {
    
    int valid;
    uint64_t hash;
    int value;
    int played;
    int iteration;
    int move_number;
    // GS gs;

} table_entry;

/* Values used for hasing.
*
*/
typedef struct  {

    uint64_t * vals;
    int * dict;



} Zob;

/* Read random bits from the system
  Copied from stackoverflow.
*/
uint64_t rndull(){

    FILE *rdp;
    uint64_t num;

    rdp = fopen(RANDDEV, "rb");
    assert(rdp);

    assert(fread(&num, sizeof(num), 1, rdp) == 1);

    fclose(rdp);

    return num;


}
/* Create Zobrist values
*/
void zobrist_values (uint64_t * return_arr){

    for (int i=0; i<64; i++){

        for (int j=0; j<12; j++){

            return_arr[i*12 + j] =  rndull(); 

        }

    }
    return_arr[64*12] = rndull();
}

/* Bit excessive. An array where the values can be read from.
*/
void make_zobrist_dict(int * return_arr){

    static char pieces[] = "pPrRhHbBkKqQ";
    for (int i=0; i<128; i++)
        return_arr[i] = 0;
    for (int i=0; i<12; i++){
        return_arr[(int) pieces[i]] = i;

    }
}

/* Make the zob structure
*/
Zob * make_zob_struct() {

    Zob * zob = malloc(sizeof(zob));
    uint64_t * zobvals =  malloc((64*12 + 1) * sizeof(uint64_t));
    zobrist_values(zobvals);
    int *zobdict =  malloc(128 * sizeof(int));
    make_zobrist_dict(zobdict);
    zob->vals = zobvals;
    zob->dict = zobdict;
    //free(zobvals);
    //free(zobdict);
    return zob;

}
/* Used for hasing the initial game state. Nothing more
*/
uint64_t zob_hash(char * board, int color, Zob * z){

    uint64_t h = 0;
    for (int i = 0; i<64; i++){
        
        if (board[i] != '_'){
            
            int p = z->dict[ (int) board[i] ];
            h ^= z->vals[i * 12 + p ];
        }
    }
    h ^= (z->vals[768] *  color);
    return h;

}
/* As we use the zob values for our openings as well, we want a set of them saved to file.
*/
void save_zobrist (Zob * zob, char * filename) {

   
    FILE * fp = fopen(filename, "ab");
    int i=0;
    while (i<=64*12){
        uint64_t val = zob->vals[i];
        fwrite(&val, sizeof(uint64_t),1, fp);

        i++;

    }
   
}

/* Computes initial game state hash code.
*/

uint64_t initial_hash(Zob * z){
    char s [] = "rhbqkbhrpppppppp________________________________pppppppprhbqkbhr";
	int i=0;
    char * board_rep = malloc(64 * sizeof(char));
	while (s[i]){
		board_rep[i] = s[i]; 
		i++;
	}
    return zob_hash(board_rep, 0, z);

}

/* Read the saved zobrist values from file.
*/
Zob * zob_from_file(char * filename){
    Zob * z = make_zob_struct();
    FILE * fp = fopen(filename, "rb");
    int i=0;
    uint64_t val;
    while (i <=64*12){

        fread(&val, sizeof(uint64_t), 1, fp);
        printf("%" PRIu64 "\n", val);
        z->vals[i] = val;
        i++;
    }
    fclose(fp);
    return z;
}

/* Create an empty transposition table
*/
table_entry * make_hash_table(int * size_of_table){
    
    table_entry * transposition_table = malloc(sizeof(table_entry) * (*size_of_table ) );
    //see how much memory we can get
    //*size_of_table = *size_of_table * 2;
    while(! transposition_table ){
        printf("\nShrinking table\n");
        *size_of_table /=2;
        transposition_table = malloc(sizeof(table_entry) * (*size_of_table ) );
    }
    table_entry t;
    t.valid = 0;
    for (int i = 0; i< *size_of_table; i++){
        transposition_table[i] = t;
    }
   
    return transposition_table;
}

/* Add the move hash and value to the tranposition table.
*/
int add_to_table_hash(table_entry * table, int size_of_table,
                         GS * gs, uint64_t hashcode, int value,
                        int iteration, int move_number){
    //we don't care about collisions here.
    int h = abs((int) (hashcode & 0xfffffffULL) ) % size_of_table;
    table_entry * item = &table[h];
    table_entry t;
    t.hash = hashcode;
    t.iteration=iteration;
    t.move_number=move_number;
    t.valid = 1;
    t.value = value;
    table[h] = t;
                        
}

/* Different method for opening moves. Add a point for each time we encounter this move.
*/
int add_to_table_opening(table_entry * table, int size_of_table,
                          uint64_t hashcode, int value){

    int h = abs((int) (hashcode & 0xfffffffULL) ) % size_of_table;
    table_entry * item = &table[h];
    table_entry t;
    t.hash = hashcode;
    t.iteration=-1;
    t.move_number=-1;
    t.valid = 1;
    t.value = value;
    if(table[h].valid && table[h].hash == hashcode)
        table[h].value = table[h].value + value;
    else
        table[h] = t;

}
//should return 0 for no find of move just in table
// 1 if the item is found with the same iteration and move number

int find_in_table(table_entry * table, int size_of_table,
                      uint64_t hashcode, int * value, int iteration, int move_number){

    int h = abs((int) (hashcode & 0xfffffffULL) ) % size_of_table;
    table_entry t = table[h];
    if (t.valid && t.hash == hashcode){
        *value = t.value;
        if(t.move_number = -1 || (t.iteration == iteration && t.move_number == move_number))
            return 1;
        else
            return 0;
    }
    *value=0;
    return 0;
}



GS hashed_initial_game_state(Zob * z){

    GS gs = init_game_state();
    gs.hash = initial_hash(z);
    return gs;

}


void full_game_state_update(GS * new_gs, int new_index, 
					int old_index, uint64_t * selected_pieces, Zob * z, char p_index){
					
	int color = new_gs->color;
	int r_color = (color + 1) % 2;
	uint64_t new_pos = 1LL << new_index;
    uint64_t old_pos = 1LL << old_index;
	//update the pieces for the player who just moved.
	
	new_gs->pieces[color] = (new_gs->pieces[color] | new_pos) & (~old_pos);
	
	*selected_pieces = (*selected_pieces | new_pos) & (~old_pos);
	
	new_gs ->all_pieces = (new_gs->all_pieces | new_pos) & (~old_pos);

	
	new_gs->color = r_color;
    
    int p = z->dict[ (int) p_index ];
    p=0;
    //flip for new and old positions
   
    new_gs->hash ^= z->vals[new_index * 12 + p ];
    new_gs->hash ^= z->vals[old_index * 12 + p ];
    //flip for player color
    new_gs->hash ^= z->vals[64 * 12];
    

	if (((new_gs->pieces[r_color] & (~new_pos)) ) != new_gs->pieces[r_color]){
	
		int multiplier;
		if (r_color == 1) multiplier = 1;
		else multiplier = -1;
		
		new_gs ->pieces[r_color] = new_gs->pieces[r_color] & (~new_pos);
		uint64_t new_pieces = new_gs->pawns[r_color] & new_gs->pieces[r_color];
		
		if (new_pieces != new_gs->pawns[r_color]){
			p_index = (r_color) ? 'P' : 'p';
            p = z->dict[ (int) p_index ];
            new_gs->hash ^= z->vals[new_index * 12 + p ];
			new_gs->score += 1 * multiplier;
			new_gs->pawns[r_color] = new_pieces;
			return;
		}
		new_pieces = new_gs->pieces[r_color] & new_gs->bishops[r_color];
		if (new_pieces != new_gs->bishops[r_color]){
			p_index = (r_color) ? 'B' : 'b';
            p = z->dict[ (int) p_index ];
            new_gs->hash ^= z->vals[new_index * 12 + p ];
			new_gs->score += 3 * multiplier;
			new_gs->bishops[r_color] = new_pieces;
			
			return;
		}
		new_pieces = new_gs->knights[r_color] & new_gs->pieces[r_color];
		if (new_pieces != new_gs->knights[r_color]){
			p_index = (r_color) ? 'H' : 'h';
            p = z->dict[ (int) p_index ];
            new_gs->hash ^= z->vals[new_index * 12 + p ];
            new_gs->score += 3 * multiplier;
			new_gs->knights[r_color] = new_pieces;
			return;
		}
		
		new_pieces = new_gs->rooks[r_color] & new_gs->pieces[r_color];
		if (new_pieces != new_gs->rooks[r_color]){
		
            if (new_index + (56*r_color) == 0)
                new_gs->castle_queen_side[r_color]=0;
            if (new_index + (56*r_color) == 7)
                new_gs->castle_king_side[r_color]=0;
            
            p_index = (r_color) ? 'R' : 'r';
            p = z->dict[ (int) p_index ];
            new_gs->hash ^= z->vals[new_index * 12 + p ];
			new_gs->score += 5 * multiplier;
			new_gs->rooks[r_color] = new_pieces;
			return;
		}
		new_pieces = new_gs->queens[r_color] & new_gs->pieces[r_color];
		if (new_pieces != new_gs->queens[r_color]){
            p_index = (r_color) ? 'Q' : 'q';
            p = z->dict[ (int) p_index ];
            new_gs->hash ^= z->vals[new_index * 12 + p ];
			new_gs->score += 9 * multiplier;
			new_gs->queens[r_color] = new_pieces;
			return;
		}
		new_pieces = new_gs->kings[r_color] & new_gs->pieces[r_color];
		if (new_pieces != new_gs->kings[r_color]){
            p_index = (r_color) ? 'K' : 'k';
            p = z->dict[ (int) p_index ];
            new_gs->hash ^= z->vals[new_index * 12 + p ];
			new_gs->score += 1000 * multiplier;
			new_gs->kings[r_color] = new_pieces;
			return;
		}
       
       
        new_pieces = new_gs->ghost_squares[r_color] & new_gs->pieces[r_color];
        if (new_pieces != new_gs->kings[r_color]){
            new_gs->score += 100000 * multiplier;
            new_gs->kings[r_color] = 0LL;
            return;

        }
	}
}






