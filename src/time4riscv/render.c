#include "render.h"
#include "Entities.h"
#include "GameState.h"

// #include "graphics.h"

volatile char *VGA = (volatile char*) VGA_BASE;
char back_buffer[VGA_BUFFER_SIZE];

const uint8_t font_5x7[96][7] = {
    // 空格 (ASCII 32)
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    
    // ! (33)
    {0x04, 0x04, 0x04, 0x04, 0x00, 0x04, 0x00},
    
    // " (34)
    {0x0A, 0x0A, 0x0A, 0x00, 0x00, 0x00, 0x00},
    
    // # (35)
    {0x0A, 0x0A, 0x1F, 0x0A, 0x1F, 0x0A, 0x0A},
    
    // $ (36)
    {0x04, 0x0F, 0x14, 0x0E, 0x05, 0x1E, 0x04},
    
    // % (37)
    {0x18, 0x19, 0x02, 0x04, 0x08, 0x13, 0x03},
    
    // & (38)
    {0x0C, 0x12, 0x14, 0x08, 0x15, 0x12, 0x0D},
    
    // ' (39)
    {0x0C, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00},
    
    // ( (40)
    {0x02, 0x04, 0x08, 0x08, 0x08, 0x04, 0x02},
    
    // ) (41)
    {0x08, 0x04, 0x02, 0x02, 0x02, 0x04, 0x08},
    
    // * (42)
    {0x00, 0x04, 0x15, 0x0E, 0x15, 0x04, 0x00},
    
    // + (43)
    {0x00, 0x04, 0x04, 0x1F, 0x04, 0x04, 0x00},
    
    // , (44)
    {0x00, 0x00, 0x00, 0x00, 0x0C, 0x04, 0x08},
    
    // - (45)
    {0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00},
    
    // . (46)
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C},
    
    // / (47)
    {0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x00},
    
    // 0 (48)
    {0x0E, 0x11, 0x13, 0x15, 0x19, 0x11, 0x0E},
    
    // 1 (49)
    {0x04, 0x0C, 0x04, 0x04, 0x04, 0x04, 0x0E},
    
    // 2 (50)
    {0x0E, 0x11, 0x01, 0x02, 0x04, 0x08, 0x1F},
    
    // 3 (51)
    {0x0E, 0x11, 0x01, 0x06, 0x01, 0x11, 0x0E},
    
    // 4 (52)
    {0x02, 0x06, 0x0A, 0x12, 0x1F, 0x02, 0x02},
    
    // 5 (53)
    {0x1F, 0x10, 0x1E, 0x01, 0x01, 0x11, 0x0E},
    
    // 6 (54)
    {0x06, 0x08, 0x10, 0x1E, 0x11, 0x11, 0x0E},
    
    // 7 (55)
    {0x1F, 0x01, 0x02, 0x04, 0x04, 0x04, 0x04},
    
    // 8 (56)
    {0x0E, 0x11, 0x11, 0x0E, 0x11, 0x11, 0x0E},
    
    // 9 (57)
    {0x0E, 0x11, 0x11, 0x0F, 0x01, 0x02, 0x0C},
    
    // : (58)
    {0x00, 0x0C, 0x0C, 0x00, 0x0C, 0x0C, 0x00},
    
    // ; (59)
    {0x00, 0x0C, 0x0C, 0x00, 0x0C, 0x04, 0x08},
    
    // < (60)
    {0x02, 0x04, 0x08, 0x10, 0x08, 0x04, 0x02},
    
    // = (61)
    {0x00, 0x00, 0x1F, 0x00, 0x1F, 0x00, 0x00},
    
    // > (62)
    {0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08},
    
    // ? (63)
    {0x0E, 0x11, 0x01, 0x02, 0x04, 0x00, 0x04},
    
    // @ (64)
    {0x0E, 0x11, 0x01, 0x0D, 0x15, 0x15, 0x0E},
    
    // A (65)
    {0x0E, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11},
    
    // B (66)
    {0x1E, 0x11, 0x11, 0x1E, 0x11, 0x11, 0x1E},
    
    // C (67)
    {0x0E, 0x11, 0x10, 0x10, 0x10, 0x11, 0x0E},
    
    // D (68)
    {0x1E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1E},
    
    // E (69)
    {0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x1F},
    
    // F (70)
    {0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x10},
    
    // G (71)
    {0x0E, 0x11, 0x10, 0x17, 0x11, 0x11, 0x0F},
    
    // H (72)
    {0x11, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11},
    
    // I (73)
    {0x0E, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0E},
    
    // J (74)
    {0x07, 0x02, 0x02, 0x02, 0x02, 0x12, 0x0C},
    
    // K (75)
    {0x11, 0x12, 0x14, 0x18, 0x14, 0x12, 0x11},
    
    // L (76)
    {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1F},
    
    // M (77)
    {0x11, 0x1B, 0x15, 0x15, 0x11, 0x11, 0x11},
    
    // N (78)
    {0x11, 0x11, 0x19, 0x15, 0x13, 0x11, 0x11},
    
    // O (79)
    {0x0E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E},
    
    // P (80)
    {0x1E, 0x11, 0x11, 0x1E, 0x10, 0x10, 0x10},
    
    // Q (81)
    {0x0E, 0x11, 0x11, 0x11, 0x15, 0x12, 0x0D},
    
    // R (82)
    {0x1E, 0x11, 0x11, 0x1E, 0x14, 0x12, 0x11},
    
    // S (83)
    {0x0F, 0x10, 0x10, 0x0E, 0x01, 0x01, 0x1E},
    
    // T (84)
    {0x1F, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04},
    
    // U (85)
    {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E},
    
    // V (86)
    {0x11, 0x11, 0x11, 0x11, 0x11, 0x0A, 0x04},
    
    // W (87)
    {0x11, 0x11, 0x11, 0x15, 0x15, 0x1B, 0x11},
    
    // X (88)
    {0x11, 0x11, 0x0A, 0x04, 0x0A, 0x11, 0x11},
    
    // Y (89)
    {0x11, 0x11, 0x11, 0x0A, 0x04, 0x04, 0x04},
    
    // Z (90)
    {0x1F, 0x01, 0x02, 0x04, 0x08, 0x10, 0x1F},
    
    // [ (91)
    {0x0E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x0E},
    
    // \ (92)
    {0x00, 0x10, 0x08, 0x04, 0x02, 0x01, 0x00},
    
    // ] (93)
    {0x0E, 0x02, 0x02, 0x02, 0x02, 0x02, 0x0E},
    
    // ^ (94)
    {0x04, 0x0A, 0x11, 0x00, 0x00, 0x00, 0x00},
    
    // _ (95)
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F},
    
    // ` (96)
    {0x08, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00},
    
    // a (97)
    {0x00, 0x00, 0x0E, 0x01, 0x0F, 0x11, 0x0F},
    
    // b (98)
    {0x10, 0x10, 0x16, 0x19, 0x11, 0x11, 0x1E},
    
    // c (99)
    {0x00, 0x00, 0x0E, 0x11, 0x10, 0x11, 0x0E},
    
    // d (100)
    {0x01, 0x01, 0x0D, 0x13, 0x11, 0x11, 0x0F},
    
    // e (101)
    {0x00, 0x00, 0x0E, 0x11, 0x1F, 0x10, 0x0E},
    
    // f (102)
    {0x06, 0x09, 0x08, 0x1C, 0x08, 0x08, 0x08},
    
    // g (103)
    {0x00, 0x0F, 0x11, 0x11, 0x0F, 0x01, 0x0E},
    
    // h (104)
    {0x10, 0x10, 0x16, 0x19, 0x11, 0x11, 0x11},
    
    // i (105)
    {0x04, 0x00, 0x0C, 0x04, 0x04, 0x04, 0x0E},
    
    // j (106)
    {0x02, 0x00, 0x06, 0x02, 0x02, 0x12, 0x0C},
    
    // k (107)
    {0x10, 0x10, 0x12, 0x14, 0x18, 0x14, 0x12},
    
    // l (108)
    {0x0C, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0E},
    
    // m (109)
    {0x00, 0x00, 0x1A, 0x15, 0x15, 0x15, 0x15},
    
    // n (110)
    {0x00, 0x00, 0x16, 0x19, 0x11, 0x11, 0x11},
    
    // o (111)
    {0x00, 0x00, 0x0E, 0x11, 0x11, 0x11, 0x0E},
    
    // p (112)
    {0x00, 0x00, 0x1E, 0x11, 0x1E, 0x10, 0x10},
    
    // q (113)
    {0x00, 0x00, 0x0D, 0x13, 0x0F, 0x01, 0x01},
    
    // r (114)
    {0x00, 0x00, 0x16, 0x19, 0x10, 0x10, 0x10},
    
    // s (115)
    {0x00, 0x00, 0x0E, 0x10, 0x0E, 0x01, 0x1E},
    
    // t (116)
    {0x08, 0x08, 0x1C, 0x08, 0x08, 0x09, 0x06},
    
    // u (117)
    {0x00, 0x00, 0x11, 0x11, 0x11, 0x13, 0x0D},
    
    // v (118)
    {0x00, 0x00, 0x11, 0x11, 0x11, 0x0A, 0x04},
    
    // w (119)
    {0x00, 0x00, 0x11, 0x11, 0x15, 0x15, 0x0A},
    
    // x (120)
    {0x00, 0x00, 0x11, 0x0A, 0x04, 0x0A, 0x11},
    
    // y (121)
    {0x00, 0x00, 0x11, 0x11, 0x0F, 0x01, 0x0E},
    
    // z (122)
    {0x00, 0x00, 0x1F, 0x02, 0x04, 0x08, 0x1F},
    
    // { (123)
    {0x02, 0x04, 0x04, 0x08, 0x04, 0x04, 0x02},
    
    // | (124)
    {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04},
    
    // } (125)
    {0x08, 0x04, 0x04, 0x02, 0x04, 0x04, 0x08},
    
    // ~ (126)
    {0x00, 0x00, 0x08, 0x15, 0x02, 0x00, 0x00},
};

