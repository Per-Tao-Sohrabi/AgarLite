#include <stdio.h>
#include <stdbool.h>
#include <math.h> // for sqrtf
#include <stddef.h>

#include "GameState.h"
#include "Entities.h"
#include "math_tools.h"


extern int rand_range(int, int);

#define MAXPLAYERS 2
#define MAXAI 0
#define MAXFOOD 5

/* Initializes the game state */
void GameState_init(volatile GameState* gs, int gm, int diff){
    print("-- Initializing GameState...\n");
    // Populate ID pool
    for(int i = 0; i< MAXPLAYERS + MAXAI + MAXFOOD; i++) {
        //print("---- Setting available_ids[%d] to true\n", i);
        gs->available_ids[i] = true; // All IDs are available at start
    }
    
    // Set game mode and difficulty
    print("-- Setting game mode to %d and difficulty to %d\n", gm, diff);
    gs->game_mode = gm;
    gs->difficulty = diff;

    // Set boundaries
    print("-- Setting game boundaries...\n");
    gs->min_x = 0;
    gs->max_x = 320;
    gs->min_y = 0;
    gs->max_y = 240;
    
    // Prepare entity lists
    print("-- Initializing players lists...\n");
    for(int i = 0; i< MAXPLAYERS; i++) {
        Player p;
        Player_init(&p, -1, -1, -1, -1); // Initialize with invalid values
        gs->players[i] = p;
    }
    print("-- Initializing food list...\n");
    for(int i = 0; i< MAXFOOD; i++) {
        Food f;
        Food_init(&f, -1, -1, -1, -1); // Initialize with invalid values
        gs->crumbs[i] = f;
    }
    print("-- Initializing AI list...\n");
    for(int i = 0; i< MAXAI; i++) {
        Ai ai;
        Ai_init(&ai, -1, -1, -1, -1); // Initialize with invalid values
        gs->ais[i] = ai;
    }
    // Generate entities
    print("-- Generating entities based on game mode and difficulty...\n");
    GameState_generate_players(gs, gm);
    GameState_generate_food(gs, gm, diff);
    GameState_generate_ai(gs, diff);
}

// Check for occupied positions, if occupied, regenerate
int GameState_get_random_position(volatile GameState* gs) {
    print("------ Generating random position...\n");
    // Set random position
    int x_pos = rand_range(gs->min_x, gs->max_x);
    int y_pos = rand_range(gs->min_y, gs->max_y);
    int coord_key = (x_pos << 16) | y_pos;
    // Check with all players
    for (int p = 0; p < MAXPLAYERS; p++) {
        // Compare with player i's position
        volatile Player* pi = &gs->players[p];
        int p_coord_key = (pi->x_pos << 16) |pi->y_pos;
        while(p_coord_key == coord_key) {
            // Redefine
            x_pos = rand_range(gs->min_x, gs->max_x);
            y_pos = rand_range(gs->min_y, gs->max_y);
            coord_key = (x_pos << 16) | y_pos;
        }  
    }
    // Check with all ai
    for (int ai = 0; ai < MAXAI; ai++) {
                // Compare with player i's position
        Ai* aii = &gs->ais[ai];
        int ai_coord_key = (aii->x_pos << 16) |aii->y_pos;
        while(ai_coord_key == coord_key) {
            // Redefine
            x_pos = rand_range(gs->min_x, gs->max_x);
            y_pos = rand_range(gs->min_y, gs->max_y);
            coord_key = (x_pos << 16) | y_pos;
        }  
    }
    // Check with all food
    for (int f = 0; f < MAXFOOD; f++) {
                // Compare with player i's position
        Ai* fi = &gs->crumbs[f];
        int f_coord_key = (fi->x_pos << 16) |fi->y_pos;
        while(f_coord_key == coord_key) {
            // Redefine
            x_pos = rand_range(gs->min_x, gs->max_x);
            y_pos = rand_range(gs->min_y, gs->max_y);
            coord_key = (x_pos << 16) | y_pos;
        }  
    }

    // while (Dict_get_key(&gs->occupied_coords_dict, (x_pos << 16) | y_pos) != -1) {
    //     x_pos = rand_range(gs->min_x, gs->max_x);
    //     y_pos = rand_range(gs->min_y, gs->max_y);
    // }

    print("------ Random position = (");
    print_dec(x_pos); // Use your print_dec function
    print(", ");
    print_dec(y_pos); // Use your print_dec function
    print(")\n");

    //coord_key = (x_pos << 16) | y_pos;
    return coord_key;
}

