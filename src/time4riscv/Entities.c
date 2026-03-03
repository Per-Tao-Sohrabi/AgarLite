#include <stdbool.h>
#include "Entities.h"
#include "GameState.h"
#include "math_tools.h"

void Entity_init_player(Entity* e, int color, int x, int y) {
    e->is_active = true;
    e->type = ENTITY_PLAYER;
    e->x_fp = INT_TO_FP(x);
    e->y_fp = INT_TO_FP(y);
    e->area = 100;
    e->color = color;
    e->vel_fp = INT_TO_FP(2);   // 2.0 px/frame
    e->radius = int_sqrt(e->area * 100 / 314);
    e->nutrition = 0;
}

void Entity_init_ai(Entity* e, int color, int x, int y) {
    e->is_active = true;
    e->type = ENTITY_AI;
    e->x_fp = INT_TO_FP(x);
    e->y_fp = INT_TO_FP(y);
    e->area = 100;
    e->color = color;
    e->vel_fp = INT_TO_FP(2);   // 2.0 px/frame
    e->radius = int_sqrt(e->area * 100 / 314);
    e->nutrition = 0;
}

void Entity_init_food(Entity* e, int type, int x, int y) {
    e->is_active = true;
    e->type = ENTITY_FOOD;
    e->x_fp = INT_TO_FP(x);
    e->y_fp = INT_TO_FP(y);
    e->area = 0;
    e->vel_fp = 0;
    e->radius = 1;

    // Set nutrition based on food type, reuse color for rendering
    switch (type) {
    case 0: e->nutrition = 5;  e->color = 5;  break; // Banana
    case 1: e->nutrition = 10; e->color = 10; break; // Apple
    case 2: e->nutrition = 15; e->color = 15; break; // Pear
    case 3: e->nutrition = 20; e->color = 20; break; // Grape
    default: e->nutrition = 0; e->color = 0;  break;
    }
}

void Entity_update_position(Entity* e, GameState* gs, int x_ctrl, int y_ctrl) {
    // Determine direction based on control input
    int x_sign = (x_ctrl == 0) ? -1 : (x_ctrl == 1) ? 1 : 0;
    int y_sign = (y_ctrl == 0) ? -1 : (y_ctrl == 1) ? 1 : 0;

    // Calculate new fixed-point position
    int new_x = e->x_fp + x_sign * e->vel_fp;
    int new_y = e->y_fp + y_sign * e->vel_fp;

    // Boundary check using integer pixel conversion
    int px = FP_TO_INT(new_x);
    int py = FP_TO_INT(new_y);

    if (px - e->radius < gs->min_x) new_x = INT_TO_FP(gs->min_x + e->radius);
    else if (px + e->radius > gs->max_x) new_x = INT_TO_FP(gs->max_x - e->radius);
    
    if (py - e->radius < gs->min_y) new_y = INT_TO_FP(gs->min_y + e->radius);
    else if (py + e->radius > gs->max_y) new_y = INT_TO_FP(gs->max_y - e->radius);

    e->x_fp = new_x;
    e->y_fp = new_y;
}

void Entity_update_velocity(Entity* e) {
    // Base speed = 2.0 px/frame, decreases with area
    // Minimum speed = 0.25 px/frame
    int vel = INT_TO_FP(2) - (e->area * FP_ONE / 500);
    if (vel < FP_ONE / 4) vel = FP_ONE / 4;
    e->vel_fp = vel;
}