
#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Entities.h"
#include "math_tools.h"

#include <stdbool.h>
#include <math.h>
#include <stddef.h>

extern int rand_range(int, int);

#define MAXPLAYERS 2
#define MAXAI 0
#define MAXFOOD 5

typedef struct GameState{
    int ticks;
    int difficulty;
    int game_mode;
    int min_x;
    int max_x;
    int min_y;
    int max_y;

    Player players[MAXPLAYERS]; // Can hold two players
    Food crumbs[MAXFOOD]; // Can hold five food pieces
    Ai ais[MAXAI];

    // //Dict and data
    // bool available_ids[MAXPLAYERS + MAXAI + MAXFOOD]; // To track available entity IDs.

    //Dictionary occupied_coords_dict; // Key. coord id. Value: entity ID
    //Dictionary id_type_dict; // Key: entity ID. Value: entity type
} GameState;

/* Initializes the game state */
void GameState_init(volatile GameState*  gs, int gm, int diff);
// Check for occupied positions, if occupied, regenerate
int GameState_get_random_position(volatile GameState*  gs);
/* Helper function to get free ids*/
int GameState_get_free_id(volatile GameState*  gs);
/* Generate Players, Food, and AI based on game mode and difficulty */
void GameState_generate_players(volatile GameState*  gs, int game_mode);
/* Generate Food based on game mode and difficulty */ // TODO: Skapa Food_init().
void GameState_generate_food(volatile GameState*  gs, int gm, int diff);
/* Generate AI based on difficulty */
void GameState_generate_ai(volatile GameState*  gs, int diff);
/* Update the Game State
- Entity movements
    - Collision detection
*/
bool GameState_update(volatile GameState*  gs, int input_vector[]);
// CHECK COLLISIONS BETWEEN ENTITIES
bool check_player_food_collision(volatile Player*  p, volatile Food*  f);
bool check_player_ai_collision(volatile Player*  p, volatile Ai*  ai);
bool check_ai_ai_collision(volatile Ai*  ai1, volatile Ai*  ai2);
bool check_ai_food_collision(volatile Ai*  ai, volatile Food*  f);
bool check_player_player_collision(volatile Player*  p1, volatile Player*  p2);
/* HANDLE COLLISION RESPONSES */
/* Handle player ai collisions*/
void GameState_handle_player_ai_collision(volatile GameState*  gs, volatile Player*  p, volatile Ai*  ai);

/* Handle player player collisions*/
void GameState_handle_player_player_collision(volatile GameState*  gs, volatile Player*  p1, volatile Player*  p2);
/* Handle player food collision*/   
void GameState_handle_player_food_collision(volatile GameState*  gs, volatile Player*  p, volatile Food*  f);/* Handle ai ai collisions*/
void GameState_handle_ai_ai_collision(volatile GameState*  gs, volatile Ai*  ai1, volatile Ai*  ai2);
/* Handle ai food collisions*/
void GameState_handle_ai_food_collision(volatile GameState*  gs, volatile Ai*  ai, volatile Food*  f);

#endif
