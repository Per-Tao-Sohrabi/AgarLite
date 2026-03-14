#include "render.h"
#include "Entities.h"
#include "GameState.h"
#include "math_tools.h"

// #include "graphics.h"

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
    }
}

/* VGA Buffer 

*/
volatile char *VGA = (volatile char*) VGA_BASE;

/* Align buffers to 4 bytes for 32-bit operations
Motivation:
- standar c char array have allignment of 1.
- Buffer point in our case cast to unit32_t: allows for writing 32 bits at once.
- Limits to writing words at adressess dividible by 4, mitigating potential crashes common to some systems. 
*/ 
char frame_buffer1[SCREEN_WIDTH*SCREEN_HEIGHT] __attribute__((aligned(4))); // GCC compiler extension to manually set allignemnt to 4 bytes.
char frame_buffer2[SCREEN_WIDTH*SCREEN_HEIGHT] __attribute__((aligned(4)));
 
/* Double buffering

*/
char *current_draw_buffer; // Adress type
char *current_display_buffer; // Adress type

/* Initialize buffers
@brief
    Blanks at the VGA buffer memory locations.
Calls:
    copy_to_vga()
@params
    void: No arguments taken.

@return
    void: No return value.
*/
void init_buffers() {
    // init buffers
    current_draw_buffer = frame_buffer1;    // initalizes to buffer one (height X width)
    current_display_buffer = frame_buffer2; // initalizes to buffer two (width X height)
    
    // clear both buffers
    for (int i = 0; i < BUFFER_SIZE; i++) { // Itterate throguht the frame buffer.
        frame_buffer1[i] = 0;
        frame_buffer2[i] = 0;
    }

    // init. vga
    copy_to_vga(current_draw_buffer);       // Paste in VGA
}

/* Copy to VGA
@brief
    Copies the contents of the current draw buffer to the VGA buffer.
@params
    void: No arguments taken.

@return
    void: No return value.
*/
void copy_to_vga(char *buffer_root){
    volatile uint32_t *vga_ptr = (volatile uint32_t*)VGA;   // The VGA pointer is copied to a local (volatile unit_32_t) pointer.
    uint32_t *buf_ptr = (uint32_t*)buffer_root;             // A pointer to the buffer root address.
    
    // Copy 4 bytes at a time
    int num_words = BUFFER_SIZE / 4;
    for(int i = 0; i < num_words; i++){
        vga_ptr[i] = buf_ptr[i];
    }
}

/* Swap buffers
@brief
    Swaps the contents of the current draw buffer and the current display buffer.

Calls:
    copy_to_vga()

@params
    void: No arguments taken.

@return
    void: No return value.
*/
void swap_buffers(){
  copy_to_vga(current_draw_buffer); // Draw draw buffer values to VGA

  char *temp = current_draw_buffer;                 // Pass draw buffer adress into temporary pointer.
  current_draw_buffer = current_display_buffer;     // Update draw buffer memory values to those of the current display buffer.
  current_display_buffer = temp;                    // Update display buffer memory values to those of the temporary pointer.
}

/* Clear current buffer
@brief
    Clears the current draw buffer.

@params
    void: No arguments taken.

@return
    void: No return value.
*/
void clear_current_buffer(){     
    uint32_t *buffer_root = (uint32_t*)current_draw_buffer;
    // Clear 4 bytes at a time
    int num_words = BUFFER_SIZE / 4;
    for(int i = 0; i < num_words; i++){
        buffer_root[i] = 0;
    }
}



// ===========================================================
// ================= Primitive shapes ========================
// ===========================================================

/* Draw pixel to buffer
@brief
    Draws a pixel to the buffer.

@params
    buffer: The buffer to draw the pixel to.
    x: The x-coordinate of the pixel.
    y: The y-coordinate of the pixel.
    color: The color of the pixel.

@return
    void: No return value.
*/
void draw_pixel_to_buffer(char *buffer_root, int x, int y, int color){
    // Edge cases
    if(x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT){
        return;
    }

    // Calculate offset
    int offset = y * SCREEN_WIDTH + x;
    buffer_root[offset] = color;
}

