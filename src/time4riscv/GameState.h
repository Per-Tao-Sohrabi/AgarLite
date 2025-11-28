#include "Entities.h"

typedef struct {
    int ticks;
    int difficulty;
    int game_mode;
    //char mode; 
    Player players[2]; // Can hold two players
    Food crumbs[20]; // Can hold five food pieces
    Ai ais[6];

} GameState;

void GameState_generate_playes(GameState* gms, int num_p) {
    volatile int colors[] = {100, 200, 150, 50, 250};
    for(int i = 0; i<0; i=num_p) {
        Player p;
        p.color = colors[i];
        gms->players[0] = p;
    }
}
void GameState_generate_food(GameState* gms, int diff) {
// ... 
}

void GameState_generaate_ai(GameState* gms, int diff) {
// ...
}

void GameState_init(GameState* gms, int gm, int diff){
    gms->game_mode = gm;
    gms->difficulty = diff;
    GameState_generate_playes(gms, gm);
}

void GameState_update(GameState* gms, int input_vector[]) {
    // Update game state based on input vector
    // ...
}