/* Helper function to get free ids*/
int GameState_get_free_id(volatile GameState* gs) {
    print("------ Searching for free entity ID...\n");
    for (int i = 0; i < MAXPLAYERS + MAXAI + MAXFOOD; i++) {
        if (gs->available_ids[i] == true) {
            gs->available_ids[i] = false; // Mark as used
            //print("---- Found free ID: %d\n", i);
            return i;
        }
    }
    // int id_candiate = (rand_range(0, sizeof(gs->available_ids) -1));
    // while(gs->available_ids[id_candiate] == false) {
    //     id_candiate = (rand_range(0, sizeof(gs->available_ids-1)));
    // }
    // // Mark as used
    // gs->available_ids[id_candiate] = false;
    // return id_candiate;
}

/* Generate Players, Food, and AI based on game mode and difficulty */
void GameState_generate_players(volatile GameState* gs, int game_mode) {
    print("---- Generating %d players...\n", game_mode + 1);
    volatile int colors[] = {100, 200, 150, 50, 250};
    for(int i = 0; i<=game_mode; i++) {
        // Create Player
        Player p;
        int color = colors[i];
        int id = GameState_get_free_id(gs);
        //int coord_key = GameState_get_random_position(gs);
        int x_pos = i*10 + 50;//coord_key >> 16;                       // Unpack X
        int y_pos = i*10 + 50;// coord_key & 0xFFFF;                    // Unpack Y
        //print("---- Initializing player %d with id %d at position (%d, %d) and color %d\n", i, id, x_pos, y_pos, color);
        Player_init(&p, id, color, x_pos, y_pos);
        //print("---- Player %d initialized: id=%d, pos=(%d,%d), color=%d, radius=%.2f\n", i, p.id, p.x_pos, p.y_pos, p.color, p.radius);
        // Save
        //gs->players[i] = p;
        
        // Set initial positions
        //print("---- Setting occupied position for player %d at (%d, %d)\n", i, x_pos, y_pos);
        int coord_key = (x_pos << 16) | y_pos; // Combine x and y into a single key
        Dict_insert(&gs->occupied_coords_dict, coord_key, id); // Key: combined coord, Value: food index
        Dict_insert(&gs->id_type_dict, id, 0); // Key: food index, Value: entity type (1 for food)
    }
    print("---- Player generation complete.\n");
}