/* Draw horizontal line
@brief
    Draws a horizontal line to the buffer.

@params
    buffer: The buffer to draw the line to.
    x_min: The minimum x-coordinate of the line.
    x_max: The maximum x-coordinate of the line.
    y: The y-coordinate of the line.
    color: The color of the line.

@return
    void: No return value.
*/
void draw_horizontal_line(char *line_root, int x_min, int x_max, int y, int color){
   // Edge cases
    if (y < 0 || y > SCREEN_HEIGHT) {
        return; // No wrap around
    }
    if (x_min < 0) x_min = 0;
    if (x_max > SCREEN_WIDTH) x_max = SCREEN_WIDTH;

    // Draw line
    for (int x = x_min; x < x_max; x++) {
        draw_pixel_to_buffer(line_root, x, y, color);
    }
}

/*
@brief
    Draws a circle to the buffer.
    Itterates vertically, calculating the width of the circle at each y-coordinate, and drawing their corresponding lines to the buffer.

@params
    buffer: The buffer to draw the circle to.
    cx: The x-coordinate of the center of the circle.
    cy: The y-coordinate of the center of the circle.
    radius: The radius of the circle.
    color: The color of the circle.

@return
    void: No return value.
*/
void draw_circle_to_buffer(char *buffer_root, int cx, int cy, int radius, int color) {
    // Pre-calculate radius squared
    int radius_squared = radius * radius;
    
    // Calculate y-range for the circle (no wrap around)
    int min_y = cy - radius;
    int max_y = cy + radius;
    
    // Edge cases
    if (min_y < 0) min_y = 0;
    if (max_y >= SCREEN_HEIGHT) max_y = SCREEN_HEIGHT - 1;  // Stops circles from becoming cut off at the top or bottom.
    
    // Draw circle
    for (int y = min_y; y <= max_y; y++) {
        int dy = y - cy;
        // Optimize: Calculate x width using integer square root
        // x = sqrt(r^2 - dy^2)
        int dx_width = int_sqrt(radius_squared - dy*dy);
        
        int min_x = cx - dx_width;
        int max_x = cx + dx_width;
        
        if (min_x < 0) min_x = 0;
        if (max_x >= SCREEN_WIDTH) max_x = SCREEN_WIDTH - 1;
        
        // Draw horizontal line
        draw_horizontal_line(buffer_root, min_x, max_x, y, color);
    }
}

/* Draw circle (WHY???)
@brief
    Draws a circle to the buffer.

@params
    cx: The x-coordinate of the center of the circle.
    cy: The y-coordinate of the center of the circle.
    radius: The radius of the circle.
    color: The color of the circle.

@return
    void: No return value.
*/
void draw_circle(int cx, int cy, int radius, int color) {
    draw_circle_to_buffer(current_draw_buffer, cx, cy, radius, color);
}

/* Draw filled rectangle
@brief
    Draws a filled rectangle to the buffer.
    Itterates on the Y axis and draws horizontal lines.

@params
    x: The x-coordinate of the top-left corner of the rectangle.
    y: The y-coordinate of the top-left corner of the rectangle.
    width: The width of the rectangle.
    height: The height of the rectangle.
    color: The color of the rectangle.

@return
    void: No return value.
*/
void draw_filled_rectangle(int x, int y, int width, int height, int color) {
    // Iterate over the vertical range
    int start_y = y;
    int end_y = y + height;

    // Clip to screen boundaries
    if (start_y < 0) start_y = 0;
    if (end_y > SCREEN_HEIGHT) end_y = SCREEN_HEIGHT;

    for (int current_y = start_y; current_y < end_y; current_y++) {
        // Draw a horizontal line for each row
        draw_horizontal_line(current_draw_buffer, x, x + width, current_y, color);
    }
}

/* Draw pixel
@brief
    Draws a pixel to the buffer.

@params
    x: The x-coordinate of the pixel.
    y: The y-coordinate of the pixel.
    color: The color of the pixel.

@return
    void: No return value.
*/
void draw_pixel(int x, int y, int color){
    draw_pixel_to_buffer(current_draw_buffer, x, y, color);
}

