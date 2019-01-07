#include "../transpose_table.c"

int main () {


    Zob * z = make_zob_struct();
    save_zobrist(z);
    z = zob_from_file("../../data/zobrist");







}