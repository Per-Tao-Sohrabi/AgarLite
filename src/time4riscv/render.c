// render.c - 伪双缓冲版本
#include "render.h"
#include "Entities.h"
#include "GameState.h"

const uint8_t font_5x7[96][7] = {
    // ... 保持你的字体数据不变 ...
};

// VGA显示内存
volatile char *VGA = (volatile char*) VGA_BASE;

// 小后缓冲区（只保存最近更新的区域）
char back_buffer[BACK_BUFFER_SIZE];

// 记录哪些行需要更新
uint8_t dirty_rows[SCREEN_HEIGHT];  // 每行一个标志位
int update_start_y = -1;            // 更新区域的起始行
int update_end_y = -1;              // 更新区域的结束行

// 上一帧位置（用于清除旧位置）
static struct {
    int x[2], y[2], r[2];  // 玩家
    int ai_x[10], ai_y[10], ai_r[10];  // AI
} last_pos;

// ========== 初始化 ==========
void clear_screen() {
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        VGA[i] = 0;
    }
}

void init_buffers_small() {
    print("Initializing pseudo double buffer (160x120)\n");
    
    // 清除屏幕
    clear_screen();
    
    // 初始化后缓冲区
    for (int i = 0; i < BACK_BUFFER_SIZE; i++) {
        back_buffer[i] = 0;
    }
    
    // 初始化脏行标记
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        dirty_rows[i] = 0;
    }
    
    // 初始化位置记录
    for (int i = 0; i < 2; i++) {
        last_pos.x[i] = -1;
        last_pos.y[i] = -1;
        last_pos.r[i] = 0;
    }
    for (int i = 0; i < 10; i++) {
        last_pos.ai_x[i] = -1;
        last_pos.ai_y[i] = -1;
        last_pos.ai_r[i] = 0;
    }
    
    print("Pseudo double buffer ready\n");
}

// ========== 标记脏行 ==========
void mark_dirty_row(int y) {
    if (y < 0 || y >= SCREEN_HEIGHT) return;
    
    dirty_rows[y] = 1;
    
    // 更新脏区域范围
    if (update_start_y == -1 || y < update_start_y) {
        update_start_y = y;
    }
    if (update_end_y == -1 || y > update_end_y) {
        update_end_y = y;
    }
}

void mark_dirty_area(int y1, int y2) {
    if (y1 < 0) y1 = 0;
    if (y2 >= SCREEN_HEIGHT) y2 = SCREEN_HEIGHT - 1;
    
    for (int y = y1; y <= y2; y++) {
        mark_dirty_row(y);
    }
}

// ========== 伪双缓冲渲染 ==========
void render_game_pseudo(GameState *game) {
    // 重置脏行标记
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        dirty_rows[i] = 0;
    }
    update_start_y = -1;
    update_end_y = -1;
    
    // 1. 标记需要清除的旧位置区域
    for (int i = 0; i < 2; i++) {
        if (last_pos.r[i] > 0) {
            int y1 = last_pos.y[i] - last_pos.r[i];
            int y2 = last_pos.y[i] + last_pos.r[i];
            mark_dirty_area(y1, y2);
            
            // 在后缓冲区中清除旧位置
            clear_circle_in_backbuffer(last_pos.x[i], last_pos.y[i], last_pos.r[i]);
        }
    }
    
    // 标记AI旧位置
    int antal_ai = sizeof(game->ais)/sizeof(game->ais[0]);
    for (int i = 0; i < antal_ai && i < 10; i++) {
        if (last_pos.ai_r[i] > 0) {
            int y1 = last_pos.ai_y[i] - last_pos.ai_r[i];
            int y2 = last_pos.ai_y[i] + last_pos.ai_r[i];
            mark_dirty_area(y1, y2);
            
            clear_circle_in_backbuffer(last_pos.ai_x[i], last_pos.ai_y[i], last_pos.ai_r[i]);
        }
    }
    
    // 2. 绘制新位置到后缓冲区
    // 绘制食物
    int antal_food = sizeof(game->crumbs) / sizeof(game->crumbs[0]); 
    for(int i = 0; i < antal_food; i++){
        Food food = game->crumbs[i];
        if (food.x_pos >= 0 && food.x_pos < SCREEN_WIDTH &&
            food.y_pos >= 0 && food.y_pos < SCREEN_HEIGHT) {
            // 食物很小，直接标记脏行
            mark_dirty_row(food.y_pos);
            draw_pixel_to_backbuffer(food.x_pos, food.y_pos, food.nutrition);
        }
    }

    // 绘制AI
    for(int i = 0; i < antal_ai; i++){
        Ai ai = game->ais[i];
        draw_circle_pseudo(ai.x_pos, ai.y_pos, ai.radius, ai.color);
        
        // 保存AI位置
        if (i < 10) {
            last_pos.ai_x[i] = ai.x_pos;
            last_pos.ai_y[i] = ai.y_pos;
            last_pos.ai_r[i] = ai.radius;
        }
    }
    
    // 绘制玩家
    int antal_players = sizeof(game->players) / sizeof(game->players[0]); 
    for(int i = 0; i < antal_players; i++){
        Player player = game->players[i];
        draw_circle_pseudo(player.x_pos, player.y_pos, player.radius, player.color);
        
        // 保存玩家位置
        if (i < 2) {
            last_pos.x[i] = player.x_pos;
            last_pos.y[i] = player.y_pos;
            last_pos.r[i] = player.radius;
        }
    }
    
    // 3. 将后缓冲区的脏区域复制到VGA
    if (update_start_y != -1 && update_end_y != -1) {
        update_partial_screen();
    }
}

