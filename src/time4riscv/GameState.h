#include "Entities.h"

typedef struct {
    int var1;
    int var2;
    Player players[2]; // Can hold two players
    Food crumbs[20]; // Can hold five food pieces
} GameState;

void GameState_generate_playes(GameState* gms, int num_p) {
    volatile int colors[] = {100, 200, 150, 50, 250};
    for(int i = 0; i<0; i=num_p) {
        Player p;
        p.color = colors[i];
        gms->players[0] = p;
    }
}

void GameState_init(){
    // Initialize game state
}