/* Draw character
@brief
    Draws a character to the buffer.

@params
    x: The x-coordinate of the top-left corner of the character.
    y: The y-coordinate of the top-left corner of the character.
    ch: The character to draw.
    color: The color of the character.

@return
    void: No return value.
*/
void draw_char(int x, int y, char ch, int color){
    if(ch < 32 || ch > 126){
        return;
    }
    
    int index = ch - 32;
    for(int row = 0; row < 7; row++){
        int current_y = y + row;
        if(current_y < 0 || current_y >= SCREEN_HEIGHT) continue;

        uint8_t line_data = font_5x7[index][row];
        for(int col = 0; col < 5; col++){
            if(line_data & (0x10 >> col)){
                int current_x = x + col;
                if(current_x >= 0 && current_x < SCREEN_WIDTH){
                    draw_pixel(current_x, current_y, color);
                }
            }
        }
    }
}

/* Draw string
@brief
    Draws a string to the buffer.

@params
    x: The x-coordinate of the top-left corner of the string.
    y: The y-coordinate of the top-left corner of the string.
    str: The string to draw.
    color: The color of the string.

@return
    void: No return value.
*/
void draw_string(int x, int y, const char *str, int color){
    int start_x = x;
    int original_y = y;

    while(*str){
        if(*str == '\n'){
            y += FONT_HEIGHT + LINE_SPACING;
            x = start_x;

        }else if(*str == '\t'){
            x += 4*(FONT_WIDTH + CHAR_SPACING);
        }else if(*str == '\b'){
            x -= FONT_WIDTH + CHAR_SPACING;
            draw_filled_rectangle(x, y, FONT_WIDTH, FONT_HEIGHT, 0);
        }
        else{
            draw_char(x, y, *str, color);
            x += FONT_WIDTH + CHAR_SPACING;
        }
        str++;
    }

}

// void draw_string_centered(int y, const char *str, int color){
//     if(y < 0 || y >= SCREEN_HEIGHT) return;

//     int len = 0;
//     const char* temp = str;

//     while(*temp && *temp != '\n'){
//         len++;
//         temp++;
//     }

//     int total_width = len*(FONT_WIDTH + CHAR_SPACING);
//     int x = (SCREEN_WIDTH-total_width)/2;
//     draw_string(x, y, str, color);
// }

void draw_string_wrapped(int x, int y, const char *str, int color, int max_width){
    if(x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) return;

    int current_x = x;
    int current_y = y;
    int char_width = FONT_WIDTH + CHAR_SPACING;

    const char *p = str;

    while(*p != '\0'){
        // om y är ut av bottom, stop draw
        if(current_y >= SCREEN_HEIGHT)break;
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

        // check if need break line
        if(current_x > x && current_x + word_width > x + max_width){
            current_x = x;
            current_y += FONT_HEIGHT + LINE_SPACING;

            if(current_y >= SCREEN_HEIGHT - FONT_HEIGHT){
                break;
            }
            continue;
        }

        //draw word
        for(const char *ch = word_start; ch < word_end; ch++){
            if(current_x < SCREEN_WIDTH && current_y < SCREEN_HEIGHT){
                draw_char(current_x, current_y, *ch, color);
            }
            current_x += char_width;
        }

        p = word_end;

        // draw space
        if(*p == ' '){
            if(current_x < SCREEN_WIDTH && current_y < SCREEN_HEIGHT){
                draw_char(current_x, current_y, ' ', color);
            }
            current_x += char_width;
            p++;
        }
    }
}

