#include <stdlib.h>
#include <stdio.h>

int * blank_bit_array() {
	
	int * b = malloc(64*sizeof(int));
	for(int i=0;i<64;i++) b[i] = 0;
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

