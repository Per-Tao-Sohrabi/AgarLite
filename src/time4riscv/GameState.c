#include "GameState.h"
#include "render.h"

void init_game(GameState* game){
    game->players[0].x_pos = 50;
    game->players[0].y_pos = 50;
    game->players[0].radius = MIN_RADIUS;
    game->players[0].dx = 2;
    game->players[0].dy = 2;

    game->players[1].x_pos = 3;
    game->players[1].y_pos = 3;
    game->players[1].radius = MIN_RADIUS;
    game->players[1].dx = -2;
    game->players[1].dy = 2;

    
    for(int i = 0; i < Food_count; i++){
        game->crumbs[i].x_pos = rand() % SCREEN_WIDTH;
        game->crumbs[i].y_pos = rand() % SCREEN_HEIGHT;
        game->crumbs[i].radius = 1;
        game->crumbs[i].nutrition = 1;
    }
}