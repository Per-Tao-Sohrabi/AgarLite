// dirty_rect.c
#include "dirty_rect.h"
#include "GameState.h"
#include <stdio.h>
#include <string.h>

// VGA显示内存
volatile uint8_t *vga_memory = (volatile uint8_t*)VGA_BASE;

// 后缓冲区（可选，如果内存足够）
static uint8_t back_buffer[SCREEN_WIDTH * SCREEN_HEIGHT];

// 初始化脏矩形系统
void dirty_rect_init(DirtyRectManager *manager) {
    manager->count = 0;
    
    // 初始化所有矩形为无效
    for (int i = 0; i < 16; i++) {
        manager->rects[i].active = false;
    }
    
    // 初始化后缓冲区
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        back_buffer[i] = 0;
    }
    
    // 初始化VGA屏幕
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        vga_memory[i] = 0;
    }
    
    printf("Dirty Rect system initialized\n");
}

// 标记一个矩形区域为脏
void dirty_rect_mark(DirtyRectManager *manager, int x, int y, int width, int height) {
    // 边界检查
    if (x < 0) { width += x; x = 0; }
    if (y < 0) { height += y; y = 0; }
    if (x + width > SCREEN_WIDTH) width = SCREEN_WIDTH - x;
    if (y + height > SCREEN_HEIGHT) height = SCREEN_HEIGHT - y;
    
    if (width <= 0 || height <= 0) return;
    
    // 检查是否有空间
    if (manager->count >= 16) {
        // 尝试合并矩形
        dirty_rect_merge(manager);
        
        if (manager->count >= 16) {
            // 如果还是满的，扩大最后一个矩形
            DirtyRect *last = &manager->rects[manager->count-1];
            if (x < last->x1) last->x1 = x;
            if (y < last->y1) last->y1 = y;
            if (x + width > last->x2) last->x2 = x + width;
            if (y + height > last->y2) last->y2 = y + height;
            return;
        }
    }
    
    // 添加新矩形
    DirtyRect *rect = &manager->rects[manager->count];
    rect->x1 = x;
    rect->y1 = y;
    rect->x2 = x + width;
    rect->y2 = y + height;
    rect->active = true;
    manager->count++;
}

// 标记圆形区域为脏
void dirty_rect_mark_circle(DirtyRectManager *manager, int cx, int cy, int radius) {
    // 圆的外接矩形
    int x = cx - radius - 1;  // 额外1像素边界
    int y = cy - radius - 1;
    int size = 2 * radius + 2;
    
    dirty_rect_mark(manager, x, y, size, size);
}

// 清除所有脏矩形
void dirty_rect_clear_all(DirtyRectManager *manager) {
    manager->count = 0;
    for (int i = 0; i < 16; i++) {
        manager->rects[i].active = false;
    }
}

// 合并重叠的矩形
void dirty_rect_merge(DirtyRectManager *manager) {
    if (manager->count <= 1) return;
    
    bool merged;
    do {
        merged = false;
        
        for (int i = 0; i < manager->count && !merged; i++) {
            for (int j = i + 1; j < manager->count; j++) {
                DirtyRect *a = &manager->rects[i];
                DirtyRect *b = &manager->rects[j];
                
                // 检查是否重叠
                if (a->x1 <= b->x2 && a->x2 >= b->x1 &&
                    a->y1 <= b->y2 && a->y2 >= b->y1) {
                    
                    // 合并两个矩形
                    if (b->x1 < a->x1) a->x1 = b->x1;
                    if (b->y1 < a->y1) a->y1 = b->y1;
                    if (b->x2 > a->x2) a->x2 = b->x2;
                    if (b->y2 > a->y2) a->y2 = b->y2;
                    
                    // 移除被合并的矩形
                    for (int k = j; k < manager->count - 1; k++) {
                        manager->rects[k] = manager->rects[k+1];
                    }
                    manager->count--;
                    manager->rects[manager->count].active = false;
                    
                    merged = true;
                    break;
                }
            }
        }
    } while (merged && manager->count > 1);
}