void clear_screen(){
    for (int i = 0; i < SCREEN_WIDTH*SCREEN_HEIGHT; i++){
        VGA[i] = 0; 
        back_buffer[i] = 0;
    }
}

void clear_backbuffer(){
    for (int i = 0; i < VGA_BUFFER_SIZE; i++)
        back_buffer[i] = 0; 
}

void swap_buffers(){
    for(int i = 0; i < VGA_BUFFER_SIZE; i++){
        VGA[i] = back_buffer[i];
    }
}

void draw_circle (int cx, int cy, int radius, int color) {
  int radius_squ = radius * radius;
  for(int y = cy - radius; y <= cy+radius; y++){
    for(int x = cx -radius; x <= cx+radius; x++){
      if(x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT){
        int dx = x-cx;
        int dy = y-cy;
        if(dx*dx+dy*dy <= radius_squ){
            int offset = y * SCREEN_WIDTH + x; 
            back_buffer[offset] = color; 
        }   
      }
    }
  }
}

void draw_filled_rect(int x, int y, int width, int height, int color){
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            draw_pixel(x+j, y+i, color);
        }
    }
}

void draw_pixel(int x, int y, int color){
    if(x < 0 || x > SCREEN_WIDTH || y < 0 || y > SCREEN_HEIGHT){
        return;
    }

    int offset = y * SCREEN_WIDTH + x;
    back_buffer[offset] = color;
}

