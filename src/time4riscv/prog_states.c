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

static int btn_action_timer = 0;
static int btn_press_count = 0;

/* Advanced input detection: returns 0 (none), 1 (single press), 2 (double press) */
int get_btn_action(void) {
    bool pressed = btn_just_pressed();
    
    if (pressed) {
        btn_press_count++;
        if (btn_press_count == 1) {
            btn_action_timer = 15; // 15 frames max delay between clicks
        }
    }
    
    if (btn_press_count > 0) {
        if (btn_action_timer > 0) {
            btn_action_timer--;
            
            if (btn_press_count == 2) {
                // Double press confirmed
                btn_press_count = 0;
                btn_action_timer = 0;
                return 2;
            }
        } else {
            // Timer expired, confirm whatever we have
            int result = btn_press_count; // should be 1
            btn_press_count = 0;
            return result == 1 ? 1 : 0;
        }
    }
    
    return 0;
}

/* STATE_SPLASH: Fades in starting text */
ProgramState state_splash(void) {
    static int splash_timer = 0;
    
    clear_current_buffer();
    
    // Animate "...", advance every 10 frames (faster & drawn every tick to prevent buffer staleness)
    if (splash_timer < 10) {
        draw_msg("AgarLite\nStarting");
    } else if (splash_timer < 20) {
        draw_msg("AgarLite\nStarting.");
    } else if (splash_timer < 30) {
        draw_msg("AgarLite\nStarting..");
    } else if (splash_timer < 40) {
        draw_msg("AgarLite\nStarting...");
    } else {
        // Auto transition after animation
        splash_timer = 0;
        screen_drawn = false;
        return STATE_MENU_START;
    }
    
    splash_timer++;

    int action = get_btn_action();
    if (action == 1) { // single press to skip
        splash_timer = 0;
        screen_drawn = false;
        return STATE_MENU_START;
    }
    
    return STATE_SPLASH;
}

/* STATE_MENU_START: Unified menu handler for mode, difficulty, and confirm */
ProgramState state_menu_start(GameState* gs) {
    static int menu_substate = 0; // 0=Mode, 1=Diff, 2=Confirm
    static int last_drawn_substate = -1;
    static int last_drawn_mode = -1;
    static int last_drawn_diff = -1;

    // Use static variables to hold the displayed state, preventing
    // switches from updating inactive components
    static int display_mode = 0;
    static int display_diff = 0;

    // Only update the displayed variables if we are currently in that substate
    if (menu_substate == 0) {
        display_mode = get_switch_state(0);
    } else if (menu_substate == 1) {
        display_diff = get_switch_state(0) | (get_switch_state(1) << 1) | (get_switch_state(2) << 2);
    }

    int action = get_btn_action();

    if (action == 1) { // Single press -> Forward/Confirm
        if (menu_substate == 2) {
            GameState_init(gs, display_mode, display_diff);
            
            // Re-initialize local statics for the next time we enter this state
            menu_substate = 0;
            last_drawn_substate = -1;
            screen_drawn = false;
            
            return STATE_FADE_TO_GAME;
        } else {
            menu_substate++;
        }
    } else if (action == 2) { // Double press -> Backward
        if (menu_substate > 0) {
            menu_substate--;
        }
    }

    // Only draw if substate changed, or if our selected value in the current substate changed.
    if (!screen_drawn || last_drawn_substate != menu_substate || 
        last_drawn_mode != display_mode || last_drawn_diff != display_diff) {
        
        draw_start_menu(menu_substate, display_mode, display_diff);
        
        last_drawn_substate = menu_substate;
        last_drawn_mode = display_mode;
        last_drawn_diff = display_diff;
        screen_drawn = true;
    }
    
    return STATE_MENU_START;
}

/* STATE_FADE_TO_GAME: Screen wipe transition */
ProgramState state_fade_to_game(void) {
    // For now, jump straight to game
    return STATE_PLAYING;
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
            //clear_current_buffer();
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
            //clear_current_buffer();
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
                return STATE_MENU_START; // Yes, restart
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
        //clear_current_buffer();
        
        draw_game_over_box("Game Over!", "Restart", NULL);
        screen_drawn = true;
    }
    if (btn_just_pressed()) {
        screen_drawn = false;
        return STATE_MENU_START;
    }
    return STATE_GAME_OVER;
}