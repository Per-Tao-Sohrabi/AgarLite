#include "render.h"
#include "Entities.h"
#include "GameState.h"

void vga_int(){
    volatile char *VGA = (volatile char*) VGA_BASE;
}

volatile char *VGA = (volatile char*) VGA_BASE;

void clear_screen(){
    for (int i = 0; i < SCREEN_WIDTH*SCREEN_HEIGHT; i++)
    VGA[i] = 0; 
};


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

void draw_pixel(int x, int y, int color){
    if(x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT){
        int offset = y * SCREEN_WIDTH + x;
        VGA[offset] = color;
    }
}

void draw_char(int x, int y, char ch, int color){
    if(ch < 32 || ch > 126) return;

    int index = ch - 32;
    for(int row = 0; row < FONT_HEIGHT; row++){
        uint8_t line = font_5x7[index][row];
        for(int col = 0; col < FONT_WIDTH; col++){
            if(line & (0x10 >> col)){
                draw_pixel(x+col, y+row, color);
            }
        }
    }
}

void draw_string(int x, int y, const char* str, int color){
    int start_x = x;

    while(*str){
        if(*str == '\n'){
            y+= FONT_HEIGHT + LINE_SPACING;
            x = start_x;
        }else if(*str == '\t'){
            x += (FONT_WIDTH + CHAR_SPACING) * 4;
        }else{
            draw_char(x, y, *str, color);
            x += FONT_WIDTH + CHAR_SPACING;
        }
        str++;
    }
}

void draw_string_wrapped(int x, int y, const char* str, int color, int max_width){
    int start_x = x;
    int word_start_x = x;
    const char* word_start = str;

    while(*str){
        if(*str == '\n'){
            y += FONT_HEIGHT + LINE_SPACING;
            x = start_x;
            word_start_x = x;
            str++;
            word_start = str;
        }else if(*str == ' '){
            draw_char(x, y, ' ', color);
            x += FONT_WIDTH + CHAR_SPACING;
            str++;
            word_start_x = x;
            word_start_x = str;
        }else{
            int word_length = 0;
            const char* temp = word_start;
            while((*temp) && (*temp != ' ') && (*temp != '\n')){
                word_length++;
                temp++;
            }
            
            int word_pixels = word_length * (FONT_WIDTH + CHAR_SPACING);
            if(x + word_pixels - word_start_x > max_width){
                y += FONT_HEIGHT + LINE_SPACING;
                x += start_x;
                word_start_x = x;
            }

            draw_char(x, y, *str, color);
            x += FONT_WIDTH + CHAR_SPACING;
            str++;
        }
    }
}

void draw_string_centered(int y, const char* str, int color) {
    int len = 0;
    const char* temp = str;
    while (*temp && *temp != '\n') {
        len++;
        temp++;
    }
    
    int total_width = len * (FONT_WIDTH + CHAR_SPACING);
    int x = (SCREEN_WIDTH - total_width) / 2;
    
    draw_string(x, y, str, color);
}

void render_game(GameState* game) {

    //players
    int antal_players = sizeof(game -> players) / sizeof(game -> players[0]); 
    for(int i = 0; i < antal_players; i++){
        Player player = game -> players[i];
        draw_circle(player.x_pos, player.y_pos, player.radius, player.color);
    }

    //food
    int antal_food = sizeof(game -> crumbs) / sizeof(game -> crumbs[0]); 
    for(int i = 0; i < antal_food;  i++){
        Food food = game -> crumbs[i];
        draw_circle(food.x_pos, food.y_pos, food.radius, food.nutrition);
    }
}