void draw_char(int x, int y, char ch, int color){
    if(ch < 32 || ch > 126){
        return;
    }
    
    int index = ch - 32;
    for(int row = 0; row < 7; row++){
        uint8_t line_data = font_5x7[index][row];

        for(int col = 0; col < 5; col++){
            if(line_data & (0x10 >> col)){
                draw_pixel(x+col, y+row, color);
            }
        }
    }
}

void draw_string(int x, int y, const char *str, int color){
    int start_x = x;

    while(*str){
        if(*str == '\n'){
            y += 8;
            x = start_x;

        }else if(*str == '\t'){
            x += 4*6;

        }else if(*str == '\b'){
            x -= 6;
            draw_filled_rect(x, y, 6, 7, 0);
        }
        else{
            draw_char(x, y, *str, color);
            x += FONT_WIDTH + CHAR_SPACING;
        }
        str++;
    }

}

void draw_string_centered(int y, const char *str, int color){
    int len = 0;
    const char* temp = str;

    while(*temp && *temp != '\n'){
        len++;
        temp++;
    }

    int total_width = len*(FONT_WIDTH + CHAR_SPACING);
    int x = (SCREEN_WIDTH-total_width)/2;
    draw_string(x, y, str, color);
}

// void draw_string_wrapped(int x, int y, const char *str, int color, int max_width){
//     int start_x = x;
//     int word_start_x = x;
//     const char* word_start = str;
//     while(*str){
//         if(*str == '\n'){
//             y += FONT_HEIGHT + LINE_SPACING;
//             x = start_x;
//             word_start_x = x;
//             str++;
//             word_start = str;
//         }else if(*str == ' '){
//             draw_char(x, y, ' ', color);
//             x += FONT_WIDTH + CHAR_SPACING;
//             str++;
//             word_start_x = x;
//             word_start = str;
//         }else{
//             int word_length = 0;
//             const char* temp = word_start;
//             while((*temp) && (*temp != ' ') && (*temp != '\n')){
//                 word_length++;
//                 temp++;
//             }          
//             int word_pixels = word_length * (FONT_WIDTH + CHAR_SPACING);
//             if(x + word_pixels - word_start_x > max_width){
//                 y += FONT_HEIGHT + LINE_SPACING;
//                 x = start_x;
//                 word_start_x = x;
//             }
//             draw_char(x, y, *str, color);
//             x += FONT_WIDTH + CHAR_SPACING;
//             str++;
//         }
//     }
// }