void int_to_string(int num, char *str){
    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    int i = 0;
    bool is_negative = false;

    if (num < 0) {
        is_negative = true;
        num = -num;
    }

    // Extract digits in reverse order
    while (num > 0) {
        str[i++] = (num % 10) + '0';
        num /= 10;
    }

    if (is_negative) {
        str[i++] = '-';
    }

    str[i] = '\0';

    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

void draw_hud(GameState *game){
    char buffer[32]; // Temporary buffer for string conversions

    // --- Global Stats (Top Center) ---
    draw_string(130, 5, "Diff:", WHITE);
    int_to_string(game->difficulty, buffer);
    draw_string(165, 5, buffer, WHITE);

    int food_count = 0;
    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (game->entities[i].is_active && game->entities[i].type == ENTITY_FOOD) {
            food_count++;
        }
    }
    
    draw_string(130, 25, "Food:", WHITE);
    int_to_string(food_count, buffer);
    draw_string(165, 25, buffer, WHITE);

    if (game->num_players == 1) {
        // --- Single Player ---
        Entity* p1 = &game->entities[0];
        if (p1->is_active) {
            draw_string(5, 5, "Velocity: ", WHITE);
            int_to_string(FP_TO_INT(p1->vel_fp), buffer);
            draw_string(65, 5, buffer, WHITE);

            draw_string(5, 15, "Area: ", WHITE);
            int_to_string(p1->area, buffer);
            draw_string(55, 15, buffer, WHITE);
        }
    } else if (game->num_players >= 2) {
        // --- Multiplayer ---
        
        // Player 1 (Top Left)
        Entity* p1 = &game->entities[0];
        if (p1->is_active) {
            draw_string(5, 5, "Player 1:", WHITE);

            draw_string(5, 15, "Lives: ", WHITE);
            int_to_string(p1->lives, buffer);
            draw_string(55, 15, buffer, WHITE);

            draw_string(5, 25, "Velocity: ", WHITE);
            int_to_string(FP_TO_INT(p1->vel_fp), buffer);
            draw_string(65, 25, buffer, WHITE);

            draw_string(5, 35, "Area: ", WHITE);
            int_to_string(p1->area, buffer);
            draw_string(45, 35, buffer, WHITE);
        }

        // Player 2 (Top Right, starting at x=250 to avoid center HUD)
        Entity* p2 = &game->entities[1];
        if (p2->is_active) {
            draw_string(250, 5, "Player 2:", WHITE);

            draw_string(250, 15, "Lives: ", WHITE);
            int_to_string(p2->lives, buffer);
            draw_string(300, 15, buffer, WHITE);

            draw_string(250, 25, "Velocity: ", WHITE);
            int_to_string(FP_TO_INT(p2->vel_fp), buffer);
            draw_string(310, 25, buffer, WHITE);

            draw_string(250, 35, "Area: ", WHITE);
            int_to_string(p2->area, buffer);
            draw_string(290, 35, buffer, WHITE);
        }
    }

    draw_horizontal_line(current_draw_buffer, 0, SCREEN_WIDTH, 48, WHITE);
}

// ===========================================================
// =================== Game rendering ========================
// ===========================================================

/* Render game to buffer*/
void render_game(GameState *game) {
    clear_current_buffer(); // Clear, then draw to the buffer. 
    for (int i = 0; i < MAX_ENTITIES; i++) {
        Entity* e = &game->entities[i];
        if (!e->is_active) continue;
        draw_circle(FP_TO_INT(e->x_fp), FP_TO_INT(e->y_fp), e->radius, e->color);
        if(e->type == ENTITY_PLAYER){
            draw_circle(FP_TO_INT(e->x_fp), FP_TO_INT(e->y_fp), e->radius/3, 0);
        }
    }
    draw_hud(game);
    swap_buffers();
}

void draw_msg(char* ch) {
    // räkna msg_box postion, att vara inner i skärmen
    int msg_x = 35;
    int msg_y = 60;
    int msg_width = MSG_WIDTH;
    int msg_height = MSG_HEIGHT;
    
    // säkertställa att box är inner i skärmen
    if(msg_x + msg_width > SCREEN_WIDTH) msg_width = SCREEN_WIDTH - msg_x;
    if(msg_y + msg_height > SCREEN_HEIGHT) msg_height = SCREEN_HEIGHT - msg_y;
    if(msg_x < 0) msg_x = 0;
    if(msg_y < 0) msg_y = 0;
    
    // clear box position
    draw_filled_rectangle(msg_x, msg_y, msg_width, msg_height, 0);
    
    // rita information
    draw_string_wrapped(msg_x, msg_y, ch, 255, msg_width);
    
    // buttar buffer
    swap_buffers();
}

