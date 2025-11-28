#include "GameState.h"

// Initalize a game state
GameState run_start_up_seq() {
    display_welcome_msg();
    int gm = prompt_game_mode(); // Sets number of players and diffuculty in gm varibale. 
    // Initalize game state based on game mode (gm)
    GameState gs;
    return  gs;
} 