// void draw_string_wrapped(int x, int y, const char *str, int color, int max_width) {
//     int current_x = x;
//     int current_y = y;
//     int line_start = 0;
//     int i = 0;
    
//     int actual_char_width = FONT_WIDTH + CHAR_SPACING;

//     if (max_width <= 0) {
//         max_width = SCREEN_WIDTH - x;
//     }
    
    
//     if (x + max_width > SCREEN_WIDTH) {
//         max_width = SCREEN_WIDTH - x;
//     }
    
    
//     if (y >= SCREEN_HEIGHT) {
//         return;  
//     }
    
//     while (str[i] != '\0') {
        
//         if (current_y >= SCREEN_HEIGHT - FONT_HEIGHT) {
           
//             break;
//         }
        
//         if (str[i] == '\n') {
//             current_x = x;
//             current_y += FONT_HEIGHT + LINE_SPACING;
//             line_start = i + 1;
//             i++;
//             continue;
//         }
        
//         if (current_x + actual_char_width > x + max_width) {
            
            
//             int last_space = i - 1;
//             while (last_space > line_start && str[last_space] != ' ') {
//                 last_space--;
//             }
            
//             if (last_space > line_start && str[last_space] == ' ') {
            
//                 i = last_space + 1;
//                 current_x = x;
//                 current_y += FONT_HEIGHT + LINE_SPACING;
//                 line_start = i;
                
                
//                 if (current_y >= SCREEN_HEIGHT - FONT_HEIGHT) {
//                     break;
//                 }
//                 continue;
//             } else {
                
//                 current_x = x;
//                 current_y += FONT_HEIGHT + LINE_SPACING;
//                 line_start = i;
                
//                 if (current_y >= SCREEN_HEIGHT - FONT_HEIGHT) {
//                     break;
//                 }
                
//                 if (current_x < SCREEN_WIDTH && current_y < SCREEN_HEIGHT) {
//                     draw_char(current_x, current_y, str[i], color);
//                 }
//                 current_x += FONT_WIDTH;
//                 i++;
//                 continue;
//             }
//         }
        
//         if (current_x < SCREEN_WIDTH && current_y < SCREEN_HEIGHT) {
//             draw_char(current_x, current_y, str[i], color);
//         }
//         current_x += actual_char_width;
//         i++;
//     }
// }