/* Generate Food based on game mode and difficulty */ // TODO: Skapa Food_init().
void GameState_generate_food(volatile GameState* gs, int gm, int diff) {
    print("---- Generating food based on game mode %d and difficulty %d...\n", gm, diff);
    // Calc numb of food based on difficulty (from the 7 diff levels) and gm (2 gm). 
    int base_food_n = 10; // TODO: Rewrite 
    int diff_food_mod = diff * 2; // TODO: Rewrite,,, Each diff level adds 2 food items
    int gm_food_mod = (gm == 1) ? 5 : 0; // If 2 players, add 5 food items
    int total_food_n = base_food_n + diff_food_mod + gm_food_mod;
    if (total_food_n > MAXFOOD) {
        total_food_n = MAXFOOD; // Cap to MAXFOOD
    }
    print("---- Total food to generate: %d\n", total_food_n);
    
    // Set food type probabilities based on difficulty
    int prob_banana = 50 + (diff * 5);     // Banana: increases with difficulty
    int prob_apple = 25 + (diff * 3);      // Apple: increases with difficulty
    int prob_pear = 15 + (diff * 2);       // Pear: increases with difficulty
    int prob_grape = 10 - (diff / 2);      // Grape: decreases with difficulty (minimum 0)
    
    // Normalize to ensure probabilities sum to 100
    int total = prob_banana + prob_apple + prob_pear + prob_grape;
    prob_banana = (prob_banana * 100) / total;
    prob_apple = prob_apple * 100 / total;
    prob_pear = prob_pear * 100 / total;
    prob_grape = 100 - prob_banana - prob_apple - prob_pear;
    print("---- Generate food items ...");
    // Generate food items
    for(int i = 0; i<total_food_n; i++) {
        //print("---- Generating food item %d...\n", i);
        Food f;                             // Create food item
        
        // Generate pseudo-random number for food type
        int r = rand_range(1, 100);
        int type;
        // Determine food type based on cumulative probabilities
        if (r <= prob_banana) {
            type = 0; // banana
        } else if (r <= prob_banana + prob_apple) {
            type = 1; // apple
        } else if (r <= prob_banana + prob_apple + prob_pear) {
            type = 2; // pear
        } else {
            type = 3; // grape
        }
        // print("---- Food item %d assigned type %d (r=%d)\n", i, type, r);
        // take id from available ids, then update available ids
        int id = GameState_get_free_id(gs);        
        int coord_key = GameState_get_random_position(gs);
        int x_pos = coord_key >> 16;                       // Unpack X
        int y_pos = coord_key & 0xFFFF;                    // Unpack Y
        // print("---- Initializing food %d with id %d at position (%d, %d) and type %d\n", i, id, x_pos, y_pos, type);
        Food_init(&f, id, type, x_pos, y_pos);

        // Store food item
        gs->crumbs[i] = f;
        coord_key = (f.x_pos << 16) | f.y_pos; // Combine x and y into a single key
        Dict_insert(&gs->occupied_coords_dict, coord_key, f.id); // Key: combined coord, Value: food index
        Dict_insert(&gs->id_type_dict, f.id, 2); // Key: food index, Value: entity type (1 for food)
        // To check, get the type and then check the id in the type entity list. 
    }
    print("---- Food generation complete.\n");
}

/* Generate AI based on difficulty */
void GameState_generate_ai(volatile GameState* gs, int diff) {
    print("---- Generating AI based on difficulty %d...\n", diff);
    int total_ai_n = diff * 2; // Each diff level adds 2 AI
    if (total_ai_n > MAXAI) {
        total_ai_n = MAXAI; // Cap to MAXAI
    }
    print("---- Total AI to generate: %d\n", total_ai_n);
    volatile int colors[] = {120, 220, 170, 70, 255, 30};
    for(int i = 0; i<total_ai_n; i++) {
        // print("---- Generating AI %d...\n", i);
        Ai ai;
        ai.color = colors[i];
        int id = GameState_get_free_id(gs);
        
        // Set random position
        int coord_key = GameState_get_random_position(gs);
        int x_pos = coord_key >> 16;                       // Unpack X
        int y_pos = coord_key & 0xFFFF;                    // Unpack Y
        // print("---- Initializing AI %d with id %d at position (%d, %d) and color %d\n", i, id, x_pos, y_pos, ai.color);
        Ai_init(&ai, id, ai.color, x_pos, y_pos);
        
        // Save
        gs->ais[i] = ai;
        coord_key = (ai.x_pos << 16) | ai.y_pos; // Combine x and y into a single key
        Dict_insert(&gs->occupied_coords_dict, coord_key, ai.id); // Key:
        Dict_insert(&gs->id_type_dict, ai.id, 1); // Key: food index, Value: entity type (1 for food)
    }
    print("---- AI generation complete.\n");
}

