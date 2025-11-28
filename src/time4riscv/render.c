#include "Entities.h"
#include "GameState.h"


#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 480
#define MAX_RADIUS 50
#define MIN_RADIUS 2

void draw_circle (int cx, int cy, int radius, int color) {
  int radius_squ = radius * radius;
  for(int y = cy - radius; y <= cy+radius; y++){
    for(int x = cx -radius; x <= cx+radius; x++){
      if(x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT){
        int dx = x-cx;
        int dy = y-cy;
        if(dx*dx+dy*dy <= radius_squ)
          VGA[y * SCREEN_WIDTH + x] = color;    
      }
    }
  }
}


int check_collision(Player a, Player b){
    int dx = a.x_pos - b.x_pos;
    int dy = a.y_pos - b.y_pos;
    int distance_squared = dx*dx + dy*dy;
    int radius_sum = a.radius + b.radius;
    return distance_squared <= radius_sum * radius_sum;
}

void update_game(GameState){

    //player
    // int antal_player = sizeof(GameState.player) / sizeof(GameState.player[0]); 
    // for(int i = 0; i < antal_player; i++){
    //     Player player = GameState.player[i];
    //     player.x_pos += player.dx;
    //     player.y_pos += player.dy;
    // }

    //food random?
   

    if(check_collision(GameState.player[0], GameState.player[1])){
        if(GameState.player[0].radius > GameState.player[1].radius){
            GameState.player[0].radius += GameState.player[1].radius/2;
            GameState.player[1].hp--;
        }else{
            GameState.player[1].radius += GameState.player[0].radius/2;
            GameState.player[0].hp--;
        }
    }

    int antal_player = sizeof(GameState.player) / sizeof(GameState.player[0]); 
    int antal_food = sizeof(GameState.crumbs) / sizeof(GameState.crumbs[0]); 
    for(int i = 0; i < antal_player; i++){
        for(int j = 0; j < antal_food; j++){
            if(check_collision(GameState.player[i], GameState.crumbs[j])){
                if(GameState.player[i].radius < MAX_RADIUS){
                GameState.player[i].radius += GameState.crumbs[j].nutrition;
                }

                //new position
                GameState.crumbs[j].x_pos = rand() % SCREEN_WIDTH;
                GameState.crumbs[j].y_pos = rand() % SCREEN_HEIGHT;
            }
        }
    }
}

void render_game(GameState) {

    //player
    int antal_player = sizeof(GameState.player) / sizeof(GameState.player[0]); 
    for(int i = 0; i < antal_player; i++){
        Player player = GameState.player[i];
        draw_circle(player.x_pos, player.y_pos, player.radius, player.color);
    }

    //food
    int antal_food = sizeof(GameState.crumbs) / sizeof(GameState.crumbs[0]); 
    for(int i = 0; i < antal_food;  i++){
        Food food = GameState.crumbs[i];
        draw_circle(food.x_pos, food.y_pos, food.radius, food.nutrition);
    }
   
    //time
}