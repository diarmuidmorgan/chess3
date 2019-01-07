#include "../transpose_table.c"

int main () {


    Zob * z = make_zob_struct();
    save_zobrist(z, "../../data/zobrist");
    Zob *g = zob_from_file("../../data/zobrist");
    int i = 0;
    while (i<300){

        if (z->vals[i] != g->vals[i])
            printf("%" PRIu64 " %" PRIu64 "\n", z->vals[i], g->vals[i]);
            printf("FUCKED\n");
        i++;

    }







}