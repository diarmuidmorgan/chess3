//#include <everything.h>
#include "gamestate_generator.c"
#include <time.h>

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
int game_state_equals(GS * gs1, GS * gs2) {
	
	
	if (  gs1->all_pieces != gs2->all_pieces ) return 0;

	int total; for (int color = 0; color<2; color++){ 
		
		total += gs1->pawns[color] == gs2->pawns[color];
		total += gs1->rooks[color] == gs2->rooks[color];
		total += gs2->knights[color] == gs2->knights[color];
		total += gs2->bishops[color] == gs2->bishops[color];
		total += gs2->queens[color] == gs2->queens[color];
		total += gs1->kings[color] == gs2->kings[color];
		
		total += gs1->enpassants[color] == gs2->enpassants[color];
		total += gs1->castle_king_side[color] == gs2->castle_king_side[color];
		total += gs1->castle_queen_side[color] == gs2->castle_queen_side[color];
	}
	total += gs1->color == gs2->color;
	




	return total == 19;
}
//implementing the wikipedia code above.
void zobrist_values (int * return_arr){
    srand(time(NULL));
    
    // for every piece value at every square, we create a random integer value.
    for (int i=0; i<64; i++){

        for (int j=0; j<12; j++){

            return_arr[i*12 + j] = rand(); 
        }

    }
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
int zob_hash(char * board, int size_of_table, int * zobrist_vals, int * zobrist_dict){

    int h = 0;
    for (int i = 0; i<64; i++){
        //printf("%d\n", i);
        if (board[i] != '_'){
            
            int p = zobrist_dict[ (int) board[i] ];
            h ^= zobrist_vals[i * 12 + p ];
        }



    }
    return h % size_of_table;

}



typedef struct {
    
    int valid;
    int value;
    GS gs;

} table_entry;


table_entry * make_hash_table(int * size_of_table){
    
    table_entry * transposition_table = NULL;
    //see how much memory we can get
    while(! transposition_table ){
        *size_of_table /=2;
        transposition_table = malloc(sizeof(table_entry) * *size_of_table );
    }
    table_entry t;
    t.valid = 0;
    for (int i = 0; i< *size_of_table; i++){
        transposition_table[i] = t;
    }
    return transposition_table;

}


int add_to_table (table_entry * table, int size_of_table, GS * gs, int value,
                        int * zob_vals, int * zob_dict){

    int hashcode = zob_hash(gs->board_rep, size_of_table, zob_vals, zob_dict);
    int begin = hashcode -1;
    while (table[hashcode].valid && hashcode != begin){
        hashcode = (hashcode + 1) % size_of_table;
    }
    if (hashcode != begin){
        
        table_entry t;
        t.gs = *gs;
        t.valid = 1;
        t.value = value;
        table[hashcode] = t;
        return 1;
    //table is full
    return 0;

}
}
//actually the null values probably won't work? Maybe we need an array of invalid/bogus table entries?

// a 'value' pointer gets passed in, and is updated if we find a gamestate.
//function returns 1/0 depending on whether the entry is found or not.

int find_in_table(GS * gs, table_entry * table, int * value,
                 int size_of_table, int * zob_vals, int * zob_dict){

    char s[1000];
    //printf("\nhasing\n");
    //scanf("%s", &s);
    int hashcode = zob_hash(gs->board_rep,size_of_table, zob_vals, zob_dict);
    //printf("\nhashed\n");
    //scanf("%s", &s);
    //i guess the worst case here is the table is completely full and we have to 
    // iterate the real thing while finding nothing?
    int begin = hashcode-1;

    while(table[hashcode].valid && hashcode != begin){
        table_entry t = table[hashcode];
        //will comparison op work here?
        if (game_state_equals(&t.gs, gs)){
            *value = t.value;
            return 1;
        }
        hashcode = (hashcode + 1) % size_of_table;
    }

    return 0;

}





