#include <math.h>

#include "Entities.h"
#include "GameState.h"
#include "math_tools.h" // For Dict_set_value

void Player_init(volatile Player* p, int id, int color, int x_pos, int y_pos) {
    p->x_pos = x_pos; //TODO: Random start position
    p->y_pos = y_pos; //TODO: Random start position
    p->area = 100;
    p->color = color;
    p->velocity = 1; // Inital velocity
    p->dx = 0;
    p->dy = 0;
    p->radius = sqrt(p->area*100/314);
    p->id = id;
    //print("-------- Player initialized\n");
}

void Ai_init(volatile Ai* ai, int id, int color, int x_pos, int y_pos) {
    ai->x_pos = x_pos; //TODO: Random start position
    ai->y_pos = y_pos; //TODO: Random start position
    ai->area = 100;
    ai->color = color;
    ai->velocity = 1; // Inital velocity
    ai->dx = 0;
    ai->dy = 0;
    ai->radius = sqrt(ai->area*100/314);
    ai->id = id;
    //print("-------- Calculating radius for AI with area \n");
    //print("-------- AI initialized: id=...d, pos=(...d,...d), color=...d, radius=\n");
}

void Food_init(volatile Food* f, int id, int type, int x_pos, int y_pos) {
    f->radius = 1;
    f->x_pos = x_pos; //TODO: Random start position
    f->y_pos = y_pos; //TODO: Random start position
    f->type = type;
    //print("------- Setting nutrition for food type ...d\n");
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
    //print("------ Food initialized: id=...d, type=...d, nutrition=...d, pos=(...d,...d), radius=\n");
}

void Player_update_position(volatile Player* p, volatile GameState* gs, int x_ctrl, int y_ctrl) {
    // Reset occupied_coords_dict entry for old position
    Dict_set_value(&gs->occupied_coords_dict, (p->x_pos << 16) | p->y_pos, -1);

    // Determine direction based on control input
    int x_sign = 0;
    int y_sign = 0;

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
        if (gs->min_x < new_x_pos - ((int)p->radius)) { 
            if (gs->max_x > new_x_pos + ((int)p->radius)) {
                p->x_pos = new_x_pos;
            }
        }
        // Check y-axis boundaries
        if (gs->min_y < new_y_pos - ((int)p->radius)) {
                if (gs-> max_y > new_y_pos + ((int)p->radius)) {
                    p->y_pos = new_y_pos;
                }
        }
        // Update occupied coords dictionary in GameState
        int coord_key = (p->x_pos << 16) | p->y_pos;
}

void Player_update_velocity(volatile Player* p) {
    int base_velocity = 1;
    p->velocity = base_velocity - ((int)p->area / 100);
    if (p->velocity < 1) {
        p->velocity = 1;
    }
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
        if (gs->min_x < new_x_pos - ((int)ai->radius)) {
            if (gs->max_x > new_x_pos + ((int)ai->radius)) {
                ai->x_pos = new_x_pos;
            }
        }
        // Check y-axis boundaries
        if (gs->min_y < new_y_pos - (int)ai->radius) {
                if (gs-> max_y > new_y_pos + (int)ai->radius) {
                    ai->y_pos = new_y_pos;
                }
        }
}

void AI_update_velocity(volatile Ai* ai) {
    int base_velocity = 1;
    ai->velocity = base_velocity - ((int)ai->area / 100);
    if (ai->velocity < 1) {
        ai->velocity = 1;
    }
}