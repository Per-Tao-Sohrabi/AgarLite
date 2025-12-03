#include "Entities.h"
#include <stdbool.h>

#define MAXPLAYERS 2
#define MAXAI 6
#define MAXFOOD 20

typedef struct {
    int ticks;
    int difficulty;
    int game_mode;
    int min_x;
    int max_x;
    int min_y;
    int max_y;
    //char mode; 
    Player players[MAXPLAYERS]; // Can hold two players
    Food crumbs[MAXFOOD]; // Can hold five food pieces
    Ai ais[MAXAI];

} GameState;

void GameState_init(GameState* gs, int gm, int diff){
    gs->game_mode = gm;
    gs->difficulty = diff;
    gs->min_x = 0;
    gs->max_x = 320;
    gs->min_y = 0;
    gs->max_y = 480;
    GameState_generate_playes(gs, gm);
}

void GameState_generate_playes(GameState* gs, int num_p) {
    volatile int colors[] = {100, 200, 150, 50, 250};
    for(int i = 0; i<num_p; i++) {
        Player p;
        p.color = colors[i];
        gs->players[i] = p;
    }
}
void GameState_generate_food(GameState* gs, int diff) {
// ... 
}

void GameState_generate_ai(GameState* gs, int diff) {
// ...
}
void GameState_update(GameState* gs, int input_vector[]) {
    // Update game state based on input vector
    
    // UPDATE PLAYER POSITION
    for(int i = 0; i< gs->game_mode; i++) {
        Player* pi = &gs->players[i];
        
        // Read player input
        int x_sign;
        int y_sign;

        switch (input_vector[i])
        {
        case 0:
            x_sign = -1;
            break;
        case 1:
            x_sign = 1;
        default:
            break;
        }
        
        switch (input_vector[i+1])
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

        // Set new partial velocities
        pi->dx = x_sign * pi->velocity;
        pi->dy = y_sign * pi->velocity;

        // Calculate new position
        int pi_old_x_pos = pi->x_pos;
        int pi_old_y_pos = pi->y_pos;

        int new_x_pos = pi->x_pos + pi->dx;
        int new_y_pos = pi->y_pos + pi->dy;

        if (gs->min_x < new_x_pos) {
            if (gs->max_x > new_x_pos) {
                pi->x_pos = new_x_pos;
            }
        }
        if (gs->min_y < new_y_pos) {
            if (gs-> max_y > new_y_pos) {
                pi->y_pos = new_y_pos;
            }
        }
    }
    // UPDATE AI
    // ...
    // DETECT COLLISIONS
    for(int i = 0; i < gs->game_mode; i++) { // For every player
        // Fix a player
        Player* pi_ptr = &gs->players[i];
        int pi_pos[] = {pi_ptr->x_pos, pi_ptr->y_pos};

        for(int j = 0; j < MAXFOOD; j++) { // Check collision with food
            Food* food = &gs->crumbs[j];
            int food_pos[] = {food->x_pos, food->y_pos};
            bool col = check_collision(pi_pos, food_pos);
            if (col == false) { // If no collision, continue to next player. 
                continue;
            }
            // Collision Logic
            Player_update_hp(food->nutrition);                       // If collision, do something
            // Move the food to new position
            Food_new_position(food);
        }

        for(int k = 0; k < MAXAI; k++) {
            Ai* ai = &gs->ais[k];
            int ai_pos[] = {ai->x_pos, ai->y_pos};
            bool col = check_collision(pi_pos, ai_pos);
            if (col == false) { // If no collision, continue to next player. 
                continue;
            }
            // Collision Logic
        }
    }
}