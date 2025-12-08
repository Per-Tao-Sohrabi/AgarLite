#include <stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define MAX_RADIUS 50
#define MIN_RADIUS 2
#define Food_count 100

extern void enable_interrupts(void);
extern void print(const char*);

#define BUFFER_SIZE (SCREEN_WIDTH*SCREEN_HEIGHT)

typedef struct {
    int x_pos;
    int y_pos;
    int radius;
    int hp;
    int dx;
    int dy;
    int velocity;
    int color;
} Player;

typedef struct {
    int x_pos;
    int y_pos;
    int radius;
    int nutrition;
} Food;

typedef struct {
    int var1;
    int var2;
    Player players[2]; // Can hold two players
    Food crumbs[Food_count]; // Can hold five food pieces
} GameState;

GameState game;

volatile char *VGA = (volatile char*) 0x08000000;

char frame_buffer1[SCREEN_HEIGHT*SCREEN_WIDTH];
char frame_buffer2[SCREEN_WIDTH*SCREEN_HEIGHT];

char *current_draw_buffer;
char *current_display_buffer;

void copy_to_vga(char *src){
  for(int i = 0; i < BUFFER_SIZE; i++){
    VGA[i] = src[i];
  }
}

void swap_buffer(){
  copy_to_vga(current_draw_buffer);

  char *temp = current_draw_buffer;
  current_draw_buffer = current_display_buffer;
  current_display_buffer = temp;
}

void clear_current_buffer(){
  for(int i = 0; i < BUFFER_SIZE; i++){
    current_draw_buffer[i] = 0;
  }
}



void clear_screen(){
    for (int i = 0; i < SCREEN_WIDTH*SCREEN_HEIGHT; i++)
        VGA[i] = 0; 
}

void draw_circle_to_buffer(char *buffer, int cx, int cy, int radius, int color) {
    int radius_squ = radius * radius;
    int min_y = cy - radius;
    int max_y = cy + radius;
    int min_x = cx - radius;
    int max_x = cx + radius;
    
    // 边界检查
    if (min_y < 0) min_y = 0;
    if (max_y >= SCREEN_HEIGHT) max_y = SCREEN_HEIGHT - 1;
    if (min_x < 0) min_x = 0;
    if (max_x >= SCREEN_WIDTH) max_x = SCREEN_WIDTH - 1;
    
    for (int y = min_y; y <= max_y; y++) {
        for (int x = min_x; x <= max_x; x++) {
            int dx = x - cx;
            int dy = y - cy;
            if (dx * dx + dy * dy <= radius_squ) {
                buffer[y * SCREEN_WIDTH + x] = color;
            }
        }
    }
}

void draw_circle(int cx, int cy, int radius, int color) {
    draw_circle_to_buffer(current_draw_buffer, cx, cy, radius, color);
}


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
                if(game.players[i].radius < 50){
                game.players[i].radius += game.crumbs[j].nutrition;
                }

                //new position
                game.crumbs[j].x_pos = rand() % SCREEN_WIDTH;
                game.crumbs[j].y_pos = rand() % SCREEN_HEIGHT;
            }
        }
    }
}

void render_game() {
    // 清除当前绘制缓冲区
    clear_current_buffer();

    // 绘制玩家
    for (int i = 0; i < 2; i++) {
        Player player = game.players[i];
        draw_circle(player.x_pos, player.y_pos, player.radius, 255); // 白色
    }

    // 绘制食物
    for (int i = 0; i < Food_count; i++) {
        Food food = game.crumbs[i];
        draw_circle(food.x_pos, food.y_pos, food.radius, 46); // 绿色
    }
    
    // 交换缓冲区（将绘制好的帧复制到VGA）
    swap_buffers();
}

void handle_interrupt (unsigned _irq)
{ }


void init_game(){
    game.players[0].x_pos = 50;
    game.players[0].y_pos = 50;
    game.players[0].radius = MIN_RADIUS;
    game.players[0].dx = 2;
    game.players[0].dy = 2;

    game.players[1].x_pos = 3;
    game.players[1].y_pos = 3;
    game.players[1].radius = MIN_RADIUS;
    game.players[1].dx = -2;
    game.players[1].dy = 2;

    
    for(int i = 0; i < Food_count; i++){
        game.crumbs[i].x_pos = rand() % SCREEN_WIDTH;
        game.crumbs[i].y_pos = rand() % SCREEN_HEIGHT;
        game.crumbs[i].radius = 1;
        game.crumbs[i].nutrition = 1;
    }
}

void init_buffers() {
    // 初始化双缓冲
    current_draw_buffer = frame_buffer1;
    current_display_buffer = frame_buffer2;
    
    // 清除两个缓冲区
    for (int i = 0; i < BUFFER_SIZE; i++) {
        frame_buffer1[i] = 0;
        frame_buffer2[i] = 0;
    }
    
    // 初始化VGA显示
    copy_to_vga(current_draw_buffer);
}

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

int main() { 
    enable_interrupts();
    
    // 初始化双缓冲系统
    init_buffers();
    
    // 初始化游戏状态
    init_game();
    
    // 主游戏循环
    while (1) {
        update_game();
        render_game();
        
        // 控制帧率
        delay(800000);
    }
    
    return 0;
}