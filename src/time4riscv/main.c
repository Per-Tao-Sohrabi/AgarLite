#include <stdio.h>
#include <stdlib.h>
#include "GameState.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 480
#define MAX_RADIUS 50
#define MIN_RADIUS 2
#define Food_count 100

extern void enable_interrupts(void);

GameState game;

volatile char *VGA = (volatile char*) 0x08000000;

// void clear_screen(){
//     for (int i = 0; i < SCREEN_WIDTH*SCREEN_HEIGHT; i++)
//         VGA[i] = 0; 
// }

// void draw_circle (int cx, int cy, int radius, int color) {
//   int radius_squ = radius * radius;
//   for(int y = cy - radius; y <= cy+radius; y++){
//     for(int x = cx -radius; x <= cx+radius; x++){
//       if(x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT){
//         int dx = x-cx;
//         int dy = y-cy;
//         if(dx*dx+dy*dy <= radius_squ)
//           VGA[y * SCREEN_WIDTH + x] = color;    
//       }
//     }
//   }
// }

int check_collision_player(Player a, Player b){
    int dx = a.x_pos - b.x_pos;
    int dy = a.y_pos - b.y_pos;
    int distance_squared = dx*dx + dy*dy;
    int radius_sum = a.radius + b.radius;
    return distance_squared <= radius_sum * radius_sum;
}

int check_collision_food(Player a, Food b){
    int dx = a.x_pos - b.x_pos;
    int dy = a.y_pos - b.y_pos;
    int distance_squared = dx*dx + dy*dy;
    int radius_sum = a.radius + b.radius;
    return distance_squared <= radius_sum * radius_sum;
}

void update_game(){

    int antal_player = sizeof(game.players) / sizeof(game.players[0]); 
    int antal_food = sizeof(game.crumbs) / sizeof(game.crumbs[0]); 

    for(int i = 0; i < antal_player; i++){
        game.players[i].x_pos += game.players[i].dx;
        game.players[i].y_pos += game.players[i].dy;

        if(game.players[i].x_pos < game.players[i].radius){
            game.players[i].x_pos = game.players[i].radius;
        }

        if(game.players[i].x_pos > SCREEN_WIDTH - game.players[i].radius){
            game.players[i].x_pos = SCREEN_WIDTH - game.players[i].radius;
        }

        if(game.players[i].y_pos < game.players[i].radius){
            game.players[i].y_pos = game.players[i].radius;
        }

        if(game.players[i].y_pos > SCREEN_HEIGHT - game.players[i].radius){
            game.players[i].y_pos = SCREEN_HEIGHT - game.players[i].radius;
        }
    }
   

    if(check_collision_player(game.players[0], game.players[1])){
        if(game.players[0].radius > game.players[1].radius){
            game.players[0].radius += game.players[1].radius/2;
            game.players[1].hp--;
        }else{
            game.players[1].radius += game.players[0].radius/2;
            game.players[0].hp--;
        }
    }

    for(int i = 0; i < antal_player; i++){
        for(int j = 0; j < antal_food; j++){
            if(check_collision_food(game.players[i], game.crumbs[j])){
                if(game.players[i].radius < MAX_RADIUS){
                game.players[i].radius += game.crumbs[j].nutrition;
                }

                //new position
                game.crumbs[j].x_pos = rand() % SCREEN_WIDTH;
                game.crumbs[j].y_pos = rand() % SCREEN_HEIGHT;
            }
        }
    }
}

// void render_game() {
//     clear_screen();

//     //player
//     int antal_player = sizeof(game.players) / sizeof(game.players[0]); 
//     for(int i = 0; i < antal_player; i++){
//         Player player = game.players[i];
//         draw_circle(player.x_pos, player.y_pos, player.radius, 255);
//     }

//     //food
//     int antal_food = sizeof(game.crumbs) / sizeof(game.crumbs[0]); 
//     for(int i = 0; i < antal_food;  i++){
//         Food food = game.crumbs[i];
//         draw_circle(food.x_pos, food.y_pos, food.radius, 46);
//     }
   
//     //time
// }

void handle_interrupt (unsigned _irq)
{ }


// void init_game(){
//     game.players[0].x_pos = 50;
//     game.players[0].y_pos = 50;
//     game.players[0].radius = MIN_RADIUS;
//     game.players[0].dx = 2;
//     game.players[0].dy = 2;

//     game.players[1].x_pos = 3;
//     game.players[1].y_pos = 3;
//     game.players[1].radius = MIN_RADIUS;
//     game.players[1].dx = -2;
//     game.players[1].dy = 2;

    
//     for(int i = 0; i < Food_count; i++){
//         game.crumbs[i].x_pos = rand() % SCREEN_WIDTH;
//         game.crumbs[i].y_pos = rand() % SCREEN_HEIGHT;
//         game.crumbs[i].radius = 1;
//         game.crumbs[i].nutrition = 1;
//     }
// }

volatile int* timer = (volatile int*) 0x04000020;

void labinit(void) {
  print("---- Initializing timer...\n");
  // Set period to 3 MHz:
  int period_val = 3000000 -1; // Subtract 1 because timer counts from 0
  timer[2] = period_val & 0xFFFF; //  Lower 16 bits
  timer[3] = (period_val >> 16) & 0xFFFF;

  // Set start status
  timer[0] = 0b110; // Enable timer, sets ito = off, cont = on, start = on, stop. = off. 
  print("---- Timer start status set.\n");
}

void delay(int cycles){
    for(int i = 0; i < cycles; i++);
}

int main()
{ 
    enable_interrupts();
    init_game();
    char* msg = "Select Game Mode: 1 or 2 Players by toggling the first switch up for single player. Switch up down for multiplayer. Press button to confirm.\n";
    draw_string_wrapped(10, 180, &msg, 300);

    while((timer[0] & 0b1) == 0 ) {
    }
    timer[0] = 0b1;
  
  // Enter a forever loop
    while (1)
        {
            update_game();
            render_game(&game);

            while((timer[0] & 0b1) == 0 ) {}
                timer[0] = 0b1;
        }
}