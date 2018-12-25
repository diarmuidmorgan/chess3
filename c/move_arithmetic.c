#include "game_state.c"
#include <stdio.h>
#include <stdlib.h>
#include "move_masks.c"
#include <stdint.h>
#include <strings.h>
#include <math.h>
/*This isn't good.
 * Really want to treat the move generation functions, as something like generators.
 * E.g knights_moves should consume a copy of the 'knight squares' each time it is called, only yielding one new game_state
 * at a time. This would be preferable, i think, to returning some kind of list with every possible configuration in it.
 *
 *
 *
 */

/*Returns a single game state to be played to greater depth.
 *@params Gs - gs, the current game state, uint64_t * msks - the move msks, uint64_t * knights, a destructible copy of the knights
 * uint64_t - a destructible copy of possible squares so far - can be null on first try
 */
uint64_t diagonal_prototype(uint64_t * msks, uint64_t index) {
	
	// how to handle friendly and unfriendly pieces
	// and the diagonal with the friendly and unfriendly masks
	// if 0LL, then return the whole diagonal
	// and the two seperately, split on which is the smallest?
	// index = ffsll( msk & opposing)
	// index2 = ffsll (msk & friendly)
	// // return (msk ^ msk[index]) ^ (msk[index2] & (1LL << index2));
	// this will only work downwards. Need a reverse bit search for the other direction
		
	
}

GS * pawn_attack_moves(GS * gs, uint64_t * msks, uint64_t * pawns, uint64_t * move_squares, uint64_t * piece_incr, uint64_t * move_incr) {
	int color = gs->color;
	int rcolor = ~color;
	int PAWNS;
	uint64_t index_p;
	uint64_t index_m;
	if (&move_squares == 0LL){
		if &pawns == 0LL)
		return NULL; 
	else {
		//shift the knights
		index_p = ffsll(pawns);
		&piece_incr += index_p;
		//anding with pieces[rcolor] is the only significant difference with below method. Maybe we can recycle?
		//also that we have to set pawns in the 
		&move_squares = msks[&piece_incr + PAWNS] & gs->pieces[rcolor];
		&pawns = &pawns >> index_p;
		&move_incr = 0;
	
		if (&move_squares == 0LL)
		return NULL;
		}
	index = ffsll(&move_squares);
	&move_squares = &move_squares >> index;
	&move_incr += index;
	// Create masks for new and old positions.
	uint64_t new_pos = 1LL << &move_incr -1;
	uint64_t old_pos = 1LL <<&piece_incr -1;
	// Create the new game state
	GS * new_gs = copy_game_state(gs);
	// Correct the bits in the game state. 
      	new_gs->pieces[color] = (new_gs->pieces[color] ^ old_pos) | new_pos;
	new_gs->pawns[color] = new_gs->pawns[color] & new_gs->pieces[color];
	new_gs->pieces[rcolor] = new_gs->pieces[rcolor] & (~new_pos);
	//below method is kind of excessive if no pieces have been taken
	///but maybe its faster than checking
	game_state_change_color(gs,rcolor);	
	return new_gs;



}	
GS * knight_move(GS * gs, uint64_t * msks, uint64_t * knights, uint64_t * move_squares, uint64_t * piece_incr, uint64_t * move_incr) {
	int color = gs->color;
	int rcolor = ~color;
	uint64_t index_p;
	uint64_t index_m;
	//run out of both pieces and potential moves
	if (&move_squares == 0LL){
		if &knights == 0LL)
		return NULL; 
		else {
		//shift the knights
		index_p = ffsll(knights);
		&piece_incr += index_p;
		&move_squares = msks[&piece_incr + 4] & ~gs->pieces[color];
		&knights = &knights >> index_p;
		&move_incr = 0;
		}
	

}
GS * king_move(GS * gs, uint64_t * msks, uint64_t * kings, 
		uint64_t * move_squares, uint64_t * piece_incr, uint64_t * move_incr) {
	int KING_INDEX;
	//get colors
	int color = new_gs->color;
	int rcolor = ~color; 
	
	//if the generator is being initialized
	if (&move_squares == 0LL){
		//find the king and set increment. Retrieve the right mask.
		uint64_t index = ffsll(&kings);
		&piece_incr = index;
		&move_squares = msk[index + KING_INDEX] & (~ gs->pieces[color]);
		if (&move_squares == 0LL) 
			return NULL;
	}
		
	index = ffsll(&move_squares);
	&move_squares = &move_squares >> index;
	&move_incr += index;
	// Create masks for new and old positions.
	uint64_t new_pos = 1LL << &move_incr -1;
	uint64_t old_pos = 1LL <<&piece_incr -1;
	// Create the new game state
	GS * new_gs = copy_game_state(gs);
	// Correct the bits in the game state. 
      	new_gs->pieces[color] = (new_gs->pieces[color] ^ old_pos) | new_pos;
	new_gs->kings[color] = new_gs->kings[color] & new_gs->pieces[color];
	new_gs->pieces[rcolor] = new_gs->pieces[rcolor] & (~new_pos);
	//below method is kind of excessive if no pieces have been taken
	///but maybe its faster than checking
	game_state_change_color(gs,rcolor);	
	return new_gs;

}
