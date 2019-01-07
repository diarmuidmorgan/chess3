//#include <everything.h>
#include "game_state.c"
#include <time.h>
#define RANDDEV "/dev/urandom"
#include <assert.h>

/* I guess it would be easiest if we started with an ordered map
* E.g have some kind of hash function that reduces positions to 32bit or maybe just 16 bit integers
* E.g add up all the words in the game state, and mod by 2**32
* Would that work?
* Then just store them in a binary tree
*
* The other option is a simple hash table in a really large array somewhere in memory (millions of bytes)
* How big would we realistically want the hash table to be?
* This would be easier to implement though because we wouldn't have to worry about balancing,
* And it would be faster for lookups.
*
*
* What I gleaned from chessprogramming wiki is that we only want to store those nodes that
* cause alpha beta cut offs. 

from wikipedia Zobrist hash codes--

constant indices
       white_pawn := 1
       white_rook := 2
       # etc.
       black_king := 12
   
   function init_zobrist():
       # fill a table of random numbers/bitstrings
       table := a 2-d array of size 64×12
       for i from 1 to 64:  # loop over the board, represented as a linear array
           for j from 1 to 12:      # loop over the pieces
               table[i][j] = random_bitstring()
   
   function hash(board):
       h := 0
       for i from 1 to 64:      # loop over the board positions
           if board[i] != empty:
               j := the piece at board[i], as listed in the constant indices, above
               h := h XOR table[i][j]
       return h


*/

//from stack overflow
// I don't think we actually need the asserts.
// reading dev/rand gives bigger and better hashes I think. 
// or does it make a difference?
//seemed to work better when it was 32 bit :(



typedef struct {
    
    int valid;
    uint64_t hash;
    int value;
    // GS gs;

} table_entry;

typedef struct  {

    uint64_t * vals;
    int * dict;



} Zob;



uint64_t rndull(){

    FILE *rdp;
    uint64_t num;

    rdp = fopen(RANDDEV, "rb");
    assert(rdp);

    assert(fread(&num, sizeof(num), 1, rdp) == 1);

    fclose(rdp);

    return num;


}


//implementing the wikipedia code above.
// need an extra bit for color
void zobrist_values (uint64_t * return_arr){
    
    srand(time(NULL));
    
    // for every piece value at every square, we create a random integer value.
    for (int i=0; i<64; i++){

        for (int j=0; j<12; j++){

            return_arr[i*12 + j] =  rndull(); 
	    //printf("%" PRIu64 "\n", return_arr[i*12 + j]);
        }

    }
    //bit for color
    return_arr[64*12 + 1] = rand();
}
void make_zobrist_dict(int * return_arr){

    static char pieces[] = "pPrRhHbBkKqQ";
    for (int i=0; i<128; i++)
        return_arr[i] = 0;
    for (int i=0; i<12; i++){
        return_arr[(int) pieces[i]] = i;

    }
}
//implementing the wikipedia code above
// change to 64 bit

//should also add incremental hashing
// will have to be broken into two parts if we don't want to rewrite all of the game loop
// like a bit for updating white, and then optional for updating black
// can also look into packing bits rather than using that awful char array.
uint64_t incremental_hash (uint64_t hashcode, int new_position, 
                            int old_position, char p, uint64_t * zob_values, uint64_t * zob_dict){

    ///code goes here.
    int pc = zob_dict[p];
    hashcode |= zob_values[old_position * 12 + pc];
    hashcode |= zob_values[new_position * 12 + pc];
    //etc, too tired right now
    return 0ULL;
}
//as per the wikipedia code above
uint64_t zob_hash(char * board, int color, Zob * z){

    uint64_t h = 0;
    for (int i = 0; i<64; i++){
        //printf("%d\n", i);
        if (board[i] != '_'){
            
            int p = z->dict[ (int) board[i] ];
            h ^= z->vals[i * 12 + p ];
        }



    }
    h ^= (z->vals[768] *  color);
    //printf("%" PRIu64 "\n", h);
    return h;

}




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


int add_to_table (table_entry * table, int size_of_table, GS * gs, int value,
                        Zob * z, int * collisions){
    //printf("ADDED TO TABLE\n");
    uint64_t hashcode = gs->hash;
    int h = abs((int) (hashcode & 0xfffffffULL) ) % size_of_table;
    int begin = h -1;

    if(table[h].valid && table[h].hash != hashcode){
        printf("COLLISION\n");
    }
    table[h].hash = hashcode;
    table[h].valid = 1;
    table[h].value = value;

    return 1;

    if (table[h].valid){
        *collisions = *collisions + 1;
    }
    while (table[h].valid && h != begin){
       
        //move is already hashed.
        // now we only compare the hashcode. Figure that a real collision should
        // be rare enough that we don't have to worry about it.
        if (table[h].hash == hashcode ) // && game_state_equals(&table[h].gs, gs))
            return 1;
         h = (h + 1) % size_of_table;
    }
    if (h != begin){
        
        table_entry t;
        t.hash = hashcode;
        t.valid = 1;
        t.value = value;
        table[h] = t;
        return 1;
    //table is full
   }
   return 0;
}
//actually the null values probably won't work? Maybe we need an array of invalid/bogus table entries?

