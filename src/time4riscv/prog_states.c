#include <stdio.h>
#include "prog_states.h"
#include "GameState.h"
#include "inputs.h"
#include "render.h"

extern void clear_current_buffer();
extern void draw_string_wrapped(int x, int y, const char *str, int color, int max_width);
extern void draw_msg(char* ch);
extern void swap_buffers();
extern void init_buffers();
extern void clear_screen();



// Initalize a game state
GameState run_start_up() {
    // Query game mode and difficulty
    int gm = query_game_mode(); // Sets number of players and diffuculty in gm varibale. 
    int diff = query_game_difficulty();
    // Initiate the game state:
    //print("Starting GameState initalization Sequence with mode %d and difficulty %d\n", gm, diff);
    GameState gs;
    GameState_init(&gs, gm, diff);
    return  gs;
} 

/* Query the player about the game mode they want to play at*/
int query_game_mode() {
    // clear_current_buffer(); 
    char* msg = "Select Game Mode: 1 or 2 Players by toggling the first switch up for single player. \nSwitch up down for multiplayer. Press button to confirm.\n";
    // draw_msg(msg);
    
    //print(msg); // render function
    // Wait for user input and return selected mode
    while(1) {
        int mode = get_switch_state(0);      // Read the mode input
        if(get_btn() == 1) {                 // Poll the button
            clear_current_buffer();
            swap_buffers();
            return mode;                     // Return set mode
        }
    }
    return 1; // Placeholder return value
}

/* Query the player about the difficulty they want to play at*/
int query_game_difficulty() {
    // clear_current_buffer();
    char* msg = "Use the three first switches to set your difficulty. \nNote binary numbers! Press button to confirm.\n";
    // draw_string_wrapped(35, 60, msg, WHITE, MSG_WIDTH);
    // draw_msg(msg);
    //print(msg); // render function
    
    // Wait for user input and return selected mode
    while(1) {
        int sw1 = get_switch_state(0);
        int sw2 = get_switch_state(1);
        int sw3 = get_switch_state(2);
        volatile int diff = sw1 + sw2 + sw3; // Binary to decimal conversion
        
        int btn = get_btn();
        //print("Selected difficulty: d%d\n", diff);
        if (btn) {
            // clear_current_buffer();
            // swap_buffers();
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
    // clear_current_buffer();
    //print("Toggle switch 4 down to exit pause");
    char* msg = "Toggle switch 4 down to exit pause";
    // draw_string_wrapped(35, 60, msg, WHITE, MSG_WIDTH);
    // draw_msg(msg);
    while(1) {
        int status = get_switch_state(4);
        if(status == 0) {
            // clear_current_buffer();
            // swap_buffers();
            break;
        }
    }
}

/* Game Over*/
void run_game_over() {
    // clear_current_buffer();
    //print("Game Over! Press button to restart.");
    char* msg = "Game Over! \nPress button to restart.";
    // draw_string_wrapped(35, 60, msg, WHITE, MSG_WIDTH);
    // draw_msg(msg);

    while(1) {
        if(get_btn() == 1) {
            // clear_current_buffer();
            // swap_buffers();
            break;
        }
    }
}