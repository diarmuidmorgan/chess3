#include "start_positions.c"
#include <stdlib.h>
#include <stdint.h>
#include "binary_ops.c"
//add castliong
int valid_coords(int x, int y){
	
	if (x>=0 && x <=7 && y>=0 && y <= 7 && y!=x){
		return 1;
	}
	return 0;

}
uint64_t king_moves(int x, int y){
	int * b = blank_bit_array();
	for (int i=-1; i<2; i++) {

		for (int j=-1;j<2;j++) {

		if (valid_coords(x+i,y+j)){
			b[ (x+i)*8 + y + j] = 1;
		}
		}
	}
	return board_to_word(b);
}

uint64_t knight_moves(int x, int y){
	int * b = blank_bit_array();
	int moves[8][2] = {{2,1},{2,-1},{-2,1},{-2,-1},
		{1,2},{-1,2},{1,-2},{-1,-2}};
	for (int i = 0; i<8;i++){
	
		int ix = moves[i][0];
		int iy = moves[i][1];
		int nx = x + ix;
		int ny = y + iy;
		if (valid_coords(nx,ny)){

		b[8*nx + ny] = 1;
		}

	}
	return board_to_word(b);
}

uint64_t pawn_attacks(int x, int y, int direction){
	int * b = blank_bit_array();
	if (valid_coords(x+direction,y+1)){

	b[(x+direction) * 8 + y+1] = 1;
	}
	if (valid_coords(x+direction,y-1)){
		b[(x+direction) * 8 + y -1] = 1;
	}
	return board_to_word(b);

}
uint64_t pawn_moves(int x, int y, int direction) {
	int * b = blank_bit_array();
	if ((x==1 && direction == -1) || (y==1 && direction == 1)){
	if (valid_coords(x+(2*direction),y)){
		b[8*(x+(2*direction)) + y] = 1;
			}

	}
	if (valid_coords((x+direction), y)){
		b[8*(x+direction) + y] = 1;
	}
	return board_to_word(b);

}

uint64_t diagonal(int x, int y, int xincr, int yincr) {

	int * b = blank_bit_array();
	for (x+=xincr; x>=0 && x <= 7; x+=xincr){

		for(y+=yincr;y>=0 && y <= 7; y+=yincr){

		b[x*8+y] = 1;
		}
	}

	return board_to_word(b);
}
uint64_t diagonal_up_left(int x, int y){
	return diagonal(x,y,-1,-1);
}
uint64_t diagonal_up_right(int x, int y){
	return diagonal(x,y,-1,1);
}
uint64_t diagonal_down_left(int x,int y){
	return diagonal(x,y,1,-1);
}
uint64_t diagonal_down_right(int x, int y){
	return diagonal(x,y,1,1);
}
uint64_t rank(int x, int y,int direction){
	int * b = blank_bit_array();
	for (y+=direction; y>=0 && y<=7; y+=direction){
		b[x*8 + y] = 1;
	}
	return b;
}
uint64_t rank_up(int x, int y){
	return rank(x,y,-1);
}
uint64_t rank_down(int x, int y){
	return rank(x,y,1);
}
uint64_t column(int x, int y,int direction){
	int * b = blank_bit_array();
	for (x+=direction; x>=0 && x<=7; x+=direction){
		b[x*8 + y] = 1;
	}
	return board_to_word(b);}

uint64_t col_left(int x, int y){
	return column(x,y,-1);
}
uint64_t col_right(int x, int y){
	return column(x,y,1);
}

uint64_t * build_mask_object() {
	int index;
	uint64_t * ob = malloc(64 * 14 * sizeof(uint64_t));
	for (int i=0; i<8;i++){

		for (int j=0; j<8;j++){

		index = i*8*14 + j;
		ob[index] = pawn_moves(i,j,1);
		ob[index+1] = pawn_moves(i,j,-1);
		ob[index+2] = pawn_attacks(i,j,1);
		ob[index+3] = pawn_attacks(i,j,-1);
		ob[index+4] = knight_moves(i,j);
		ob[index+5] = king_moves(i,j);
		ob[index+6] = rank_up(i,j);
		ob[index+7] = rank_down(i,j);
		ob[index+8] = col_left(i,j);
		ob[index+9] = col_right(i,j);
		ob[index+10] = diagonal_up_left(i,j);
		ob[index+11] = diagonal_up_right(i,j);
		ob[index+12] = diagonal_down_left(i,j);
		ob[index+13] = diagonal_down_right(i,j);


		}
	}
	return ob;
}
int main () {

	uint64_t * b = build_mask_object;
	return 0;


}
