#include <stdlib.h>
#include <stdint.h>
#include "printer.c"
//add castling
#define PAWNMVINDEX 0
#define PAWNATINDEX 2
#define KNIGHTMINDEX 4
#define KINGMINDEX 5
#define RANKUINDEX 6
#define COLLINDEX 7
#define RANKDINDEX 8
#define COLRINDEX 9
#define DIAGULINDEX 10
#define DIAGURINDEX 11
#define DIAGDLINDEX 12
#define DIAGDRINDEX 13
 
int valid_coords(int x, int y){
	
	if (x>=0 && x <=7 && y>=0 && y <= 7){
		return 1;
	}
	return 0;

}
uint64_t king_moves(int x, int y){
	int * b = blank_bit_array();
	for (int i=-1; i<2; i++) {

		for (int j=-1;j<2;j++) {

		if ( !(i==0 && j==0) && valid_coords(x+i,y+j)){
			b[ (x+i)*8 + y + j] = 1;
		}
		}
	}
	return board_to_word(b);
}

uint64_t knight_moves(int x, int y,int p){
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
	if (p) {
		for(int i=0; i<8; i++){
		for(int j=0; j<8;j++){
		printf("%d",b[i*8+j]);
		}
		printf("\n");
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
	
	if ((x==1 && direction == 1) || (x==6 && direction == -1)){
	if (valid_coords(x+(2*direction),y)){
		b[8*(x+(2*direction)) + y] = 1;
			}

	}
	
	if (valid_coords((x+direction), y)){
		b[8*(x+direction) + y] = 1;
	}
	return board_to_word(b);

}

uint64_t diagonal(int i, int j, int xincr, int yincr) {
	int x = i + xincr;
	int y = j + yincr;
	int * b = blank_bit_array();
	while (x>=0 && x <= 7 && y>=0 && y <= 7 ){
	
		b[x*8+y] = 1;
		x+= xincr;
		y+=yincr;
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
	return board_to_word(b);
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

/*
* packages all of the masks into one large array.
*
*/
uint64_t * build_mask_object() {
	int index;
	uint64_t * ob = malloc(64 * 14 * sizeof(uint64_t));
	for (int i=0; i<8;i++){

		for (int j=0; j<8;j++){

		index = (i*8 + j )*14;
		ob[index] = pawn_moves(i,j,1);
		ob[index+1] = pawn_moves(i,j,-1);
		ob[index+PAWNATINDEX] = pawn_attacks(i,j,1);
		ob[index+PAWNATINDEX+1] = pawn_attacks(i,j,-1);
		ob[index+KNIGHTMINDEX] = knight_moves(i,j,0);
		ob[index+KINGMINDEX] = king_moves(i,j);
		ob[index+RANKUINDEX] = rank_up(i,j);
		ob[index+COLLINDEX] = col_left(i,j);
		ob[index+RANKDINDEX] = rank_down(i,j);
		ob[index+COLRINDEX] = col_right(i,j);
		ob[index+DIAGULINDEX] = diagonal_up_left(i,j);
		ob[index+DIAGURINDEX] = diagonal_up_right(i,j);
		ob[index+DIAGDLINDEX] = diagonal_down_left(i,j);
		ob[index+DIAGDRINDEX] = diagonal_down_right(i,j);


		}
	}
	return ob;
}


typedef struct {

	uint64_t kingside_free[2];
	uint64_t queenside_free[2];
	uint64_t kingside_non_attack[2];
	uint64_t queenside_non_attack[2];
	uint64_t kingside_king_location[2];
	

} CS_mask;

CS_mask * build_castle_masks () {

	CS_mask * cs = malloc(sizeof (CS_mask));
	for (int i=0; i<2; i++){
		cs->kingside_free[i] = board_to_word(king_castling_empty(7*i));
		cs->queenside_free[i] = board_to_word(queen_castling_empty(7*i));
		cs->kingside_non_attack[i] = board_to_word(king_castling_non_attack(7*i));
		cs->queenside_non_attack[i] = board_to_word(queen_castling_non_attack(7*i));
		


	}


	return cs;

}


