#ifndef ENTITIES_H  // 1. Check if the guard symbol is NOT defined
#define ENTITIES_H  // 2. Define the guard symbol
#include <math.h>
#include <stdbool.h>
//#include "GameState.h"

// 1. FORWARD DECLARATION for GameState
typedef struct GameState GameState; 
// // 2. FORWARD DECLARATION for Dictionary (Used in GameState struct)
// typedef struct Dictionary Dictionary;

typedef struct {
    bool is_active;
    int x_fp;      // Q16.16 fixed-point x position
    int y_fp;      // Q16.16 fixed-point y position
    int area;
    int radius;
    int color;
    int vel_fp;    // Q16.16 fixed-point velocity
    int dx, dy;
} Player;

typedef struct {
    bool is_active;
    int radius;
    int x_pos;
    int y_pos;
    int nutrition;
    int type;
} Food;

typedef struct {
    bool is_active;
    int x_fp;      // Q16.16 fixed-point x position
    int y_fp;      // Q16.16 fixed-point y position
    int area;
    int radius;
    int color;
    int vel_fp;    // Q16.16 fixed-point velocity
    int dx, dy;
} Ai;

void Ai_init(volatile Ai* ai, int color, int x_pos, int y_pos);

void Player_init(volatile Player* p, int color, int x_pos, int y_pos);

void Food_init(volatile Food* f, int type, int x_pos, int y_pos);

void Player_update_position(volatile Player* p, volatile GameState* gs, int x_ctrl, int y_ctrl);

void Player_update_velocity(volatile Player* p);

void AI_update_position(volatile Ai* ai, volatile GameState* gs, int x_ctrl, int y_ctrl);

void AI_update_velocity(volatile Ai* ai);

#endif // ENTITIES_H  // 3. Close the conditional block