#ifndef PROG_STATES_H
#define PROG_STATES_H

#include <stdbool.h>
#include "GameState.h"

typedef enum {
    STATE_SPLASH,
    STATE_MENU_START,
    STATE_TRANSITION,
    STATE_PLAYING,
    STATE_PAUSED,
    STATE_GAME_OVER
} ProgramState;

// Generalized transition trigger
ProgramState set_transition(ProgramState from, ProgramState to);

// Per-frame state handlers (non-blocking, called once per tick)
ProgramState state_splash(void);
ProgramState state_menu_start(GameState* gs);
ProgramState state_transition(void);
ProgramState state_playing(GameState* gs, int input_vector[]);
ProgramState state_paused(void);
ProgramState state_game_over(void);

// Edge detection helper
bool btn_just_pressed(void);
int get_btn_action(void);

#endif