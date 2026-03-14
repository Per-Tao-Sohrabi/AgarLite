
#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Entities.h"
#include "math_tools.h"

#include <stdbool.h>
#include <stddef.h>

extern int rand_range(int, int);



typedef struct GameState {
    int ticks;
    int difficulty;
    int game_mode;
    int min_x;
    int max_x;
    int min_y;
    int max_y;

    Entity entities[MAX_ENTITIES];
    int num_players;    // entities[0..num_players-1] are players
    int num_ai;         // entities[num_players..num_players+num_ai-1] are AI
    int num_food;       // remaining slots are food
} GameState;

/* Initializes the game state */
void GameState_init(GameState* gs, int gm, int diff);

/* Generate random non-overlapping position, returns packed coord */
int GameState_get_random_position(GameState* gs);

/* Generate entities */
void GameState_generate_players(GameState* gs, int game_mode);
void GameState_generate_food(GameState* gs, int gm, int diff);
void GameState_generate_ai(GameState* gs, int diff);

/* Update the game state (movement + collision). Returns true on game over. */
bool GameState_update(GameState* gs, int input_vector[]);

/* Collision check (unified) */
bool check_collision(Entity* a, Entity* b);

/* Collision handlers */
void handle_entity_eat(GameState* gs, Entity* eater, Entity* eaten);
void handle_food_eat(GameState* gs, Entity* eater, Entity* food);

#endif
