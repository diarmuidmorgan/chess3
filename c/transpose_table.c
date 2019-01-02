#include <everything.h>

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
*/
typedef struct {
    
    int valid;
    int value;
    GS gs;

} table_entry;
table_entry * make_hash_table(int size_of_table){

    table_entry * transposition_table = malloc(sizeof(table_entry) * size_of table );
    table_entry t;
    t.valid = 0;
    for (int i = 0; i< size_of_table; i++){
        transposition_table[i] = t;
    }
    return transposition_table;

}

// actually this won't work because the overflows and casting
// could well lead to undefined behavior????
// how else could we do it?
// I guess i gotta read up more on hash functions!
int hash_function (GS * gs, int size_of_table){

    //don't worry about overflows
    uint64_t total = 0;
    for (int i = 0; i<2; i ++ ){
        total+=gs->pawns[i];
        total+=gs->bishops[i];
        total+=gs->rooks[i];
        total+=gs->knights[i];
        total+=gs->queens[i];
        total+=gs->kings[i];


    }
    //add some big number if the player is black
    total += 10100010101 * gs->color;

    return abs( ((int) total) ) % size_of_table; 

}

int add_to_table (table_entry * table, int size_of_table, GS * gs, int value){

    int hashcode = hash_function(gs, size_of_table);
    while (table[hashcode].valid && hashcode < size_of_table){
        hashcode += 1;
    }
    if (hascode < size_of_table){
        
        table_entry t;
        t.gs = *gs;
        t.valid = 1;
        t.value = value;
        table[hashcode] = t;
        return 1;
    //table is full
    return 0;

}
//actually the null values probably won't work? Maybe we need an array of invalid/bogus table entries?

// a 'value' pointer gets passed in, and is updated if we find a gamestate.
//function returns 1/0 depending on whether the entry is found or not.

int find_in_table(GS * gs, table_entry * table, int * value, int size_of_table){

    int hashcode = hash_function(gs);
    while(table[hashcode].valid && hashcode < size_of_table){
        table_entry t = table[hashcode];
        //will comparison op work here?
        if (t->gs == *gs){
            *value = t->value;
            return 1
        }
        hashcode++;
    }

    return 0;

}
