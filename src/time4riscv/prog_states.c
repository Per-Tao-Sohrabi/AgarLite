#include <stdio.h>
#include <stdbool.h>
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

// Edge-detection state for the button
static int last_btn = 0;

/* Returns true only on the frame the button transitions from released to pressed */
bool btn_just_pressed() {
    int current = get_btn();
    bool pressed = (current == 1) && (last_btn == 0);
    last_btn = current;
    return pressed;
}

// Initialize a game state
GameState run_start_up() {
    // Query game mode and difficulty
    int gm = query_game_mode();
    int diff = query_game_difficulty();
    // Initiate the game state:
    print("Starting GameState initalization Sequence\n");
    GameState gs;
    GameState_init(&gs, gm, diff);
    return gs;
} 

/* Query the player about the game mode they want to play at */
int query_game_mode() {
    clear_current_buffer(); 
    char* msg = "Select Game Mode: 1 or 2 Players by toggling the first switch up for single player. \nSwitch up down for multiplayer. Press button to confirm.\n";
    draw_msg(msg);
    print(msg);

    // Wait for user input and return selected mode
    while (1) {
        int mode = get_switch_state(0);
        if (btn_just_pressed()) {
            clear_current_buffer();
            swap_buffers();
            return mode;
        }
    }
    return 1;
}

/* Query the player about the difficulty they want to play at */
int query_game_difficulty() {
    clear_current_buffer();
    char* msg = "Use the three first switches to set your difficulty. \nNote binary numbers! Press button to confirm.\n";
    draw_msg(msg);

    while (1) {
        int sw1 = get_switch_state(0);
        int sw2 = get_switch_state(1);
        int sw3 = get_switch_state(2);
        int diff = sw1 + sw2 + sw3;

        if (btn_just_pressed()) {
            if (diff >= 0 && diff <= 7) {
                return diff;
            }
        }
    }
    return 1;
}
 
/* Pause logic */
void run_pause() {
    clear_current_buffer();
    print("Toggle switch 4 down to exit pause");
    char* msg = "Toggle switch 4 down to exit pause";
    draw_msg(msg);
    while (1) {
        int status = get_switch_state(4);
        if (status == 0) {
            clear_current_buffer();
            swap_buffers();
            break;
        }
    }
}

/* Game Over */
void run_game_over() {
    clear_current_buffer();
    print("Game Over! Press button to restart.");
    char* msg = "Game Over! \nPress button to restart.";
    draw_msg(msg);

    while (1) {
        if (btn_just_pressed()) {
            clear_current_buffer();
            swap_buffers();
            break;
        }
    }
}