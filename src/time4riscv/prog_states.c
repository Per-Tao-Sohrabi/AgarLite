#include <stdio.h>
#include "prog_states.h"
#include "GameState.h"
#include "inputs.h"
#include "render.h"

extern void clear_screen();
extern void draw_string_wrapped(int x, int y, const char *str, int color, int max_width);



// Initalize a game state
GameState run_start_up() {
    // Query game mode and difficulty
    int gm = query_game_mode(); // Sets number of players and diffuculty in gm varibale. 
    int diff = query_game_difficulty();
    // Initiate the game state:
    print("Starting GameState initalization Sequence with mode %d and difficulty %d\n", gm, diff);
    GameState gs;
    GameState_init(&gs, gm, diff);
    return  gs;
} 

/* Query the player about the game mode they want to play at*/
int query_game_mode() {
    clear_screen();   
    char* msg = "Select Game Mode: 1 or 2 Players by toggling the first switch up for single player. Switch up down for multiplayer. Press button to confirm.\n";
    draw_string_wrapped(35, 60, msg, WHITE, MSG_WIDTH);
    print(msg); // render function
    // Wait for user input and return selected mode
    while(1) {
        int mode = get_switch_state(0);      // Read the mode input
        if(get_btn() == 1) {                 // Poll the button
            clear_screen();
            return mode;                     // Return set mode
        }
    }
    clear_screen();
    return 1; // Placeholder return value
}

/* Query the player about the difficulty they want to play at*/
int query_game_difficulty() {
    char* msg = "Use the three first switches to set your difficulty. Note binary numbers! Press button to confirm.\n";
    draw_string_wrapped(35, 60, msg, WHITE, MSG_WIDTH);
    print(msg); // render function
    
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
            clear_screen();
        }
    }
    clear_screen();
    return 1; // Placeholder return value
}
 
/* Pause logic*/
void run_pause() {
    int status = get_switch_state(4);
    while(1) {
        print("Toggle switch 4 down to exit pause");
        char* msg = "Toggle switch 4 down to exit pause";
        clear_screen();
        draw_string_wrapped(35, 60, msg, WHITE, MSG_WIDTH);

        if(status = 0) {
            clear_screen();
            break;
        }
    }
}

/* Game Over*/
void run_game_over() {
    print("Game Over! Press button to restart.");
    clear_screen();
    char* msg = "Game Over! Press button to restart.";
    draw_string_wrapped(35, 60, msg, WHITE, MSG_WIDTH);
    while(1) {
        if(get_btn() == 1) {
            break;
        }
    }
}