// void draw_string_wrapped(int x, int y, const char *str, int color, int max_width){
//     int current_x = x;
//     int current_y = y;
//     int word_start_index = 0;
//     int i = 0;

//     int char_width = FONT_WIDTH + CHAR_SPACING;

//     while(str[i] != '\0'){
//         int word_end_index = i;
//         while(str[word_end_index] != '\0' &&
//               str[word_end_index] != ' ' &&
//               str[word_end_index] != '\n'){
//                 word_end_index++;
//         }

//         int word_len = word_end_index - word_start_index;
//         int word_width = word_len * char_width;

//         //check if current row have space for the word
//         if(current_x + word_width > x + max_width && current_x > x){
//             // if not, dont draw and break line
//             current_x = x;
//             current_y += FONT_HEIGHT + LINE_SPACING;
//             //check boundries
//             if(current_y >= SCREEN_HEIGHT - FONT_HEIGHT){
//                 break;
//             }
//             //re-do this word
//             continue;
//         }

//         // break line
//         if(str[i] == '\n'){
//             current_x = x;
//             current_y += FONT_HEIGHT + LINE_SPACING;
//             word_start_index = i + 1;
//             i++;
//             continue;
//         }

//         //draw current char
//         if(current_x < SCREEN_WIDTH && current_y < SCREEN_HEIGHT){
//             draw_char(current_x, current_y, str[i], color);
//         }

//         current_x += char_width;
//         i++;
//         //if end of the word, space
//         if(i == word_end_index){
//             if(str[i] == ' '){
//                 if(current_x < SCREEN_WIDTH && current_y < SCREEN_HEIGHT){
//                     draw_char(current_x, current_y, ' ', color);
//                 }
//                 current_x += char_width;
//                 i++;
//             }

//             word_start_index = i;
//         }
//     }
// }


void draw_string_wrapped(int x, int y, const char *str, int color, int max_width){
    int current_x = x;
    int current_y = y;
    int char_width = FONT_WIDTH + CHAR_SPACING;

    const char *p = str;

    while(*p != '\0'){
        // hoppar över första space i första raden
        if(*p == ' ' && current_x == x){
            p++;
            continue;
        }
        //när \n
        if(*p == '\n'){
            current_x = x;
            current_y += FONT_HEIGHT + LINE_SPACING;
            p++;
            continue; 
        }

        //hittar nuvarande ord börja och sluta
        const char *word_start = p;
        const char *word_end = word_start;
        //hittar när ordet sluta
        while(*word_end != '\0' && *word_end != ' ' && *word_end != '\n'){
            word_end++;
        }
        // räkna ord-width
        int word_len = word_end - word_start;
        int word_width = word_len * char_width;

        if(current_x > x && current_x + word_width > x + max_width){
            current_x = x;
            current_y += FONT_HEIGHT + LINE_SPACING;

            if(current_y >= SCREEN_HEIGHT - FONT_HEIGHT){
                break;
            }
            continue;
        }

        for(const char *ch = word_start; ch < word_end; ch++){
            if(current_x < SCREEN_WIDTH && current_y < SCREEN_HEIGHT){
                draw_char(current_x, current_y, *ch, color);
            }
            current_x += char_width;
        }

        p = word_end;

        if(*p == ' '){
            if(current_x < SCREEN_WIDTH && current_y < SCREEN_HEIGHT){
                draw_char(current_x, current_y, ' ', color);
            }
            current_x += char_width;
            p++;
        }
    }
}


void render_game(GameState *game) {
    clear_backbuffer();
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

    //ai
    int antal_ai = sizeof(game -> ais)/sizeof(game->ais[0]);
    for(int i = 0; i < antal_ai; i++){
        Ai ai = game -> ais[i];
        draw_circle(ai.x_pos, ai.y_pos, ai.radius, ai.color);
    }
    swap_buffers();
}

void draw_msg(char* ch){
    draw_filled_rect(35, 60, MSG_WIDTH, MSG_HEIGHT, 0);
    draw_string_wrapped(35, 60, ch, 255, MSG_WIDTH);
    swap_buffers();
}