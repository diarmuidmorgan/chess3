#include "game_state.c"
#include <stdio.h>
typedef struct {

    GS Stack[150];
    int top;
    


} Game_Stack;

Game_Stack * new_game_stack(){

    Game_Stack * s = malloc(sizeof(Game_Stack));
    s->top = 0;
    return s;
}
void game_stack_push(GS * gs, Game_Stack * s){
    // DANGER WITHIN. DON't KNOW HOW TO WRITE EXCEPTIONS.
    if (s->top == 149) printf("EVERYTHING IS FUCKED CRASHING NOW");
    s->Stack[s->top] = *gs;
    s->top = s->top + 1;
}
GS * game_stack_pop (Game_Stack * s){


    return NULL;

}
int main(){

    Game_Stack * gs = new_game_stack();
    int d;
    scanf("%d", &d);
}