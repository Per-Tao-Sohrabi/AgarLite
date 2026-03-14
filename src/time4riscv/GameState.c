#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

#include "GameState.h"
#include "Entities.h"
#include "math_tools.h"

extern int rand_range(int, int);



/* Initializes the game state */
void GameState_init(GameState* gs, int gm, int diff) {
    gs->game_mode = gm;
    gs->difficulty = diff;

    // Set boundaries
    gs->min_x = 0;
    gs->max_x = 320;
    gs->min_y = 48;
    gs->max_y = 240;

    // Clear all entity slots
    for (int i = 0; i < MAX_ENTITIES; i++) {
        gs->entities[i].is_active = false;
        gs->entities[i].type = ENTITY_DEAD;
    }

    gs->num_players = 0;
    gs->num_ai = 0;
    gs->num_food = 0;

    // Generate entities
    GameState_generate_players(gs, gm);
    GameState_generate_food(gs, gm, diff);
    GameState_generate_ai(gs, diff);
}

/* Generate a random position that doesn't overlap existing entities */
int GameState_get_random_position(GameState* gs) {
    // print("------ Generating random position...\n");
    // int x_pos = rand_range(gs->min_x, gs->max_x);
    // int y_pos = rand_range(gs->min_y, gs->max_y);
    // int coord_key = (x_pos << 16) | y_pos;

    // // Check against all active entities
    // for (int i = 0; i < MAX_ENTITIES; i++) {
    //     Entity* e = &gs->entities[i];
    //     if (!e->is_active) continue;
    //     int e_coord = (FP_TO_INT(e->x_fp) << 16) | FP_TO_INT(e->y_fp);
    //     while (e_coord == coord_key) {
    //         x_pos = rand_range(gs->min_x, gs->max_x);
    //         y_pos = rand_range(gs->min_y, gs->max_y);
    //         coord_key = (x_pos << 16) | y_pos;
    //     }
    // }

    // print("------ Random position = (");
    // print_dec(x_pos);
    // print(", ");
    // print_dec(y_pos);
    // print(")\n");

    // return coord_key;

    int x_pos, y_pos;
    bool valid_position = false;

    Entity temp;
    temp.radius = 10;

    while(!valid_position){
        x_pos = rand_range(gs->min_x, gs->max_x);
        y_pos = rand_range(gs->min_y, gs->max_y);
        temp.x_fp = INT_TO_FP(x_pos);
        temp.y_fp = INT_TO_FP(y_pos);
        
        // assume the position is valid
        valid_position = true;

        // Check collision with all active entities
        for(int i = 0; i < MAX_ENTITIES; i++){
            Entity* e = &gs->entities[i];
            if(e->is_active && check_collision(&temp, e)){
                valid_position = false;
                break;
            }
        }
    }
    return (x_pos << 16) | y_pos;
}

/* Generate players — stored at entities[0..game_mode] */
void GameState_generate_players(GameState* gs, int game_mode) {
    // Player 1: Red (70 - mid-dark grey), Player 2: Blue (30 - dark grey)
    int colors[] = {70, 30, 255, 200, 100};
    int count = game_mode + 1; // game_mode=0 → 1 player, game_mode=1 → 2 players
    if (count > MAXPLAYERS) count = MAXPLAYERS;

    for (int i = 0; i < count; i++) {
        int x_pos = i * 10 + 50;
        int y_pos = i * 10 + 50;
        Entity_init_player(&gs->entities[i], colors[i], x_pos, y_pos);
    }
    gs->num_players = count;
}

/* Generate food — stored after players in the entities array */
void GameState_generate_food(GameState* gs, int gm, int diff) {
    int base_food_n = 10;
    int diff_food_mod = diff * 2;
    int gm_food_mod = (gm == 1) ? 5 : 0;
    int total_food_n = base_food_n + diff_food_mod + gm_food_mod;
    if (total_food_n > MAXFOOD) total_food_n = MAXFOOD;

    // Food type probabilities
    int prob_banana = 50 + (diff * 5);
    int prob_apple = 25 + (diff * 3);
    int prob_pear = 15 + (diff * 2);
    int prob_grape = 10 - (diff / 2);
    int total = prob_banana + prob_apple + prob_pear + prob_grape;
    prob_banana = (prob_banana * 100) / total;
    prob_apple = prob_apple * 100 / total;
    prob_pear = prob_pear * 100 / total;
    prob_grape = 100 - prob_banana - prob_apple - prob_pear;

    int base_idx = gs->num_players; // food starts after players
    for (int i = 0; i < total_food_n; i++) {
        int r = rand_range(1, 100);
        int type;
        if (r <= prob_banana) type = 0;
        else if (r <= prob_banana + prob_apple) type = 1;
        else if (r <= prob_banana + prob_apple + prob_pear) type = 2;
        else type = 3;

        int coord_key = GameState_get_random_position(gs);
        int x_pos = coord_key >> 16;
        int y_pos = coord_key & 0xFFFF;

        Entity_init_food(&gs->entities[base_idx + i], type, x_pos, y_pos);
    }
    gs->num_food = total_food_n;
}

