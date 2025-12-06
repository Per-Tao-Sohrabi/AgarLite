#ifndef ENTITIES_H  // 1. Check if the guard symbol is NOT defined
#define ENTITIES_H  // 2. Define the guard symbol
#include <math.h>
//#include "GameState.h"

// 1. FORWARD DECLARATION for GameState
typedef struct GameState GameState; 
// // 2. FORWARD DECLARATION for Dictionary (Used in GameState struct)
// typedef struct Dictionary Dictionary;

typedef struct {
    int x_pos;
    int y_pos;
    int area;
    int color;
    int velocity;
    int dx, dy;
    int radius;
    int id;
} Player;

typedef struct {
    int x_pos;
    int y_pos;
    int nutrition;
    int type;
    int id;
    int radius;
} Food;

typedef struct {
    int x_pos;
    int y_pos;
    int area;
    int color;
    int velocity;
    int dx, dy;
    int radius;
    int id;
} Ai;

void Ai_init(volatile Ai* ai, int id, int color, int x_pos, int y_pos);

void Player_init(volatile Player* p, int id, int color, int x_pos, int y_pos);

void Food_init(volatile Food* f, int id, int type, int x_pos, int y_pos);

void Player_update_position(volatile Player* p, volatile GameState* gs, int x_ctrl, int y_ctrl);

void Player_update_velocity(volatile Player* p);

void AI_update_position(volatile Ai* ai, volatile GameState* gs, int x_ctrl, int y_ctrl);

void AI_update_velocity(volatile Ai* ai);

#endif // ENTITIES_H  // 3. Close the conditional block