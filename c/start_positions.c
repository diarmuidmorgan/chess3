#include <stdlib.h>
#include <stdio.h>

// functions to build the starting board. 
// None of the mallocd memory every gets freed.
// Should have just loaded arrays from .txt files but I didn't know how to open a file at this point.

int * blank_bit_array() {
	
	int * b = malloc(64*sizeof(int));
	for(int i=0;i<64;i++) b[i] = 0;
	return b;	
}
int * king_castling_empty (int rank) {

	int * b = blank_bit_array();
	for (int i = 5; i<7;i++){
		
		b[(rank * 8) + i] = 1;
	}
	return b;

}
int * queen_castling_empty(int rank){
	int * b = blank_bit_array();
	for (int i = 1; i<4;i++){
		b[(rank * 8) + i] = 1;
	}
	return b;
}

int * king_castling_non_attack(int rank){

	int * b = blank_bit_array();
	for (int i = 4; i<7;i++){
		b[(rank * 8) + i] = 1;
	}
	return b;
}
int * queen_castling_non_attack(int rank){

	int * b = blank_bit_array();
	for (int i = 2; i<5;i++){
		b[(rank * 8) + i] = 1;
	}
	return b;
}

int * starting_pawns(int rank){
	int * b = blank_bit_array();
	int j=rank*8;
	for (int i=j; i<j+8;i++) b[i] = 1;
	return b;
}
int * starting_pawns_white(){
	return starting_pawns(1);
}
int * starting_pawns_black() {
	return starting_pawns(6);
}
int * starting_pieces(int rank,int offset){
	int * b = blank_bit_array();
	int i=rank*8;
	b[i + offset] = 1;
	b[i + 7 - offset] = 1;
	return b;
}
int * starting_royalty(int rank, int offset){
	int * b = blank_bit_array();
	int i=rank*8;
	b[i + offset] = 1;
	return b;
}
int * starting_king_white(){
	return starting_royalty(0,4);
}
int * starting_king_black(){
	return starting_royalty(7,4);
}
int * starting_queen_white(){
	return starting_royalty(0,3);
}
int * starting_queen_black(){
	return starting_royalty(7,3);
}
int * starting_rooks_white(){
	return starting_pieces(0,0);
}
int * starting_rooks_black(){
	return starting_pieces(7,0);
}
int * starting_knights_white(){
	return starting_pieces(0,1);
}
int * starting_knights_black(){
	return starting_pieces(7,1);
}
int * starting_bishops_white(){
	return starting_pieces(0,2);
}
int * starting_bishops_black(){
	return starting_pieces(7,2);
}

