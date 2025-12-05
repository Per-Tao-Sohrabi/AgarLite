#include "Entities.h"
#include <stdbool.h>
#include "math.c"

extern int rand_range(int, int);

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

    Player players[MAXPLAYERS]; // Can hold two players
    Food crumbs[MAXFOOD]; // Can hold five food pieces
    Ai ais[MAXAI];

    //Dict and data
    bool available_ids[MAXPLAYERS + MAXAI + MAXFOOD]; // To track available entity IDs.


    Dictionary occupied_coords_dict; // Key. coord. Value: entity ID
    Dictionary id_entity_dict; // Key: entity ID. Value: entity type

    

} GameState;

/* Initializes the game state */
void GameState_init(GameState* gs, int gm, int diff){
    
    // Populate ID pool
    for(int i = 0; i< sizeof(gs->available_ids); i++) {
        gs->available_ids[i] = true; // All IDs are available at start
    }
    
    // Set game mode and difficulty
    gs->game_mode = gm;
    gs->difficulty = diff;

    // Set boundaries
    gs->min_x = 0;
    gs->max_x = 320;
    gs->min_y = 0;
    gs->max_y = 480;
    
    // Generate entities
    GameState_generate_food(gs, diff);
    GameState_generate_ai(gs, diff);
    GameState_generate_playes(gs, gm);
}

/* Generate Players, Food, and AI based on game mode and difficulty */
void GameState_generate_playes(GameState* gs, int num_p) {
    volatile int colors[] = {100, 200, 150, 50, 250};
    for(int i = 0; i<num_p; i++) {
        Player p;
        p.color = colors[i];
        gs->players[i] = p;
        // Set initial positions
    }
}

/* Generate Food based on game mode and difficulty */
void GameState_generate_food(GameState* gs, int gm, int diff) {
    // Calc numb of food based on difficulty (from the 7 diff levels) and gm (2 gm). 
    int base_food_n = 10;
    int diff_food_mod = diff * 2; // Each diff level adds 2 food items
    int gm_food_mod = (gm == 2) ? 5 : 0; // If 2 players, add 5 food items
    int total_food_n = base_food_n + diff_food_mod + gm_food_mod;
    if (total_food_n > MAXFOOD) {
        total_food_n = MAXFOOD; // Cap to MAXFOOD
    }
    int prob_pear = 100 - (diff * 10); // Decrease pear prob with diff, high energy
    int prob_apple = (100-prob_pear)/2; // Increase apple prob with diff
    int prob_banana = 100-prob_pear-prob_apple; // Increase banana prob with diff 
    
    // Generate food items
    for(int i = 0; i<total_food_n; i++) {
        Food f;                             // Create food item
        // Generate pseudo-random number for nutrition type
        int r = rand_range(1,100);
        // Determine nutrition based on probability
        if (r < prob_banana) {
            f.type = "banana"; // Banana
        } else if (r < prob_apple) {
            f.type = "apple"; // Apple
        } else if (r < prob_pear) {
            f.type = "pear"; // Pear
        } else {
            f.type = "grape"; // Grape
        }
        // take id from available ids, then update available ids
        int id_candiate = (rand_range(0, sizeof(gs->available_ids-1)));
        while(gs->available_ids[id_candiate] == false) {
            id_candiate = (rand_range(0, sizeof(gs->available_ids-1)));
        
        f.id = id_candiate;
        gs->available_ids[id_candiate] = false; // Mark ID as used
        gs->crumbs[i] = f;

        // Set random position
        int x_pos = rand_range(gs->min_x, gs->max_x);
        int y_pos = rand_range(gs->min_y, gs->max_y);
        
        // Check for occupied positions, if occupied, regenerate
        while (Dict_get(&gs->occupied_coords_dict, (x_pos << 16) | y_pos) != -1) {
            x_pos = rand_range(gs->min_x, gs->max_x);
            y_pos = rand_range(gs->min_y, gs->max_y);
        }
        f.x_pos = x_pos;
        f.y_pos = y_pos;

        // Data
        int coord_key = (f.x_pos << 16) | f.y_pos; // Combine x and y into a single key
        Dict_insert(&gs->occupied_coords_dict, coord_key, f.id); // Key: combined coord, Value: food index
        Dict_insert(&gs->id_entity_dict, f.id, 1); // Key: food index, Value: entity type (1 for food)
        // To check, get the type and then check the id in the type entity list. 
    }
}

/* Generate AI based on difficulty */
void GameState_generate_ai(GameState* gs, int diff) {
    int total_ai_n = diff * 2; // Each diff level adds 2 AI
    if (total_ai_n > MAXAI) {
        total_ai_n = MAXAI; // Cap to MAXAI
    }
    volatile int colors[] = {120, 220, 170, 70, 255, 30};
    for(int i = 0; i<total_ai_n; i++) {
        Ai ai;
        ai.color = colors[i];
        gs->ais[i] = ai;
    }
}

/* Update the Game State
- Entity movements
    - Collision detection
*/
void GameState_update(GameState* gs, int input_vector[]) {
    // UPDATE PLAYER POSITION
    for(int i = 0; i< sizeof(gs->players); i++) {
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

        // Calculate displacement
        pi->dx = x_sign * pi->velocity;
        pi->dy = y_sign * pi->velocity;

        // Calculate new position
        int pi_old_x_pos = pi->x_pos;
        int pi_old_y_pos = pi->y_pos;

        int new_x_pos = pi->x_pos + pi->dx;
        int new_y_pos = pi->y_pos + pi->dy;

        // Check side boundaries
        if (gs->min_x < new_x_pos - pi->radius) {
            if (gs->max_x > new_x_pos + pi->radius) {
                pi->x_pos = new_x_pos;
            }
        }
        if (gs->min_y < new_y_pos - pi->radius) {
                if (gs-> max_y > new_y_pos + pi->radius) {
                    pi->y_pos = new_y_pos;
                }
        }
    }

    // UPDATE FOOD

    
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