/* Update the Game State
- Entity movements
    - Collision detection
*/
bool GameState_update(volatile GameState* gs, int input_vector[]) {
    
    // UPDATE PLAYER POSITION
    print("---- Updating player positions...\n");
    for(int i = 0; i< gs->game_mode + 1; i++) {
        // print("---- Updating player %d position...\n", i);
        volatile Player* p_i = &gs->players[i];
        if(p_i->id == -1) {
            continue; // Skip if Player does not exist
        }
        // Read player input
        int offset = i * 2;
        int x_ctrl = input_vector[offset + 0]; // X control
        int y_ctrl = input_vector[offset + 1]; // Y control
        Player_update_position(p_i, gs, x_ctrl, y_ctrl);
    }

    // UPDATE AI POSITION
    print("---- Updating AI positions...\n");
    for(int i = 0; i< MAXAI; i++) {
        // print("---- Updating AI %d position...\n", i);
        volatile Ai* ai_i = &gs->ais[i];
        if(ai_i->id == -1) {
            continue; // Skip if AI does not exist
        }
        // Simple AI movement logic: random walk
        int x_ctrl = rand_range(0, 1); // Random x control
        int y_ctrl = rand_range(0, 1); // Random y control
        AI_update_position(ai_i, gs, x_ctrl, y_ctrl); // Cast Ai to Player for position update
    }

    // HANDLE COLLISIONS BETWEEN PLAYER AND OTHER TODO...
    print("---- Checking collisions between players and other entities...\n");
    for(int i = 0; i <= gs->game_mode; i++) { // For every player
        // print("---- Checking collisions for player %d with...\n", i);
        // Fix a player
        volatile Player* p_ptr = &gs->players[i];
        if(p_ptr->id == -1) {
            continue; // Skip if Player does not exist
        }
        // Check collision with AI
        for(int j = 0; j < MAXAI; j++) {
            if(&gs->ais[j] == -1) {
                continue; // Skip if AI does not exist
            }
            volatile Ai* ai_ptr = &gs->ais[j];
            // print("------ AI %d\n", j);
            bool col = check_player_ai_collision(p_ptr, ai_ptr);
            if (col == false) { // If no collision, continue to next player. 
                // print("-------- No collision detected.\n");
                continue;
            } else {
                print("------ Collision detected at \n x: %d, y: %d\n", p_ptr->x_pos, p_ptr->y_pos);
                GameState_handle_player_ai_collision(gs, p_ptr, ai_ptr);
            }
            // Collision Logic
        }

        // Check collision with other players
        print("---- Check collision with other players");
        for(int j = i+1; j <= gs->game_mode; j++) {
            volatile Player* p2_ptr = &gs->players[j];
            if(p2_ptr->id == -1) {
                continue; // Skip if Player does not exist
            }
            bool col = check_player_player_collision(p_ptr, p2_ptr);
            if (col == false) { // If no collision, continue to next player. 
                continue;
            } else {
                print("------ Collision detected");
                GameState_handle_player_player_collision(gs, p_ptr, p2_ptr);
            }
        }

        // Check collision with food
        print("---- Checking collision with food");
        for(int j = 0; j < MAXFOOD; j++) {
            volatile Food* f_ptr = &gs->crumbs[j];
            if(f_ptr->id == -1) {
                continue; // Skip if Food does not exist
            }
            bool col = check_player_food_collision(p_ptr, f_ptr);
            if (col == false) { // If no collision, continue to next player. 
                continue;
            } else {
                print("------ Collision detected");
                GameState_handle_player_food_collision(gs, p_ptr, f_ptr);
            }
            // Collision Logic
            //Player_update_hp(food->nutrition);                       // If collision, do something
            // Move the food to new position
            //Food_new_position(food);
        }
        
    }

    // HANLDE COLLISIONS BETWEEN AI AND OTHER TODO...
    for(int i = 0; i < MAXAI; i++) {
            volatile Ai* ai_ptr = &gs->ais[i];
            if(ai_ptr->id == -1) {
                continue; // Skip if AI does not exist
            }
            // Check collision with other AI
            for(int j = i+1; j < MAXAI; j++) {
                volatile Ai* ai2_ptr = &gs->ais[j];
                if(ai2_ptr->id == -1) {
                    continue; // Skip if AI does not exist
                }
                bool col = check_ai_ai_collision(ai_ptr, ai2_ptr);
                if (col == false) { // If no collision, continue to next AI. 
                    continue;
                } else {
                    GameState_handle_ai_ai_collision(gs, ai_ptr, ai2_ptr);
                }
            }

            // Check collision with food
            for(int j = 0; j < MAXFOOD; j++) {
                volatile Food* f_ptr = &gs->crumbs[j];
                if(f_ptr->id == -1) {
                    continue; // Skip if Food does not exist
                }

                bool col = check_ai_food_collision(ai_ptr, f_ptr);
                if (col == false) { // If no collision, continue to next AI. 
                    continue;
                } else {
                    GameState_handle_ai_food_collision(gs, ai_ptr, f_ptr);
                }
                // Collision Logic
            }

        }
    
    // Check game over conditions TODO...
    print("---- Checking Game Over conditions");
    for(int i = 0; i <= gs->game_mode; i++) {
        volatile Player* p_i = &gs->players[i];
        if(p_i->id == -1) {
            continue; // Skip if Player does not exist
        }
        if (p_i->area <= 0) {
            // Handle game over for player i
            return true;
        }
    }
    return false; 
}

