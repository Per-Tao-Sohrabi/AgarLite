// dirty_rect.h
#ifndef DIRTY_RECT_H
#define DIRTY_RECT_H

#include <stdint.h>
#include <stdbool.h>

// 屏幕尺寸
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define VGA_BASE 0x08000000

// 脏矩形结构
typedef struct {
    int x1, y1;     // 左上角
    int x2, y2;     // 右下角
    bool active;    // 是否激活
} DirtyRect;

// 脏矩形管理器
typedef struct {
    DirtyRect rects[16];  // 最多16个脏矩形
    int count;            // 当前有效数量
    uint8_t *back_buffer; // 后缓冲区（可选）
} DirtyRectManager;

// 函数声明
void dirty_rect_init(DirtyRectManager *manager);
void dirty_rect_mark(DirtyRectManager *manager, int x, int y, int width, int height);
void dirty_rect_mark_circle(DirtyRectManager *manager, int cx, int cy, int radius);
void dirty_rect_clear_all(DirtyRectManager *manager);
void dirty_rect_render(GameState *game, DirtyRectManager *manager);
void dirty_rect_merge(DirtyRectManager *manager);  // 合并重叠的矩形

#endif // DIRTY_RECT_H