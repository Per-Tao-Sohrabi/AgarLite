// render.h
#ifndef RENDER_H
#define RENDER_H

#include "Entities.h"
#include "GameState.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// ========== 降低分辨率 ==========
#define SCREEN_WIDTH 160      // 从320减半
#define SCREEN_HEIGHT 120     // 从240减半
#define VGA_BASE 0x08000000

// 字体和UI常量（相应调整）
#define MARGIN 5
#define TOTAL_WIDTH 150
#define FONT_HEIGHT 7
#define FONT_WIDTH 5
#define LINE_SPACING 1
#define CHAR_SPACING 1
#define WHITE 255
#define MSG_WIDTH 125         // 减半
#define MSG_HEIGHT 60         // 减半

// ========== 伪双缓冲 ==========
// 只使用一个小的后缓冲区，存储最近变化的区域
#define BACK_BUFFER_SIZE (SCREEN_WIDTH * 32)  // 只缓冲32行
#define BUFFER_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT)

extern volatile char *VGA;
extern char back_buffer[BACK_BUFFER_SIZE];  // 小后缓冲区

// 函数声明
void clear_screen(void);
void init_buffers_small(void);
void render_game_pseudo(GameState *game);
void draw_circle_pseudo(int cx, int cy, int radius, int color);
void update_partial_screen(void);

#endif // RENDER_H