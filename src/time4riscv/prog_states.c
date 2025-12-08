#include <stdio.h>
#include "prog_states.h"
#include "GameState.h"
#include "inputs.h"
#include "render.h"

// Initalize a game state
GameState run_start_up() {
    // Query game mode and difficulty
    int gm = query_game_mode(); // Sets number of players and diffuculty in gm varibale. 
    int diff = query_game_difficulty();
    // Initiate the game state:
    print("Starting GameState initalization Sequence with mode %d and difficulty %d\n");
    GameState gs;
    GameState_init(&gs, gm, diff);
    return  gs;
} 

/* Query the player about the game mode they want to play at*/
int query_game_mode() {
    char* msg = "Select Game Mode: 1 or 2 Players by toggling the first switch up for single player. \nSwitch up down for multiplayer. Press button to confirm.\n";
    
    print(msg); // render function
    // Wait for user input and return selected mode
    while(1) {
        int mode = get_switch_state(0);      // Read the mode input
        if(get_btn() == 1) {                 // Poll the button
            return mode;                     // Return set mode
        }
    }
    return 1; // Placeholder return value
}

/* Query the player about the difficulty they want to play at*/
int query_game_difficulty() {
    char* msg = "Use the three first switches to set your difficulty. \nNote binary numbers! Press button to confirm.\n";
    // draw_string_wrapped(35, 60, msg, WHITE, MSG_WIDTH);
    print(msg); 
    // render function

    // Wait for user input and return selected mode
    while(1) {
        int sw1 = get_switch_state(0);
        int sw2 = get_switch_state(1);
        int sw3 = get_switch_state(2);
        volatile int diff = sw1 + sw2 + sw3; // Binary to decimal conversion
        
        int btn = get_btn();
        print("Selected difficulty: d%d\n", diff);
        if (btn) {
            switch (diff)
            {
            case 0:
                return 0;
                break;
            case 1:
                return 1;
                break;
            case 2:
                return 2;
                break;
            case 3:
                return 3;
                break;
            case 4:
                return 4;
                break;
            case 5:
                return 5;
                break;
            case 6: 
                return 6;
                break;
            case 7:
                return 7;
                break;
            default:
                break;
            }
        }
    }
    return 1; // Placeholder return value
}
 
/* Pause logic*/
void run_pause() {
    print("Toggle switch 4 down to exit pause");
    // char* msg = "Toggle switch 4 down to exit pause";

    while(1) {
        int status = get_switch_state(4);
        if(status == 0) {
            clear_current_buffer();
            swap_buffers();
            break;
        }
    }
}

/* Game Over*/
void run_game_over() {
    print("Game Over! Press button to restart.");
    // char* msg = "Game Over! \nPress button to restart.";
    // draw_string_wrapped(35, 60, msg, WHITE, MSG_WIDTH);
    
    while(1) {
        if(get_btn() == 1) {
            clear_current_buffer();
            swap_buffers();
            break;
        }
    }
}