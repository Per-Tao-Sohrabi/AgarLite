#ifndef PROG_STATES_H
#define PROG_STATES_H

#include <stdbool.h>
#include "GameState.h"

typedef enum {
    STATE_MENU_MODE,
    STATE_MENU_DIFFICULTY,
    STATE_PLAYING,
    STATE_PAUSED,
    STATE_GAME_OVER
} ProgramState;

// Per-frame state handlers (non-blocking, called once per tick)
ProgramState state_menu_mode(GameState* gs);
ProgramState state_menu_difficulty(GameState* gs);
ProgramState state_playing(GameState* gs, int input_vector[]);
ProgramState state_paused(void);
ProgramState state_game_over(void);

// Edge detection helper
bool btn_just_pressed(void);

#endif