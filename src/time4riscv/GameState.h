#ifndef GAMESTATE_H  // 1. Check if the guard symbol is NOT defined
#define ENTITIES_H 

#include "Entities.h"

typedef struct {
    int var1;
    int var2;
    Player players[2]; // Can hold two players
    Food crumbs[20]; // Can hold five food pieces
} GameState;

void GameState_generate_playes(GameState* gms, int num_p);

void GameState_init(){
    // Initialize game state
}

#endif









