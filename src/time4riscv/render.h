// =========================================================
// In render.h (or a separate vga_config.h)
// =========================================================

#include <stdint.h>

// Assuming standard 320x240, 8-bit color depth (256 colors)
#define SCREEN_WIDTH        320
#define SCREEN_HEIGHT       240
#define VGA_BUFFER_SIZE     (SCREEN_WIDTH * SCREEN_HEIGHT)

// Base address of the VGA memory-mapped I/O (this is system-specific)
// Assuming VGA_BASE is 0x04000000 or similar, based on your previous code
#define VGA_BASE            0x04000000 
#define VGA                 ((volatile uint8_t*) VGA_BASE)

// Font constants (from your original code)
#define FONT_WIDTH          5
#define FONT_HEIGHT         7
#define CHAR_SPACING        1
#define LINE_SPACING        1

// Simple Color Palette (8-bit)
#define BLACK               0
#define WHITE               255
#define RED                 224 // High bits set
#define GREEN               28
#define BLUE                3

// Add any other necessary struct definitions (e.g., Player, Food, GameState)
// ...

// Function prototypes
void clear_buffer(uint8_t color);
void swap_buffers(void);
void draw_pixel(int x, int y, uint8_t color);
void draw_filled_rect(int x, int y, int width, int height, uint8_t color);
void draw_char(int x, int y, char ch, uint8_t color);
void draw_string_wrapped(int x, int y, const char *str, uint8_t color, int max_width);

// The main rendering loop function
void render_game(const GameState *game);