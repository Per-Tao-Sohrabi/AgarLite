#ifndef ENTITIES_H
#define ENTITIES_H
#include <stdbool.h>
#include "math_tools.h"

// Forward declaration for GameState
typedef struct GameState GameState;

typedef enum {
    ENTITY_DEAD = 0,
    ENTITY_PLAYER,
    ENTITY_AI,
    ENTITY_FOOD
} EntityType;

#define MAXPLAYERS 2
#define MAXAI 5
#define MAXFOOD 10

#define MAX_ENTITIES (MAXPLAYERS + MAXAI + MAXFOOD)

typedef struct {
    bool is_active;
    EntityType type;
    int x_fp;       // Q16.16 fixed-point x position
    int y_fp;       // Q16.16 fixed-point y position
    int area;       // size metric (food: 0)
    int radius;
    int color;      // grayscale value (food: uses nutrition for render color)
    int vel_fp;     // Q16.16 velocity (food: 0)
    int nutrition;  // food only (players/ai: 0)
    int lives;      // number of remaining lives (players only)
} Entity;

// Init functions
void Entity_init_player(Entity* e, int color, int x, int y);
void Entity_init_ai(Entity* e, int color, int x, int y);
void Entity_init_food(Entity* e, int type, int x, int y);

// Update functions (only for ENTITY_PLAYER and ENTITY_AI)
void Entity_update_position(Entity* e, GameState* gs, int x_ctrl, int y_ctrl);
void Entity_update_velocity(Entity* e);

#endif // ENTITIES_H