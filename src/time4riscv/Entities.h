#ifndef ENTITIES_H  // 1. Check if the guard symbol is NOT defined
#define ENTITIES_H  // 2. Define the guard symbol
#include <math.h>
#include "GameState.h"
#include "math.c" // For Dict_set_value

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

void Ai_init(volatile Ai* ai, int id, int color, int x_pos, int y_pos) {
    ai->x_pos = x_pos; //TODO: Random start position
    ai->y_pos = y_pos; //TODO: Random start position
    ai->area = 100;
    ai->color = color;
    ai->velocity = 5; // Inital velocity
    ai->dx = 0;
    ai->dy = 0;
    ai->radius = (int) sqrt(ai->area/3.14);
    ai->id = id;
}

void Player_init(volatile Player* p, int id, int color, int x_pos, int y_pos) {
    p->x_pos = x_pos; //TODO: Random start position
    p->y_pos = y_pos; //TODO: Random start position
    p->area = 100;
    p->color = color;
    p->velocity = 5; // Inital velocity
    p->dx = 0;
    p->dy = 0;
    p->radius = (int) sqrt(p->area/3.14);
    p->id = id;
}

void Food_init(volatile Food* f, int id, int type, int x_pos, int y_pos) {
    f->radius = 1;
    f->x_pos = x_pos; //TODO: Random start position
    f->y_pos = y_pos; //TODO: Random start position
    f->type = type;
    switch (type)
    {
    case 0: // Banana
        f->nutrition = 5;
        break;
    case 1: // Apple
        f->nutrition = 10;
        break;
    case 2: // Pear
        f->nutrition = 15;
        break;
    case 3: // Grape
        f->nutrition = 20;
        break;
    default:
        f->nutrition = 0;
        break;
    }
    f->id = id;
}

void Player_update_position(volatile Player* p, volatile GameState* gs, int x_ctrl, int y_ctrl) {
    // Reset occupied_coords_dict entry for old position
    Dict_set_value(&gs->occupied_coords_dict, (p->x_pos << 16) | p->y_pos, -1);

    // Determine direction based on control input
    int x_sign;
    int y_sign;

        switch (x_ctrl)
        {
        case 0:
            x_sign = -1;
            break;
        case 1:
            x_sign = 1;
        default:
            break;
        }
        
        switch (y_ctrl)
        {
        case 0:
            y_sign = -1;
            break;
        case 1:
            y_sign = 1;
            break;
        default:
            break;
        }

        // Calculate displacement
        p->dx = x_sign * p->velocity;
        p->dy = y_sign * p->velocity;

        // Calculate new position
        int p_old_x_pos = p->x_pos;
        int p_old_y_pos = p->y_pos;

        int new_x_pos = p_old_x_pos + p->dx;
        int new_y_pos = p_old_y_pos + p->dy;

        // Check x-axis boundaries
        if (gs->min_x < new_x_pos - p->radius) { 
            if (gs->max_x > new_x_pos + p->radius) {
                p->x_pos = new_x_pos;
            }
        }
        // Check y-axis boundaries
        if (gs->min_y < new_y_pos - p->radius) {
                if (gs-> max_y > new_y_pos + p->radius) {
                    p->y_pos = new_y_pos;
                }
        }
        // Update occupied coords dictionary in GameState
        int coord_key = (p->x_pos << 16) | p->y_pos;
}

void Player_update_velocity(volatile Player* p) {
    p->velocity = 5 + (int)(sqrt((double)p->area) / 10.0); // Example: velocity increases with the square root of area
}

void AI_update_position(volatile Ai* ai, volatile GameState* gs, int x_ctrl, int y_ctrl) {
    
    Dict_set_value(&gs->occupied_coords_dict, (ai->x_pos << 16) | ai->y_pos, -1);
    int x_sign;
        int y_sign;

        switch (x_ctrl)
        {
        case 0:
            x_sign = -1;
            break;
        case 1:
            x_sign = 1;
        default:
            break;
        }
        
        switch (y_ctrl)
        {
        case 0:
            y_sign = -1;
            break;
        case 1:
            y_sign = 1;
            break;
        default:
            break;
        }

        // Calculate displacement
        ai->dx = x_sign * ai->velocity;
        ai->dy = y_sign * ai->velocity;

        // Calculate new position
        int ai_old_x_pos = ai->x_pos;
        int ai_old_y_pos = ai->y_pos;

        int new_x_pos = ai_old_x_pos + ai->dx;
        int new_y_pos = ai_old_y_pos + ai->dy;

        // Check x-axis boundaries
        if (gs->min_x < new_x_pos - ai->radius) {
            if (gs->max_x > new_x_pos + ai->radius) {
                ai->x_pos = new_x_pos;
            }
        }
        // Check y-axis boundaries
        if (gs->min_y < new_y_pos - ai->radius) {
                if (gs-> max_y > new_y_pos + ai->radius) {
                    ai->y_pos = new_y_pos;
                }
        }
}

void AI_update_velocity(volatile Ai* ai) {
    ai->velocity = 5 + (int)(sqrt((double)ai->area) / 10.0); // Example: velocity increases with the square root of area
}

#endif // ENTITIES_H  // 3. Close the conditional block