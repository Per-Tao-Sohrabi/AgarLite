#ifndef RENDER_H
#define RENDER_H

#include "Entities.h"
#include "GameState.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define VGA_BASE 0x08000000
#define MAGRIN 10
#define TOTAL_WIDTH 300
#define FONT_HEIGHT 7
#define FONT_WIDTH 5
#define LINE_SPACING 1
#define CHAR_SPACING 1
#define WHITE 255
#define COLOR_BLACK 0
#define COLOR_BRIGHT_GREEN 0x1C  // RGB332 000 111 00
#define MSG_WIDTH 250
#define MSG_HEIGHT 120

// #define VGA_BUFFER_SIZE SCREEN_WIDTH*SCREEN_HEIGHT
#define BUFFER_SIZE (SCREEN_WIDTH*SCREEN_HEIGHT)
extern volatile char *VGA;
extern char back_buffer[BUFFER_SIZE];

extern char frame_buffer1[SCREEN_HEIGHT*SCREEN_WIDTH]; // Är detta inte bara buffer size?
extern char frame_buffer2[SCREEN_WIDTH*SCREEN_HEIGHT];

extern char *current_draw_buffer;
extern char *current_display_buffer;

extern const uint8_t font_5x7[96][7];


void clear_screen();

void init_buffers();

void copy_to_vga(char *src);

void swap_buffers();

void clear_current_buffer();

void draw_circle (int cx, int cy, int radius, int color);

void draw_circle_to_buffer(char *buffer, int cx, int cy, int radius, int color);

void draw_filled_rectangle(int x, int y, int width, int height, int color);

void draw_horizontal_line(char *buffer, int x_min, int x_max, int y, int color);

void draw_pixel_to_buffer(char *buffer, int x, int y, int color);

void draw_pixel(int x, int y, int color);

void draw_char(int x, int y, char ch, int color);

void draw_string(int x, int y, const char *str, int color);

void draw_string_centered(int y, const char *str, int color);

void draw_string_wrapped(int x, int y, const char *str, int color, int max_width);

void render_game(GameState *game);

void draw_msg(char* ch);

void draw_hud(GameState *game);

void int_to_string(int num, char *str);

void draw_rounded_rectangle(int x, int y, int width, int height, int radius, int color);

void draw_pause_box(const char* msg, const char* button, void* callback);

void draw_confirm_box(const char* msg, int selected_option);

#endif