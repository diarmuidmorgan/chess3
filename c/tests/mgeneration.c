#include "../move_generators2.c"
int main () {

    uint64_t * msks = build_mask_object();
    GS gs = init_game_state();
    int depth = 6;
    int position_evals = 0;
    game_loop(&gs, msks, depth, &position_evals, 0 );
    printf("%d\n", position_evals);


}