// CHECK COLLISIONS BETWEEN ENTITIES
bool check_player_food_collision(volatile Player* p, volatile Food* f) {
    int dx = p-> x_pos - f-> x_pos;
    int dy = p-> y_pos - f-> y_pos;
    int distance_squared = dx*dx + dy*dy;
    int radius_sum = p->radius + f->radius;
    return distance_squared <= radius_sum*radius_sum;
}

bool check_player_player_collision(volatile Player* p1, volatile Player* p2) {
    int dx = p1-> x_pos - p2-> x_pos;
    int dy = p1-> y_pos - p2-> y_pos;
    int distance_squared = dx*dx + dy*dy;
    int radius_sum = p1->radius + p2->radius;
    return distance_squared <= radius_sum*radius_sum;
}

bool check_player_ai_collision(volatile Player* p, volatile Ai* ai) {
    int dx = p-> x_pos - ai-> x_pos;
    int dy = p-> y_pos - ai-> y_pos;
    int distance_squared = dx*dx + dy*dy;
    int radius_sum = p->radius + ai->radius;
    return distance_squared <= radius_sum*radius_sum;
}

bool check_ai_ai_collision(volatile Ai* ai1, volatile Ai* ai2) {
    int dx = ai1-> x_pos - ai2-> x_pos;
    int dy = ai1-> y_pos - ai2-> y_pos;
    int distance_squared = dx*dx + dy*dy;
    int radius_sum = ai1->radius + ai2->radius;
    return distance_squared <= radius_sum*radius_sum;
}

bool check_ai_food_collision(volatile Ai* ai, volatile Food* f) {
    int dx = ai-> x_pos - f-> x_pos;
    int dy = ai-> y_pos - f-> y_pos;
    int distance_squared = dx*dx + dy*dy;
    int radius_sum = ai->radius + f->radius;
    return distance_squared <= radius_sum*radius_sum;
}


