#include <math.h>
#include <stdbool.h>
#include "Entities.h"
#include "GameState.h"
#include "math_tools.h" // For Dict_set_value

void Player_init(volatile Player* p, int color, int x_pos, int y_pos) {
    p->x_fp = INT_TO_FP(x_pos);  // Convert pixel pos to Q16.16
    p->y_fp = INT_TO_FP(y_pos);  // Convert pixel pos to Q16.16
    p->area = 100;
    p->color = color;
    p->vel_fp = INT_TO_FP(2);    // Initial velocity: 2.0 px/frame
    p->dx = 0;
    p->dy = 0;
    p->radius = int_sqrt(p->area*100/314);
    p->is_active = true;
    //print("-------- Player initialized\n");
}

void Ai_init(volatile Ai* ai, int color, int x_pos, int y_pos) {
    ai->x_fp = INT_TO_FP(x_pos);  // Convert pixel pos to Q16.16
    ai->y_fp = INT_TO_FP(y_pos);  // Convert pixel pos to Q16.16
    ai->area = 100;
    ai->color = color;
    ai->vel_fp = INT_TO_FP(2);    // Initial velocity: 2.0 px/frame
    ai->dx = 0;
    ai->dy = 0;
    ai->radius = int_sqrt(ai->area*100/314);
    ai->is_active = true;
    //print("-------- Calculating radius for AI with area \n");
    //print("-------- AI initialized: id=...d, pos=(...d,...d), color=...d, radius=\n");
}

void Food_init(volatile Food* f, int type, int x_pos, int y_pos) {
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
    f->is_active = true;
    //print("------ Food initialized: id=...d, type=...d, nutrition=...d, pos=(...d,...d), radius=\n");
}

void Player_update_position(volatile Player* p, volatile GameState* gs, int x_ctrl, int y_ctrl) {
    // Determine direction based on control input
    int x_sign = (x_ctrl == 0) ? -1 : (x_ctrl == 1) ? 1 : 0;
    int y_sign = (y_ctrl == 0) ? -1 : (y_ctrl == 1) ? 1 : 0;

    // Calculate new fixed-point position
    int new_x = p->x_fp + x_sign * p->vel_fp;
    int new_y = p->y_fp + y_sign * p->vel_fp;

    // Boundary check using integer pixel conversion
    int px = FP_TO_INT(new_x);
    int py = FP_TO_INT(new_y);

    if (px - p->radius > gs->min_x && px + p->radius < gs->max_x)
        p->x_fp = new_x;
    if (py - p->radius > gs->min_y && py + p->radius < gs->max_y)
        p->y_fp = new_y;
}

void Player_update_velocity(volatile Player* p) {
    // Base speed = 2.0 px/frame, decreases by 0.1 per 50 area units
    // Minimum speed = 0.25 px/frame
    int vel = INT_TO_FP(2) - (p->area * FP_ONE / 500);
    if (vel < FP_ONE / 4) vel = FP_ONE / 4;  // min 0.25 px/frame
    p->vel_fp = vel;
}

void AI_update_position(volatile Ai* ai, volatile GameState* gs, int x_ctrl, int y_ctrl) {
    // Determine direction based on control input
    int x_sign = (x_ctrl == 0) ? -1 : (x_ctrl == 1) ? 1 : 0;
    int y_sign = (y_ctrl == 0) ? -1 : (y_ctrl == 1) ? 1 : 0;

    // Calculate new fixed-point position
    int new_x = ai->x_fp + x_sign * ai->vel_fp;
    int new_y = ai->y_fp + y_sign * ai->vel_fp;

    // Boundary check using integer pixel conversion
    int px = FP_TO_INT(new_x);
    int py = FP_TO_INT(new_y);

    if (px - ai->radius > gs->min_x && px + ai->radius < gs->max_x)
        ai->x_fp = new_x;
    if (py - ai->radius > gs->min_y && py + ai->radius < gs->max_y)
        ai->y_fp = new_y;
}

void AI_update_velocity(volatile Ai* ai) {
    // Base speed = 2.0 px/frame, decreases by 0.1 per 50 area units
    // Minimum speed = 0.25 px/frame
    int vel = INT_TO_FP(2) - (ai->area * FP_ONE / 500);
    if (vel < FP_ONE / 4) vel = FP_ONE / 4;  // min 0.25 px/frame
    ai->vel_fp = vel;
}