// a 'value' pointer gets passed i:wq:n, and is updated if we find a gamestate.
//function returns 1/0 depending on whether the entry is found or not.

int find_in_table(GS * gs, table_entry * table, int * value,
                 int size_of_table, Zob z){

    uint64_t hashcode = gs->hash;
    int h = ( (int) ( hashcode & 0xfffffffULL  ) ) % size_of_table;
    
    int begin = h-1;
    //we could end up reading the entire table every time, if there are enough collisions!!
    while(table[h].valid && h != begin){
     
        if (table[h].hash == hashcode  ) { 
            *value = table[h].value;
          
            return 1;
        }
          h = (h + 1) % size_of_table;
        }
      
    

    return 0;
}


Zob * make_zob_struct() {

    Zob * zob = malloc(sizeof(zob));
    uint64_t * zobvals =  malloc((64*12 + 1) * sizeof(uint64_t));
    zobrist_values(zobvals);
    int *zobdict =  malloc(128 * sizeof(int));
    make_zobrist_dict(zobdict);
    zob->vals = zobvals;
    zob->dict = zobdict;
    return zob;

}

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
	
	//'selected pieces' is a pointer to the filed in the game state (e.g pawns, rooks)
	//that needs to be updated. 
	//despite the game state itself being passed as a pointer.
	
	new_gs->pieces[color] = (new_gs->pieces[color] | new_pos) & (~old_pos);
	
	*selected_pieces = (*selected_pieces | new_pos) & (~old_pos);
	//binary_print_board(*selected_pieces);
	new_gs ->all_pieces = (new_gs->all_pieces | new_pos) & (~old_pos);

	//change the game state color;
	new_gs->color = r_color;
    //printf("%c\n", p_index);
    int p = z->dict[ (int) p_index ];
    p=0;
    //flip for new and old positions
    //printf("%d\n", p);
    //printf("%d\n", new_index * 12 + p);
    //printf("%" PRIu64 "\n",z->vals[new_index * 12 + p ] );
    new_gs->hash ^= z->vals[new_index * 12 + p ];
    new_gs->hash ^= z->vals[old_index * 12 + p ];
    //flip for player color
    new_gs->hash &= z->vals[64 * 12];
    

	
	//handle captures.
	//if this update changes the other sides piece's, we need to update every piece array,
	// and check for a base score change.
	// I think this is kind of inefficient, so many if statements.
	// At least, we should exit on the first condition for the majority of moves.
	// And hopefully exit on the pawns condition for most of the rest, right?

	//TBH it might not even be worth it keeping a running score count, 
	//easier to just check for it on the terminal nodes?
	
	// though it MIGHT be good if we implement move_ordering and quiesence search.

	if (((new_gs->pieces[r_color] & (~new_pos)) ) != new_gs->pieces[r_color]){
	//	printf("\nUPDATING\n");
		int multiplier;
		if (color == 1) multiplier = 1;
		else multiplier = -1;
		//binary_print_board(new_pos);
		new_gs ->pieces[r_color] = new_gs->pieces[r_color] & (~new_pos);
		//binary_print_board(new_gs->pieces[r_color]);
		//new_gs->all_pieces |= new_gs->pieces[r_color];
		//binary_print_board(new_gs->rooks[r_color]);
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
			//binary_print_board(new_gs->bishops[r_color]);
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
			//binary_print_board(new_pieces);
			//binary_print_board(new_gs->rooks[r_color]);
            if (new_index + (56*r_color) == 0)
                new_gs->castle_queen_side[r_color]=0;
            if (new_index + (56*r_color) == 7)
                new_gs->castle_king_side[r_color]=0;
            
            p_index = (r_color) ? 'R' : 'R';
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
       
        //here we catch and penalize illegal castling moves
        new_pieces = new_gs->ghost_squares[r_color] & new_gs->pieces[r_color];
        if (new_pieces != new_gs->kings[r_color]){
            new_gs->score += 100000 * multiplier;
            new_gs->kings[r_color] = 0LL;
            return;

        }
	}
}






