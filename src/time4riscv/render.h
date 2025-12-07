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
#define FONT_HEIGHT 7
#define FONT_WIDTH 5
#define LINE_SPACING 1
#define CHAR_SPACING 1

extern volatile char *VGA;

extern const uint8_t font_5x7[96][7];

// void vga_int();

void clear_screen();

void draw_circle (int cx, int cy, int radius, int color);

void draw_filled_rect(int x, int y, int width, int height, int color);

void draw_pixel(int x, int y, int color);

void draw_chars(int x, int y, char ch, int color);

void draw_string(int x, int y, const char *str, int color);

void draw_string_wrapped(int x, int y, const char *str, int color, int max_width);

void draw_string_centered(int y, const char *str, int color);

void render_game(GameState *game);

#endif