// HANDLE COLLISION RESPONSES
/* Handle player ai collisions*/
void GameState_handle_player_ai_collision(volatile GameState* gs, volatile Player* p, volatile Ai* ai) {
    if (p->area > ai->area) {
        print("-------- Player eats AI \n", p->id, ai->id);
        // p eats ai
        float old_area_ai = ai->area;
        float new_area_ai = (ai->area * (1.0f - 0.5f)); // Reduce ai area by 50%
        float delta_area_ai = old_area_ai - new_area_ai;

        // Update areas and radii
        ai->area = new_area_ai;
        p->area += delta_area_ai; // Increase p area
        ai->radius = sqrtf(ai->area / 3.14f); // Update AI radius
        p->radius = sqrtf(p->area / 3.14f); // Update player radius
        Player_update_velocity(p);
        // print("Updated Player %d velocity to %d\n", p->id, p->velocity);
        AI_update_velocity(ai);
        // print("Updated AI %d velocity to %d\n", ai->id, ai->velocity);
        
        // Update occupied coords dictionary in GameState
        int coord_key_ai = (ai->x_pos << 16) | ai->y_pos;  // Combine x and y into a single key
        Dict_set_value(&gs->occupied_coords_dict, coord_key_ai, ai->id);   
        
        int coord_key_p = (p->x_pos << 16) | p->y_pos;  // Combine x and y into a single key
        Dict_set_value(&gs->occupied_coords_dict, coord_key_p, p->id);
    } else if (ai->area > p->area) {
        print("-------- Ai eats Player \n", p->id, ai->id);
        // AI eats Player
        float a_ratio = ai->area / p->area;
        
        // ai eats p
        float old_area_p = p->area;
        float new_area_p = (p->area * (1.0f - a_ratio * 0.5f)); // Reduce p2 area
        float delta_area_p = old_area_p - new_area_p;

        // Update areas and radii
        ai->area = new_area_p;
        ai->area += delta_area_p; // Increase p1 area
        p->radius =  sqrtf(p->area/3.14f); // Update player radius
        ai->radius =  sqrtf(ai->area/3.14f); // Update player radius
        Player_update_velocity(p);
        AI_update_velocity(ai);
        // Update occupied coords dictionary in GameState
        int coord_key_p = (p->x_pos << 16) | p->y_pos;  // Combine x and y into a single key
        Dict_set_value(&gs->occupied_coords_dict, coord_key_p, p->id);   
        
        int coord_key_ai = (ai->x_pos << 16) | ai->y_pos;  // Combine x and y into a single key
        Dict_set_value(&gs->occupied_coords_dict, coord_key_ai, ai->id);
    } else {
        print("--------  Equal size");
        // Equal area, no one eats
        int coord_key_ai = (ai->x_pos << 16) | ai->y_pos;  // Combine x and y into a single key
        Dict_set_value(&gs->occupied_coords_dict, coord_key_ai, ai->id);   
        
        int coord_key_p = (p->x_pos << 16) | p->y_pos;  // Combine x and y into a single key
        Dict_set_value(&gs->occupied_coords_dict, coord_key_p, p->id);
        return;
    }
}
/* Handle player player collisions*/
void GameState_handle_player_player_collision(volatile GameState* gs, volatile Player* p1, volatile Player* p2) {
    volatile Player* pi; // Player with larger area, holds address
    volatile Player* pj; // Player with smaller area, holds address
    if (p1->area > p2->area) {
        print("-------- P1 eats P2");
        pi = p1;
        pj = p2;
    } else if (p2->area > p1->area) {
        print("-------- P2 eats P1");
        pi = p2;
        pj = p1;
    }   else {
        print("-------- No conflict");
        // Equal area, no one eats
            // Update occupied coords dictionary in GameState
            int coord_key_pj = (pj->x_pos << 16) | pj->y_pos;  // Combine x and y into a single key
            Dict_set_value(&gs->occupied_coords_dict, coord_key_pj, pj->id);   
            
            int coord_key_pi = (pi->x_pos << 16) | pi->y_pos;  // Combine x and y into a single key
            Dict_set_value(&gs->occupied_coords_dict, coord_key_pi, pi->id);
        return;
    }
    float a_ratio = pj->area / pi->area;
    
    // p1 eats p2
    float old_area_pj = pj->area;
    float new_area_pj = (pj->area * (1.0f - a_ratio * 0.5f)); // Reduce p2 area
    float delta_area_pj = old_area_pj - new_area_pj;

    // Update areas, radii, and velocities
    pi->area = new_area_pj;
    pi->area += delta_area_pj; // Increase p1 area
    pj->radius =  sqrtf(pj->area/3.14f); // Update player radius
    pi->radius =  sqrtf(pi->area/3.14f); // Update player radius
    Player_update_velocity(pi);
    Player_update_velocity(pj);

    // Update occupied coords dictionary in GameState
    int coord_key_pj = (pj->x_pos << 16) | pj->y_pos;  // Combine x and y into a single key
    Dict_set_value(&gs->occupied_coords_dict, coord_key_pj, pj->id);   
    int coord_key_pi = (pi->x_pos << 16) | pi->y_pos;  // Combine x and y into a single key
    Dict_set_value(&gs->occupied_coords_dict, coord_key_pi, pi->id);
}
/* Handle player food collision*/   
void GameState_handle_player_food_collision(volatile GameState* gs, volatile Player* p, volatile Food* f) {
    // Update player area based on nutrient
    // Compute current area:
    print("-------- Player eats");
    float area = p->radius * p->radius * 3.14f; 
    float new_area = area + f->nutrition;
    float new_r = sqrtf(new_area/3.14f);

    p->area = new_area; // Update player area
    p->radius =  new_r; // Update player radius
    Player_update_velocity(p);
    
    // Update food position. 
    Dict_set_value(&gs->occupied_coords_dict, (f->x_pos << 16) | f->y_pos, -1);  // Reset old position in occupied coords dict
    int coord_key = GameState_get_random_position(gs);
    int x_pos = coord_key >> 16;                       // Unpack X
    int y_pos = coord_key & 0xFFFF;                    // Unpack Y
    f-> x_pos = x_pos;
    f-> y_pos = y_pos;
    // Update occupied coords dictionary in GameState
    
    int coord_key_f = (f->x_pos << 16) | f->y_pos;  // Combine x and y into a single key
    Dict_set_value(&gs->occupied_coords_dict, coord_key_f, f->id);   
    int coord_key_p = (p->x_pos << 16) | p->y_pos;  // Combine x and y into a single key
    Dict_set_value(&gs->occupied_coords_dict, coord_key_p, p->id);
}
/* Handle ai ai collisions*/
void GameState_handle_ai_ai_collision(volatile GameState* gs, volatile Ai* ai1, volatile Ai* ai2) {
    volatile Ai* ai; // Player with larger area, holds address
    volatile Ai* aj; // Player with smaller area, holds address
    if (ai1->area > ai2->area) {
        ai = ai1;
        aj = ai2;
    } else if (ai2->area > ai1->area) {
        ai = ai2;
        aj = ai1;
    }   else {
        // Equal area, no one eats
            // Update occupied coords dictionary in GameState
            int coord_key_aj = (aj->x_pos << 16) | aj->y_pos;  // Combine x and y into a single key
            Dict_set_value(&gs->occupied_coords_dict, coord_key_aj, aj->id);   
            
            int coord_key_ai = (ai->x_pos << 16) | ai->y_pos;  // Combine x and y into a single key
            Dict_set_value(&gs->occupied_coords_dict, coord_key_ai, ai->id);
        return;
    }
    float a_ratio = aj->area / ai->area;
    
    // p1 eats p2
    float old_area_aj = aj->area;
    float new_area_aj = (aj->area * (1.0f - a_ratio * 0.5f)); // Reduce p2 area
    float delta_area_aj = old_area_aj - new_area_aj;

    // Update areas and radii
    ai->area = new_area_aj;
    ai->area += delta_area_aj; // Increase p1 area
    aj->radius =  sqrtf(aj->area/3.14f); // Update player radius
    ai->radius =  sqrtf(ai->area/3.14f); // Update player radius    
    AI_update_velocity(aj);
    AI_update_velocity(ai);

    // Update occupied coords dictionary in GameState
    int coord_key_aj = (aj->x_pos << 16) | aj->y_pos;  // Combine x and y into a single key
    Dict_set_value(&gs->occupied_coords_dict, coord_key_aj, aj->id);   
    int coord_key_ai = (ai->x_pos << 16) | ai->y_pos;  // Combine x and y into a single key
    Dict_set_value(&gs->occupied_coords_dict, coord_key_ai, ai->id);
}
/* Handle ai food collisions*/
void GameState_handle_ai_food_collision(volatile GameState* gs, volatile Ai* ai, volatile Food* f) {
    // Update AI area based on nutrient
    
    // Update player area based on nutrient, radius, and velocity
    float area = ai->radius * ai->radius * 3.14f; 
    float new_area = area + f-> nutrition;
    float new_r = sqrtf(new_area/3.14f);
    ai->radius = new_r; // Update AI radius
    AI_update_velocity(ai);

    // Update food position. 
    Dict_set_value(&gs->occupied_coords_dict, (f->x_pos << 16) | f->y_pos, -1);  // Reset old position in occupied coords dict
    int coord_key = GameState_get_random_position(gs);
    int x_pos = coord_key >> 16;                       // Unpack X
    int y_pos = coord_key & 0xFFFF;                    // Unpack Y
    f-> x_pos = x_pos;
    f-> y_pos = y_pos;
    
    // Update occupied coords dictionary in GameState
    int coord_key_f = (f->x_pos << 16) | f->y_pos;  // Combine x and y into a single key
    Dict_set_value(&gs->occupied_coords_dict, coord_key_f, f->id);   
    int coord_key_ai = (ai->x_pos << 16) | ai->y_pos;  // Combine x and y into a single key
    Dict_set_value(&gs->occupied_coords_dict, coord_key_ai, ai->id);
}