/* Generate AI — stored after food in the entities array */
void GameState_generate_ai(GameState* gs, int diff) {
    int total_ai_n = diff * 2;
    if (total_ai_n > MAXAI) total_ai_n = MAXAI;

    int colors[] = {120, 220, 170, 70, 255, 30};
    int base_idx = gs->num_players + gs->num_food; // AI starts after food
    for (int i = 0; i < total_ai_n; i++) {
        int coord_key = GameState_get_random_position(gs);
        int x_pos = coord_key >> 16;
        int y_pos = coord_key & 0xFFFF;
        Entity_init_ai(&gs->entities[base_idx + i], colors[i], x_pos, y_pos);
    }
    gs->num_ai = total_ai_n;
}

/* Unified collision check */
bool check_collision(Entity* a, Entity* b) {
    int dx = FP_TO_INT(a->x_fp) - FP_TO_INT(b->x_fp);
    int dy = FP_TO_INT(a->y_fp) - FP_TO_INT(b->y_fp);
    int r = a->radius + b->radius;
    return (dx * dx + dy * dy) <= (r * r);
}

/* Handle eating food: eater gains nutrition, food respawns */
void handle_food_eat(GameState* gs, Entity* eater, Entity* food) {
    // Update eater
    eater->area += food->nutrition;
    eater->radius = int_sqrt(eater->area * 100 / 314);
    Entity_update_velocity(eater);

    // Respawn food at a new random position
    int coord_key = GameState_get_random_position(gs);
    food->x_fp = INT_TO_FP(coord_key >> 16);
    food->y_fp = INT_TO_FP(coord_key & 0xFFFF);
}

/* Handle entity-vs-entity eating: larger eats half of smaller's area */
void handle_entity_eat(GameState* gs, Entity* eater, Entity* eaten) {
    Entity* winner = NULL;
    Entity* loser = NULL;

    // Require winner to be larger than the loser
    if (eater->area > eaten->area) {
        winner = eater;
        loser = eaten; 
    } else if (eaten->area > eater->area) {
        winner = eaten;
        loser = eater;
    } else {
        // Not significantly larger, nothing happens
        return;
    }

    // Transfer half of loser's area to winner
    int transfer_amount = loser->area / 2;
    winner->area += transfer_amount;
    loser->area -= transfer_amount;

    // Update winner stats
    winner->radius = int_sqrt(winner->area * 100 / 314);
    Entity_update_velocity(winner);
    
    // Loser logic: lose 1 life and instantly respawn with remaining half area
    if (loser->lives > 1) {
        loser->lives -= 1;
        // Respawn with remaining area
        int coord_key = GameState_get_random_position(gs);
        loser->x_fp = INT_TO_FP(coord_key >> 16);
        loser->y_fp = INT_TO_FP(coord_key & 0xFFFF);
        loser->radius = int_sqrt(loser->area * 100 / 314);
        Entity_update_velocity(loser);
    } else {
        // Loser dies permanently (0 lives left)
        loser->lives = 0;
        loser->area = 0;
        loser->is_active = false;
    }
}

/* Update the game state. Returns true on game over. */
bool GameState_update(GameState* gs, int input_vector[]) {
    // UPDATE PLAYER POSITIONS
    for (int i = 0; i < gs->num_players; i++) {
        Entity* p = &gs->entities[i];
        if (!p->is_active) continue;
        // Read player input: player 0 uses switches 0,1; player 1 uses 2,3
        int offset = i * 2;
        int x_ctrl = input_vector[offset + 0];
        int y_ctrl = input_vector[offset + 1];
        Entity_update_position(p, gs, x_ctrl, y_ctrl);
    }

    // UPDATE AI POSITIONS (random walk)
    int ai_base = gs->num_players + gs->num_food;
    for (int i = ai_base; i < ai_base + gs->num_ai; i++) {
        Entity* ai = &gs->entities[i];
        if (!ai->is_active) continue;
        int x_ctrl = rand_range(0, 1);
        int y_ctrl = rand_range(0, 1);
        Entity_update_position(ai, gs, x_ctrl, y_ctrl);
    }

    // HANDLE COLLISIONS — single O(N²/2) loop
    for (int i = 0; i < MAX_ENTITIES; i++) {
        Entity* a = &gs->entities[i];
        if (!a->is_active) continue;

        for (int j = i + 1; j < MAX_ENTITIES; j++) {
            Entity* b = &gs->entities[j];
            if (!b->is_active) continue;

            if (!check_collision(a, b)) continue;

            // Determine collision type
            if (a->type == ENTITY_FOOD || b->type == ENTITY_FOOD) {
                // One is food — figure out who eats
                Entity* eater = (a->type != ENTITY_FOOD) ? a : b;
                Entity* food = (a->type == ENTITY_FOOD) ? a : b;
                if (eater->type == ENTITY_FOOD) continue; // food-food: skip
                handle_food_eat(gs, eater, food);
            } else {
                // Entity vs entity (player/ai)
                handle_entity_eat(gs, a, b);
            }

            // If entity 'a' died permanently in this collision, abort checking it against others
            if (!a->is_active) break;
        }
    }

    // CHECK GAME OVER
    int active_players = 0;
    for (int i = 0; i < gs->num_players; i++) {
        Entity* p = &gs->entities[i];
        if (p->is_active && p->lives > 0) {
            active_players++;
        }
    }
    
    // Game over if 0 players remain (all modes) or 1 player remains (in multiplayer)
    if (active_players == 0) return true;
    if (gs->game_mode == 1 && active_players == 1) return true; // Multiplayer win condition
    
    return false;
}