// 渲染脏矩形区域
void dirty_rect_render(GameState *game, DirtyRectManager *manager) {
    // 如果没有脏矩形，直接返回
    if (manager->count == 0) return;
    
    printf("Rendering %d dirty rects\n", manager->count);
    
    // 合并重叠的矩形
    dirty_rect_merge(manager);
    
    // 处理每个脏矩形
    for (int r = 0; r < manager->count; r++) {
        DirtyRect rect = manager->rects[r];
        
        // 边界检查
        if (rect.x1 < 0) rect.x1 = 0;
        if (rect.y1 < 0) rect.y1 = 0;
        if (rect.x2 > SCREEN_WIDTH) rect.x2 = SCREEN_WIDTH;
        if (rect.y2 > SCREEN_HEIGHT) rect.y2 = SCREEN_HEIGHT;
        
        // 清除该区域（在后缓冲区）
        for (int y = rect.y1; y < rect.y2; y++) {
            int offset = y * SCREEN_WIDTH;
            for (int x = rect.x1; x < rect.x2; x++) {
                back_buffer[offset + x] = 0;
            }
        }
    }
    
    // 重新绘制所有游戏对象到后缓冲区
    // 注意：我们需要检查哪些对象在脏矩形内
    
    // 绘制食物
    int antal_food = sizeof(game->crumbs) / sizeof(game->crumbs[0]); 
    for (int i = 0; i < antal_food; i++) {
        Food food = game->crumbs[i];
        
        // 检查是否在任何脏矩形内
        bool needs_draw = false;
        for (int r = 0; r < manager->count; r++) {
            DirtyRect rect = manager->rects[r];
            if (food.x_pos >= rect.x1 && food.x_pos < rect.x2 &&
                food.y_pos >= rect.y1 && food.y_pos < rect.y2) {
                needs_draw = true;
                break;
            }
        }
        
        if (needs_draw) {
            back_buffer[food.y_pos * SCREEN_WIDTH + food.x_pos] = food.nutrition;
        }
    }
    
    // 绘制AI
    int antal_ai = sizeof(game->ais)/sizeof(game->ais[0]);
    for (int i = 0; i < antal_ai; i++) {
        Ai ai = game->ais[i];
        
        // 检查是否在任何脏矩形内
        for (int r = 0; r < manager->count; r++) {
            DirtyRect rect = manager->rects[r];
            
            // 检查圆形是否与矩形相交
            int closest_x = ai.x_pos;
            if (closest_x < rect.x1) closest_x = rect.x1;
            else if (closest_x > rect.x2 - 1) closest_x = rect.x2 - 1;
            
            int closest_y = ai.y_pos;
            if (closest_y < rect.y1) closest_y = rect.y1;
            else if (closest_y > rect.y2 - 1) closest_y = rect.y2 - 1;
            
            int dx = ai.x_pos - closest_x;
            int dy = ai.y_pos - closest_y;
            
            if (dx*dx + dy*dy <= ai.radius * ai.radius) {
                // 需要绘制
                draw_circle_to_buffer(back_buffer, ai.x_pos, ai.y_pos, 
                                     ai.radius, ai.color);
                break;
            }
        }
    }
    
    // 绘制玩家
    int antal_players = sizeof(game->players) / sizeof(game->players[0]); 
    for (int i = 0; i < antal_players; i++) {
        Player player = game->players[i];
        
        // 检查是否在任何脏矩形内
        for (int r = 0; r < manager->count; r++) {
            DirtyRect rect = manager->rects[r];
            
            // 检查圆形是否与矩形相交
            int closest_x = player.x_pos;
            if (closest_x < rect.x1) closest_x = rect.x1;
            else if (closest_x > rect.x2 - 1) closest_x = rect.x2 - 1;
            
            int closest_y = player.y_pos;
            if (closest_y < rect.y1) closest_y = rect.y1;
            else if (closest_y > rect.y2 - 1) closest_y = rect.y2 - 1;
            
            int dx = player.x_pos - closest_x;
            int dy = player.y_pos - closest_y;
            
            if (dx*dx + dy*dy <= player.radius * player.radius) {
                // 需要绘制
                draw_circle_to_buffer(back_buffer, player.x_pos, player.y_pos,
                                     player.radius, player.color);
                break;
            }
        }
    }
    
    // 将后缓冲区的内容复制到VGA（只复制脏矩形区域）
    for (int r = 0; r < manager->count; r++) {
        DirtyRect rect = manager->rects[r];
        
        for (int y = rect.y1; y < rect.y2; y++) {
            int offset = y * SCREEN_WIDTH;
            for (int x = rect.x1; x < rect.x2; x++) {
                vga_memory[offset + x] = back_buffer[offset + x];
            }
        }
    }
    
    // 清除所有脏矩形
    dirty_rect_clear_all(manager);
}

// 绘制圆形到缓冲区
void draw_circle_to_buffer(uint8_t *buffer, int cx, int cy, int radius, uint8_t color) {
    int radius_squ = radius * radius;
    int min_y = cy - radius;
    int max_y = cy + radius;
    int min_x = cx - radius;
    int max_x = cx + radius;
    
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