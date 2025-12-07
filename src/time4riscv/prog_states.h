#ifndef PROG_STATES_H
#define PROG_STATES_H

#include "GameState.h"
#include "inputs.h"

// Initalize a game state
GameState run_start_up();
/* Query the player about the game mode they want to play at*/
int query_game_mode();

/* Query the player about the difficulty they want to play at*/
int query_game_difficulty();
/* Pause logic*/
void run_pause();

/* Game Over*/
void run_game_over();

#endif