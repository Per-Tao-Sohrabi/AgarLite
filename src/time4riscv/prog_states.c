#include <stdbool.h>
#include "prog_states.h"
#include "GameState.h"
#include "inputs.h"
#include "render.h"

extern void clear_current_buffer(void);
extern void draw_msg(char* ch);
extern void swap_buffers(void);

// Edge-detection state for the button
static int last_btn = 0;

// Persistent menu state
static int selected_mode = 0;
static int selected_difficulty = 0;
static bool screen_drawn = false;  // Only draw menu text once per state entry

/* Returns true only on the frame the button transitions 0 → 1 */
bool btn_just_pressed(void) {
    int current = get_btn();
    bool pressed = (current == 1) && (last_btn == 0);
    last_btn = current;
    return pressed;
}

/* STATE_MENU_MODE: player selects 1P or 2P via switch 0 */
ProgramState state_menu_mode(GameState* gs) {
    if (!screen_drawn) {
        clear_current_buffer();
        draw_msg("Select Game Mode: toggle switch 0.\nUp = 2P, Down = 1P.\nPress button to confirm.");
        screen_drawn = true;
    }
    selected_mode = get_switch_state(0);
    if (btn_just_pressed()) {
        screen_drawn = false;
        return STATE_MENU_DIFFICULTY;
    }
    return STATE_MENU_MODE;
}

/* STATE_MENU_DIFFICULTY: player selects difficulty via switches 0-2 */
ProgramState state_menu_difficulty(GameState* gs) {
    if (!screen_drawn) {
        clear_current_buffer();
        draw_msg("Set difficulty with switches 0-2.\nPress button to confirm.");
        screen_drawn = true;
    }
    selected_difficulty = get_switch_state(0) + get_switch_state(1) + get_switch_state(2);
    if (btn_just_pressed()) {
        screen_drawn = false;
        GameState_init(gs, selected_mode, selected_difficulty);
        return STATE_PLAYING;
    }
    return STATE_MENU_DIFFICULTY;
}

/* STATE_PLAYING: core game loop — move, collide, render */
ProgramState state_playing(GameState* gs, int input_vector[]) {
    if (get_switch_state(4)) {
        screen_drawn = false;  // Reset for pause screen
        return STATE_PAUSED;
    }

    bool game_over = GameState_update(gs, input_vector);
    render_game(gs);

    if (game_over) {
        screen_drawn = false;  // Reset for game over screen
        return STATE_GAME_OVER;
    }
    return STATE_PLAYING;
}

/* STATE_PAUSED: wait for switch 4 to toggle off */
ProgramState state_paused(void) {
    static int pause_substate = 0; // 0 = main, 1 = confirm restart
    static int debounce_timer = 0;
    static int last_switch_state = -1;

    if (debounce_timer > 0) debounce_timer--;

    if (pause_substate == 0) {
        if (!screen_drawn) {
            clear_current_buffer();
            draw_pause_box("Paused.\nDown S4 to resume.\nPress btn to restart.", "Restart", NULL);
            screen_drawn = true;
        }

        if (!get_switch_state(4)) {
            screen_drawn = false;
            // Reset for next pause
            pause_substate = 0;
            return STATE_PLAYING;
        }

        if (btn_just_pressed() && debounce_timer == 0) {
            pause_substate = 1;
            screen_drawn = false;
            debounce_timer = 15;    // 15 frames debounce
            last_switch_state = -1;
        }
    } else if (pause_substate == 1) {
        int current_switch = get_switch_state(0);

        if (!screen_drawn || current_switch != last_switch_state) {
            clear_current_buffer();
            draw_confirm_box("Are you sure you want to restart?\nUp S0 = Yes, Down S0 = No", current_switch);
            screen_drawn = true;
            last_switch_state = current_switch;
        }

        if (btn_just_pressed() && debounce_timer == 0) {
            pause_substate = 0;     // reset for next pause
            screen_drawn = false;
            last_switch_state = -1;
            debounce_timer = 15;    // debounce for whatever state comes next

            if (current_switch == 1) {
                return STATE_MENU_MODE; // Yes, restart
            } else {
                return STATE_PAUSED;    // No, back to main pause screen
            }
        }
    }

    return STATE_PAUSED;
}

/* STATE_GAME_OVER: display message, button restarts */
ProgramState state_game_over(void) {
    if (!screen_drawn) {
        clear_current_buffer();
        draw_msg("Game Over!\nPress button to restart.");
        screen_drawn = true;
    }
    if (btn_just_pressed()) {
        screen_drawn = false;
        return STATE_MENU_MODE;
    }
    return STATE_GAME_OVER;
}