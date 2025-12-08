// =========================================================
// In render.c
// =========================================================

#include "render.h" 
#include "GameState.h" 
#include "Entities.h"
#include "inputs.h"

// Pointer to the memory-mapped VGA device (the *visible* screen)
volatile uint8_t *const VGA_DISPLAY = VGA;

// The back buffer stored in fast RAM (the *hidden* screen)
uint8_t back_buffer[VGA_BUFFER_SIZE];

// [Insert font_5x7 definition here]...
// ...

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

// Clear the back buffer to a specific color
void clear_buffer(uint8_t color){
    for (int i = 0; i < VGA_BUFFER_SIZE; i++) {
        back_buffer[i] = color; 
    }
}

// Copy the back buffer to the visible VGA display memory
void swap_buffers(){
    // This is the simplest (but potentially slow) copy operation
    for(int i = 0; i < VGA_BUFFER_SIZE; i++){
        VGA_DISPLAY[i] = back_buffer[i];
    }
}

// CRITICAL FUNCTION: All drawing must flow through here.
void draw_pixel(int x, int y, uint8_t color){
    // Use x >= SCREEN_WIDTH or y >= SCREEN_HEIGHT for strict boundary check
    if(x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT){
        return;
    }

    int offset = y * SCREEN_WIDTH + x;
    
    // 🔥 FIX: ALWAYS write to the back_buffer.
    back_buffer[offset] = color; 
}

void draw_filled_rect(int x, int y, int width, int height, uint8_t color){
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            draw_pixel(x+j, y+i, color); // Writes to back_buffer via draw_pixel
        }
    }
}

void draw_circle (int cx, int cy, int radius, uint8_t color) {
  int radius_squ = radius * radius;
  for(int y = cy - radius; y <= cy+radius; y++){
    for(int x = cx -radius; x <= cx+radius; x++){
      // Only draw if inside screen bounds
      if(x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT){
        int dx = x-cx;
        int dy = y-cy;
        // Check if inside the circle radius
        if(dx*dx + dy*dy <= radius_squ){
            draw_pixel(x, y, color); // Writes to back_buffer via draw_pixel
        }   
      }
    }
  }
}

// You would also include your draw_char, draw_string, and draw_string_wrapped 
// functions here, ensuring they all call the corrected draw_pixel.
// ...

void render_game(const GameState *game) {
    // 1. CLEAR: Clear the hidden buffer for the new frame.
    clear_buffer(BLACK); 

    // 2. DRAW: Draw all game entities to the back buffer.

    // A. Draw Players
    // Note: Assuming you correctly calculate the size of dynamic arrays
    // For fixed-size arrays (like MAXPLAYERS=2), use the constant:
    for(int i = 0; i < MAXPLAYERS; i++){
        const Player *player = &game->players[i];
        draw_circle(player->x_pos, player->y_pos, (int)player->radius, player->color);
    }

    // B. Draw Food
    for(int i = 0; i < MAXFOOD; i++){
        const Food *food = &game->crumbs[i];
        // Using Food->nutrition as a color is common in this type of game
        draw_circle(food->x_pos, food->y_pos, food->radius, food->nutrition);
    }

    // C. Draw AI
    for(int i = 0; i < MAXAI; i++){
        const Ai *ai = &game->ais[i];
        draw_circle(ai->x_pos, ai->y_pos, (int)ai->radius, ai->color);
    }

    // D. Draw UI/HUD (e.g., score, pause message)
    if (get_switch_state(4) == 1) {
        draw_filled_rect(100, 100, 120, 40, WHITE);
        draw_string_wrapped(110, 110, "PAUSED", BLACK, 100);
    }
    
    // 3. SWAP: Copy the complete, new frame from the back buffer to the screen.
    swap_buffers();
}

// In render.c

