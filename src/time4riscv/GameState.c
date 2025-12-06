
#include "GameState.h"
#include "Entities.h"
#include <stdbool.h>
#include <math.h> // for sqrt
#include <stddef.h>
#include "math_tools.h"


extern int rand_range(int, int);

#define MAXPLAYERS 2
#define MAXAI 6
#define MAXFOOD 20

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
    GameState_generate_players(gs, gm);
    GameState_generate_food(gs, gm, diff);
    GameState_generate_ai(gs, diff);
}

// Check for occupied positions, if occupied, regenerate
int GameState_get_random_position(GameState* gs) {
    // Set random position
    int x_pos = rand_range(gs->min_x, gs->max_x);
    int y_pos = rand_range(gs->min_y, gs->max_y);
    while (Dict_get_key(&gs->occupied_coords_dict, (x_pos << 16) | y_pos) != -1) {
        x_pos = rand_range(gs->min_x, gs->max_x);
        y_pos = rand_range(gs->min_y, gs->max_y);
    }
    int coord_key = (x_pos << 16) | y_pos;
    return coord_key;
}

/* Helper function to get free ids*/
int GameState_get_free_id(GameState* gs) {
    for (int i = 0; i < sizeof(gs->available_ids); i++) {
        if (gs->available_ids[i] == true) {
            gs->available_ids[i] = false; // Mark as used
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
void GameState_generate_players(GameState* gs, int game_mode) {
    volatile int colors[] = {100, 200, 150, 50, 250};
    for(int i = 0; i<=game_mode; i++) {
        // Create Player
        Player p;
        int color = colors[i];
        int id = GameState_get_free_id(gs);
        int coord_key = GameState_get_random_position(gs);
        int x_pos = coord_key >> 16;                       // Unpack X
        int y_pos = coord_key & 0xFFFF;                    // Unpack Y
        Player_init(&p, id, color, x_pos, y_pos);
        
        // Save
        gs->players[i] = p;
        
        // Set initial positions
        int coord_key = (p.x_pos << 16) | p.y_pos; // Combine x and y into a single key
        Dict_insert(&gs->occupied_coords_dict, coord_key, p.id); // Key: combined coord, Value: food index
        Dict_insert(&gs->id_type_dict, p.id, 0); // Key: food index, Value: entity type (1 for food)
    }
}

/* Generate Food based on game mode and difficulty */ // TODO: Skapa Food_init().
void GameState_generate_food(GameState* gs, int gm, int diff) {
    
    // Calc numb of food based on difficulty (from the 7 diff levels) and gm (2 gm). 
    int base_food_n = 10; // TODO: Rewrite 
    int diff_food_mod = diff * 2; // TODO: Rewrite,,, Each diff level adds 2 food items
    int gm_food_mod = (gm == 1) ? 5 : 0; // If 2 players, add 5 food items
    int total_food_n = base_food_n + diff_food_mod + gm_food_mod;
    if (total_food_n > MAXFOOD) {
        total_food_n = MAXFOOD; // Cap to MAXFOOD
    }
    
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
    
    // Generate food items
    for(int i = 0; i<total_food_n; i++) {
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
        
        // take id from available ids, then update available ids
        int id = GameState_get_free_id(gs);        
        int coord_key = GameState_get_random_position(gs);
        int x_pos = coord_key >> 16;                       // Unpack X
        int y_pos = coord_key & 0xFFFF;                    // Unpack Y
        Food_init(&f, id, type, x_pos, y_pos);

        // Store food item
        gs->crumbs[i] = f;
        int coord_key = (f.x_pos << 16) | f.y_pos; // Combine x and y into a single key
        Dict_insert(&gs->occupied_coords_dict, coord_key, f.id); // Key: combined coord, Value: food index
        Dict_insert(&gs->id_type_dict, f.id, 2); // Key: food index, Value: entity type (1 for food)
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
        int id = GameState_get_free_id(gs);
        
        // Set random position
        int coord_key = GameState_get_random_position(gs);
        int x_pos = coord_key >> 16;                       // Unpack X
        int y_pos = coord_key & 0xFFFF;                    // Unpack Y

        Ai_init(&ai, id, ai.color, x_pos, y_pos);
        
        // Save
        gs->ais[i] = ai;
        int coord_key = (ai.x_pos << 16) | ai.y_pos; // Combine x and y into a single key
        Dict_insert(&gs->occupied_coords_dict, coord_key, ai.id); // Key:
        Dict_insert(&gs->id_type_dict, ai.id, 1); // Key: food index, Value: entity type (1 for food)
    }
}

/* Update the Game State
- Entity movements
    - Collision detection
*/
bool GameState_update(GameState* gs, int input_vector[]) {
    // UPDATE PLAYER POSITION
    for(int i = 0; i< sizeof(gs->players); i++) {
        Player* p_i = &gs->players[i];
        
        // Read player input
        int x_ctrl = input_vector[i]; // X control
        int y_ctrl = input_vector[i+1]; // Y control
        Player_update_position(p_i, gs, x_ctrl, y_ctrl);
    }

    // UPDATE AI POSITION
    for(int i = 0; i< sizeof(gs->ais); i++) {
        Ai* ai_i = &gs->ais[i];
        // Simple AI movement logic: random walk
        int x_ctrl = rand_range(0, 1); // Random x control
        int y_ctrl = rand_range(0, 1); // Random y control
        AI_update_position(ai_i, gs, x_ctrl, y_ctrl); // Cast Ai to Player for position update
    }

    // HANDLE COLLISIONS BETWEEN PLAYER AND OTHER TODO...
    for(int i = 0; i <= gs->game_mode; i++) { // For every player
        
        // Fix a player
        Player* p_ptr = &gs->players[i];

        // Check collision with AI
        for(int j = 0; j < MAXAI; j++) {
            if(&gs->ais[j] == NULL) {
                continue; // Skip if AI does not exist
            }
            Ai* ai_ptr = &gs->ais[j];
            bool col = check_player_ai_colision(p_ptr, ai_ptr);
            if (col == false) { // If no collision, continue to next player. 
                continue;
            } else {
                GameState_handle_player_ai_collision(gs, p_ptr, ai_ptr);
            }
            // Collision Logic
        }

        // Check collision with other players
        for(int j = i+1; j <= gs->game_mode; j++) {
            Player* p2_ptr = &gs->players[j];
            bool col = check_player_player_colision(p_ptr, p2_ptr);
            if (col == false) { // If no collision, continue to next player. 
                continue;
            } else {
                GameState_handle_player_player_collision(gs, p_ptr, p2_ptr);
            }
        }

        // Check collision with food
        for(int j = 0; j < MAXFOOD; j++) {
            Food* f_ptr = &gs->crumbs[j];
            bool col = check_player_food_colision(p_ptr, f_ptr);
            if (col == false) { // If no collision, continue to next player. 
                continue;
            } else {
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
            Ai* ai_ptr = &gs->ais[i];
            if(ai_ptr == NULL) {
                continue; // Skip if AI does not exist
            }

            // Check collision with other AI
            for(int j = i+1; j < MAXAI; j++) {
                Ai* ai2_ptr = &gs->ais[j];
                if(ai2_ptr == NULL) {
                    continue; // Skip if AI does not exist
                }
                bool col = check_ai_ai_colision(ai_ptr, ai2_ptr);
                if (col == false) { // If no collision, continue to next AI. 
                    continue;
                } else {
                    GameState_handle_ai_ai_colision(gs, ai_ptr, ai2_ptr);
                }
            }

            // Check collision with food
            for(int j = 0; j < MAXFOOD; j++) {
                Food* f_ptr = &gs->crumbs[j];
                bool col = check_ai_food_colision(ai_ptr, f_ptr);
                if (col == false) { // If no collision, continue to next AI. 
                    continue;
                } else {
                    GameState_handle_ai_food_collision(gs, ai_ptr, f_ptr);
                }
                // Collision Logic
            }

        }
    
    // Check game over conditions TODO...
    for(int i = 0; i <= gs->game_mode; i++) {
        Player* p_i = &gs->players[i];
        if (p_i->area <= 0) {
            // Handle game over for player i
            return true;
        }
    }
    return false; 
}

// CHECK COLLISIONS BETWEEN ENTITIES
bool check_player_food_collision(Player* p, Food* f) {
    int dx = p-> x_pos - f-> x_pos;
    int dy = p-> y_pos - f-> y_pos;
    int distance_squared = dx*dx + dy*dy;
    int radius_sum = p->radius + f->radius;
    return distance_squared <= radius_sum*radius_sum;
}

bool check_player_ai_collision(Player* p, Ai* ai) {
    int dx = p-> x_pos - ai-> x_pos;
    int dy = p-> y_pos - ai-> y_pos;
    int distance_squared = dx*dx + dy*dy;
    int radius_sum = p->radius + ai->radius;
    return distance_squared <= radius_sum*radius_sum;
}

bool check_ai_ai_collision(Ai* ai1, Ai* ai2) {
    int dx = ai1-> x_pos - ai2-> x_pos;
    int dy = ai1-> y_pos - ai2-> y_pos;
    int distance_squared = dx*dx + dy*dy;
    int radius_sum = ai1->radius + ai2->radius;
    return distance_squared <= radius_sum*radius_sum;
}

bool check_ai_food_collision(Ai* ai, Food* f) {
    int dx = ai-> x_pos - f-> x_pos;
    int dy = ai-> y_pos - f-> y_pos;
    int distance_squared = dx*dx + dy*dy;
    int radius_sum = ai->radius + f->radius;
    return distance_squared <= radius_sum*radius_sum;
}

bool check_player_player_collision(Player* p1, Player* p2) {
    int dx = p1-> x_pos - p2-> x_pos;
    int dy = p1-> y_pos - p2-> y_pos;
    int distance_squared = dx*dx + dy*dy;
    int radius_sum = p1->radius + p2->radius;
    return distance_squared <= radius_sum*radius_sum;
}

bool check_player_ai_collision(Food* f1, Food* f2) {
    int dx = f1-> x_pos - f2-> x_pos;
    int dy = f1-> y_pos - f2-> y_pos;
    int distance_squared = dx*dx + dy*dy;
    int radius_sum = f1->radius + f2->radius;
    return distance_squared <= radius_sum*radius_sum;
}

// HANDLE COLLISION RESPONSES
/* Handle player ai collisions*/
void GameState_handle_player_ai_collision(GameState* gs, Player* p, Ai* ai) {
    if (p->area > ai->area) {
    
        double a_ratio = (double)p->area / (double)p->area;
        
        // p eats ai
        int old_area_ai = p->area;
        int new_area_ai = (int)(p->area * (1.0 - a_ratio * 0.5)); // Reduce p2 area
        int delta_area_ai = old_area_ai - new_area_ai;

        // Update areas and radii
        p->area = new_area_ai;
        p->area += delta_area_ai; // Increase p1 area
        ai->radius = (int) sqrt(ai->area/3.14); // Update player radius
        p->radius = (int) sqrt(ai->area/3.14); // Update player radius
        Player_update_velocity(p);
        AI_update_velocity(ai);
        // Update occupied coords dictionary in GameState
        int coord_key_ai = (ai->x_pos << 16) | ai->y_pos;  // Combine x and y into a single key
        Dict_set_value(&gs->occupied_coords_dict, coord_key_ai, ai->id);   
        
        int coord_key_p = (p->x_pos << 16) | p->y_pos;  // Combine x and y into a single key
        Dict_set_value(&gs->occupied_coords_dict, coord_key_p, p->id);
    } else if (ai->area > p->area) {
        // AI eats Player
        double a_ratio = (double)ai->area / (double)p->area;
        
        // ai eats p
        int old_area_p = p->area;
        int new_area_p = (int)(p->area * (1.0 - a_ratio * 0.5)); // Reduce p2 area
        int delta_area_p = old_area_p - new_area_p;

        // Update areas and radii
        ai->area = new_area_p;
        ai->area += delta_area_p; // Increase p1 area
        p->radius = (int) sqrt(p->area/3.14); // Update player radius
        ai->radius = (int) sqrt(ai->area/3.14); // Update player radius
        Player_update_velocity(p);
        AI_update_velocity(ai);
        // Update occupied coords dictionary in GameState
        int coord_key_p = (p->x_pos << 16) | p->y_pos;  // Combine x and y into a single key
        Dict_set_value(&gs->occupied_coords_dict, coord_key_p, p->id);   
        
        int coord_key_ai = (ai->x_pos << 16) | ai->y_pos;  // Combine x and y into a single key
        Dict_set_value(&gs->occupied_coords_dict, coord_key_ai, ai->id);
    } else {
        // Equal area, no one eats
        int coord_key_ai = (ai->x_pos << 16) | ai->y_pos;  // Combine x and y into a single key
        Dict_set_value(&gs->occupied_coords_dict, coord_key_ai, ai->id);   
        
        int coord_key_p = (p->x_pos << 16) | p->y_pos;  // Combine x and y into a single key
        Dict_set_value(&gs->occupied_coords_dict, coord_key_p, p->id);
        return;
    }
}
/* Handle player player collisions*/
void GameState_handle_player_player_collision(GameState* gs, Player* p1, Player* p2) {
    Player *pi; // Player with larger area, holds address
    Player *pj; // Player with smaller area, holds address
    if (p1->area > p2->area) {
        pi = p1;
        pj = p2;
    } else if (p2->area > p1->area) {
        pi = p2;
        pj = p1;
    }   else {
        // Equal area, no one eats
            // Update occupied coords dictionary in GameState
            int coord_key_pj = (pj->x_pos << 16) | pj->y_pos;  // Combine x and y into a single key
            Dict_set_value(&gs->occupied_coords_dict, coord_key_pj, pj->id);   
            
            int coord_key_pi = (pi->x_pos << 16) | pi->y_pos;  // Combine x and y into a single key
            Dict_set_value(&gs->occupied_coords_dict, coord_key_pi, pi->id);
        return;
    }
    double a_ratio = (double)pj->area / (double)pi->area;
    
    // p1 eats p2
    int old_area_pj = pj->area;
    int new_area_pj = (int)(pj->area * (1.0 - a_ratio * 0.5)); // Reduce p2 area
    int delta_area_pj = old_area_pj - new_area_pj;

    // Update areas, radii, and velocities
    pi->area = new_area_pj;
    pi->area += delta_area_pj; // Increase p1 area
    pj->radius = (int) sqrt(pj->area/3.14); // Update player radius
    pi->radius = (int) sqrt(pi->area/3.14); // Update player radius
    Player_update_velocity(pi);
    Player_update_velocity(pj);

    // Update occupied coords dictionary in GameState
    int coord_key_pj = (pj->x_pos << 16) | pj->y_pos;  // Combine x and y into a single key
    Dict_set_value(&gs->occupied_coords_dict, coord_key_pj, pj->id);   
    int coord_key_pi = (pi->x_pos << 16) | pi->y_pos;  // Combine x and y into a single key
    Dict_set_value(&gs->occupied_coords_dict, coord_key_pi, pi->id);
}
/* Handle player food collision*/   
void GameState_handle_player_food_coolision(GameState* gs, Player* p, Food* f) {
    // Update player area based on nutrient
    // Compute current area:
    double area = (double)p->radius * (double)p->radius * 3.14; 
    double new_area = area + f->nutrition;
    double new_r = sqrt(new_area/3.14);

    p->area = (int)new_area; // Update player area
    p->radius = (int) new_r; // Update player radius
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
    int coord_key_p = (f->x_pos << 16) | f->y_pos;  // Combine x and y into a single key
    Dict_set_value(&gs->occupied_coords_dict, coord_key_p, p->id);
}
/* Handle ai ai collisions*/
void GameState_handle_ai_ai_colision(GameState* gs, Ai* ai1, Ai* ai2) {
    Ai *ai; // Player with larger area, holds address
    Ai *aj; // Player with smaller area, holds address
    if (ai1->area > ai2->area) {
        ai = ai1;
        aj = ai2;
    } else if (ai1->area > ai1->area) {
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
    double a_ratio = (double)aj->area / (double)ai->area;
    
    // p1 eats p2
    int old_area_aj = aj->area;
    int new_area_aj = (int)(aj->area * (1.0 - a_ratio * 0.5)); // Reduce p2 area
    int delta_area_aj = old_area_aj - new_area_aj;

    // Update areas and radii
    ai->area = new_area_aj;
    ai->area += delta_area_aj; // Increase p1 area
    aj->radius = (int) sqrt(aj->area/3.14); // Update player radius
    ai->radius = (int) sqrt(ai->area/3.14); // Update player radius    
    AI_update_velocity(aj);
    AI_update_velocity(ai);

    // Update occupied coords dictionary in GameState
    int coord_key_aj = (aj->x_pos << 16) | aj->y_pos;  // Combine x and y into a single key
    Dict_set_value(&gs->occupied_coords_dict, coord_key_aj, aj->id);   
    int coord_key_ai = (ai->x_pos << 16) | ai->y_pos;  // Combine x and y into a single key
    Dict_set_value(&gs->occupied_coords_dict, coord_key_ai, ai->id);
}
/* Handle ai food collisions*/
void GameState_handle_ai_food_collision(GameState* gs, Ai* ai, Food* f) {
    // Update AI area based on nutrient
    
    // Update player area based on nutrient, radius, and velocity
    double area = (double)ai->radius * (double)ai->radius * 3.14; 
    double new_area = area + f-> nutrition;
    double new_r = sqrt(new_area/3.14);
    ai->radius = (int)new_r; // Update AI radius
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
    int coord_key_ai = (f->x_pos << 16) | f->y_pos;  // Combine x and y into a single key
    Dict_set_value(&gs->occupied_coords_dict, coord_key_ai, ai->id);
}