// ========== 绘制函数（伪双缓冲版本） ==========
void draw_circle_pseudo(int cx, int cy, int radius, int color) {
    if (radius <= 0) return;
    
    // 标记脏区域
    int y1 = cy - radius;
    int y2 = cy + radius;
    mark_dirty_area(y1, y2);
    
    // 在后缓冲区绘制
    int radius_squ = radius * radius;
    int start_y = cy - radius;
    int end_y = cy + radius;
    int start_x = cx - radius;
    int end_x = cx + radius;
    
    if (start_y < 0) start_y = 0;
    if (end_y >= SCREEN_HEIGHT) end_y = SCREEN_HEIGHT - 1;
    if (start_x < 0) start_x = 0;
    if (end_x >= SCREEN_WIDTH) end_x = SCREEN_WIDTH - 1;
    
    for (int y = start_y; y <= end_y; y++) {
        int dy = y - cy;
        int dy_sq = dy * dy;
        int row_offset = y * SCREEN_WIDTH;
        
        for (int x = start_x; x <= end_x; x++) {
            int dx = x - cx;
            if (dx * dx + dy_sq <= radius_squ) {
                // 计算在后缓冲区中的位置
                int buffer_offset = (y % 32) * SCREEN_WIDTH + x;
                back_buffer[buffer_offset] = color;
                
                // 同时标记脏行
                dirty_rows[y] = 1;
            }
        }
    }
}

void clear_circle_in_backbuffer(int cx, int cy, int radius) {
    // 在后缓冲区中清除圆形区域（用黑色绘制）
    draw_circle_pseudo(cx, cy, radius, 0);
}

void draw_pixel_to_backbuffer(int x, int y, int color) {
    if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) return;
    
    // 计算在后缓冲区中的位置
    int buffer_offset = (y % 32) * SCREEN_WIDTH + x;
    back_buffer[buffer_offset] = color;
    
    // 标记脏行
    mark_dirty_row(y);
}

// ========== 更新部分屏幕 ==========
void update_partial_screen() {
    // 只更新脏行
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        if (dirty_rows[y]) {
            int vga_offset = y * SCREEN_WIDTH;
            int buffer_offset = (y % 32) * SCREEN_WIDTH;
            
            // 复制这一行
            for (int x = 0; x < SCREEN_WIDTH; x++) {
                VGA[vga_offset + x] = back_buffer[buffer_offset + x];
            }
        }
    }
}

// ========== 保持兼容性的包装函数 ==========
void render_game(GameState *game) {
    // 调用伪双缓冲版本
    render_game_pseudo(game);
}

void init_buffers() {
    // 调用小缓冲区初始化
    init_buffers_small();
}

// ========== 其他绘图函数需要相应修改 ==========
void draw_filled_rect(int x, int y, int width, int height, int color) {
    // 标记脏区域
    mark_dirty_area(y, y + height - 1);
    
    // 在后缓冲区绘制
    for (int i = 0; i < height; i++) {
        int current_y = y + i;
        if (current_y < 0 || current_y >= SCREEN_HEIGHT) continue;
        
        int buffer_row = (current_y % 32) * SCREEN_WIDTH;
        
        for (int j = 0; j < width; j++) {
            int current_x = x + j;
            if (current_x < 0 || current_x >= SCREEN_WIDTH) continue;
            
            back_buffer[buffer_row + current_x] = color;
        }
    }
}

void draw_char(int x, int y, char ch, int color) {
    if (ch < 32 || ch > 126) return;
    
    // 标记脏区域（字符高度7像素）
    mark_dirty_area(y, y + 6);
    
    int index = ch - 32;
    for (int row = 0; row < 7; row++) {
        int current_y = y + row;
        if (current_y < 0 || current_y >= SCREEN_HEIGHT) continue;
        
        int buffer_row = (current_y % 32) * SCREEN_WIDTH;
        uint8_t line_data = font_5x7[index][row];
        
        for (int col = 0; col < 5; col++) {
            if (line_data & (0x10 >> col)) {
                int current_x = x + col;
                if (current_x >= 0 && current_x < SCREEN_WIDTH) {
                    back_buffer[buffer_row + current_x] = color;
                }
            }
        }
    }
}

// 其他绘图函数类似修改...