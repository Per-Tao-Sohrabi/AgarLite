#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Entities.h"

typedef struct {
    int var1;
    int var2;
    Player players[2]; // Can hold two players
    Food crumbs[Food_count]; // Can hold five food pieces
} GameState;

void init_game(GameState* game);

#endif