void draw_string_wrapped(int x, int y, const char *str, uint8_t color, int max_width){
    int current_x = x;
    int current_y = y;
    int word_start_index = 0;
    int i = 0;

    // The total pixel width of one character (e.g., 5 + 1 = 6)
    int char_width = FONT_WIDTH + CHAR_SPACING;
    int line_height = FONT_HEIGHT + LINE_SPACING;

    // Calculate maximum right boundary
    int max_right_bound = x + max_width;

    while(str[i] != '\0'){
        int word_end_index = i;
        
        // 1. Find the end of the current word
        while(str[word_end_index] != '\0' &&
              str[word_end_index] != ' ' &&
              str[word_end_index] != '\n'){
                word_end_index++;
        }

        int word_len = word_end_index - word_start_index;
        int word_width = word_len * char_width;

        // 2. Check for word wrap: Does the word fit on the current line?
        if(current_x + word_width > max_right_bound && current_x > x){
            // If it doesn't fit and we're not at the very start of the line,
            // break the line and repeat the check for this word.
            current_x = x;
            current_y += line_height;
            
            // Check vertical boundary before redrawing word
            if(current_y >= SCREEN_HEIGHT - FONT_HEIGHT){
                break;
            }
            // Continue forces the loop to restart the word check
            continue; 
        }

        // 3. Handle explicit newline character
        if(str[i] == '\n'){
            current_x = x;
            current_y += line_height;
            word_start_index = i + 1;
            i++;
            
            // Check vertical boundary after newline
            if(current_y >= SCREEN_HEIGHT - FONT_HEIGHT){
                break;
            }
            continue;
        }

        // 4. Draw current character
        // Only draw if within horizontal and vertical bounds
        if(current_x + FONT_WIDTH <= SCREEN_WIDTH && current_y + FONT_HEIGHT <= SCREEN_HEIGHT){
            draw_char(current_x, current_y, str[i], color);
        }

        current_x += char_width;
        i++;

        // 5. Check if we've reached the end of the word
        if(i == word_end_index){
            // If the next character is a space, draw it
            if(str[i] == ' '){
                // Draw the space character (optional, often a blank space is sufficient)
                // If you want to explicitly draw it (e.g., for background clearing):
                // draw_char(current_x, current_y, ' ', color); 
                
                current_x += char_width; // Advance past the space
                i++;
            }

            // Set the start index for the next word
            word_start_index = i;
        }
        
        // 6. Final vertical boundary check for game screen
        if(current_y >= SCREEN_HEIGHT - FONT_HEIGHT){
             break;
        }
    }
}

// In render.c

void draw_msg(char* ch){
    int box_x = 80;
    int box_y = 60;
    int box_width = 160;
    int box_height = 120;
    int padding = 5; // Small padding inside the box

    draw_filled_rect(box_x, box_y, box_width, box_height, BLACK); // Draw the box

    // Calculate max_width for the text to fit inside the box with padding
    int text_max_width = box_width - (2 * padding);
    int text_x = box_x + padding;
    int text_y = box_y + padding;

    // Draw the text inside the padded box
    draw_string_wrapped(text_x, text_y, ch, WHITE, text_max_width);

    // CRITICAL: Call swap_buffers() after drawing the message
    swap_buffers();
}

// In render.c (Place before draw_string_wrapped)

void draw_char(int x, int y, char ch, uint8_t color) {
    // Check if the character is in the supported ASCII range (32 to 127)
    // If not, default to the space character (index 0)
    if (ch < 32 || ch > 127) {
        ch = 32; 
    }
    
    // Calculate the index in the font array (ASCII 32 (' ') is index 0)
    int font_index = ch - 32; 
    
    // FONT_HEIGHT is 7
    for (int row = 0; row < FONT_HEIGHT; row++) {
        // Get the 5-bit column data for this row
        uint8_t row_data = font_5x7[font_index][row]; 

        // FONT_WIDTH is 5
        for (int col = 0; col < FONT_WIDTH; col++) {
            // Check if the bit is set (if the pixel should be drawn)
            // 0x10 is 10000 binary. Shifting it right checks columns 4, 3, 2, 1, 0.
            if (row_data & (0x10 >> col)) { 
                // Draw the pixel at the correct screen coordinate
                // draw_pixel handles boundary checking and writes to the back_buffer
                draw_pixel(x + col, y + row, color);
            }
        }
    }
}