void draw_rounded_rectangle(int x, int y, int width, int height, int radius, int color) {
    // Fill middle parts
    draw_filled_rectangle(x + radius, y, width - 2*radius, height, color); // horizontal
    draw_filled_rectangle(x, y + radius, width, height - 2*radius, color); // vertical
    
    // Corners
    draw_circle(x + radius, y + radius, radius, color); // Top-left
    draw_circle(x + width - 1 - radius, y + radius, radius, color); // Top-right
    draw_circle(x + radius, y + height - 1 - radius, radius, color); // Bottom-left
    draw_circle(x + width - 1 - radius, y + height - 1 - radius, radius, color); // Bottom-right
}

void draw_pause_box(const char* msg, const char* button, void* callback) {
    int msg_x = 35;
    int msg_y = 60;
    int msg_width = MSG_WIDTH;
    int msg_height = MSG_HEIGHT;
    
    // Draw white border, black interior
    draw_rounded_rectangle(msg_x, msg_y, msg_width, msg_height, 5, WHITE);
    draw_rounded_rectangle(msg_x + 2, msg_y + 2, msg_width - 4, msg_height - 4, 3, COLOR_BLACK);
    
    // Draw msg
    draw_string_wrapped(msg_x + 10, msg_y + 10, msg, WHITE, msg_width - 20);
    
    // Draw button
    if (button) {
        int btn_w = 60;
        int btn_h = 20;
        int btn_x = msg_x + (msg_width - btn_w)/2;
        int btn_y = msg_y + msg_height - 30;
        draw_rounded_rectangle(btn_x, btn_y, btn_w, btn_h, 3, WHITE);
        draw_rounded_rectangle(btn_x+1, btn_y+1, btn_w-2, btn_h-2, 2, COLOR_BLACK);
        draw_string(btn_x + 10, btn_y + 6, button, WHITE);
    }
    swap_buffers();
}

void draw_confirm_box(const char* msg, int selected_option) {
    int msg_x = 35;
    int msg_y = 60;
    int msg_width = MSG_WIDTH;
    int msg_height = MSG_HEIGHT;
    
    // Draw white border, black interior
    draw_rounded_rectangle(msg_x, msg_y, msg_width, msg_height, 5, WHITE);
    draw_rounded_rectangle(msg_x + 2, msg_y + 2, msg_width - 4, msg_height - 4, 3, COLOR_BLACK);
    
    draw_string_wrapped(msg_x + 10, msg_y + 10, msg, WHITE, msg_width - 20);
    
    // Options: 1 = Yes, 0 = No
    int btn_w = 40;
    int btn_h = 20;
    int btn1_x = msg_x + msg_width/2 - btn_w - 10;
    int btn0_x = msg_x + msg_width/2 + 10;
    int btn_y = msg_y + msg_height - 30;
    
    int color1 = (selected_option == 1) ? COLOR_BRIGHT_GREEN : WHITE;
    int color0 = (selected_option == 0) ? COLOR_BRIGHT_GREEN : WHITE;
    
    // Yes button
    draw_rounded_rectangle(btn1_x, btn_y, btn_w, btn_h, 3, color1);
    draw_rounded_rectangle(btn1_x+1, btn_y+1, btn_w-2, btn_h-2, 2, COLOR_BLACK);
    draw_string(btn1_x + 10, btn_y + 6, "Yes", color1);
    
    // No button
    draw_rounded_rectangle(btn0_x, btn_y, btn_w, btn_h, 3, color0);
    draw_rounded_rectangle(btn0_x+1, btn_y+1, btn_w-2, btn_h-2, 2, COLOR_BLACK);
    draw_string(btn0_x + 10, btn_y + 6, "No", color0);
    
    swap_buffers();
}