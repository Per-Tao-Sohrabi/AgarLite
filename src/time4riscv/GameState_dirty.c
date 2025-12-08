// GameState_dirty.c - 扩展GameState支持脏矩形
#include "GameState.h"
#include "dirty_rect.h"

// 保存上一帧的位置
static int last_player_x[2] = {0, 0};
static int last_player_y[2] = {0, 0};
static int last_player_radius[2] = {0, 0};

static int last_ai_x[10] = {0};  // 假设最多10个AI
static int last_ai_y[10] = {0};
static int last_ai_radius[10] = {0};

// 更新游戏状态并标记脏矩形
bool GameState_update_with_dirty(GameState *game, int *input_vector, DirtyRectManager *dirty_mgr) {
    // 1. 标记上一帧位置的区域为脏
    for (int i = 0; i < 2; i++) {
        if (last_player_radius[i] > 0) {
            dirty_rect_mark_circle(dirty_mgr, 
                last_player_x[i], last_player_y[i], 
                last_player_radius[i]);
        }
    }
    
    for (int i = 0; i < 10; i++) {
        if (last_ai_radius[i] > 0) {
            dirty_rect_mark_circle(dirty_mgr,
                last_ai_x[i], last_ai_y[i],
                last_ai_radius[i]);
        }
    }
    
    // 2. 更新游戏状态（你的原有逻辑）
    bool game_over = GameState_update(game, input_vector);
    
    // 3. 标记新位置的区域为脏
    for (int i = 0; i < 2; i++) {
        dirty_rect_mark_circle(dirty_mgr,
            game->players[i].x_pos, game->players[i].y_pos,
            game->players[i].radius);
            
        // 保存当前位置供下一帧使用
        last_player_x[i] = game->players[i].x_pos;
        last_player_y[i] = game->players[i].y_pos;
        last_player_radius[i] = game->players[i].radius;
    }
    
    // 标记AI
    int antal_ai = sizeof(game->ais)/sizeof(game->ais[0]);
    for (int i = 0; i < antal_ai && i < 10; i++) {
        dirty_rect_mark_circle(dirty_mgr,
            game->ais[i].x_pos, game->ais[i].y_pos,
            game->ais[i].radius);
            
        last_ai_x[i] = game->ais[i].x_pos;
        last_ai_y[i] = game->ais[i].y_pos;
        last_ai_radius[i] = game->ais[i].radius;
    }
    
    // 4. 标记食物变化的区域（如果食物被吃掉后重生）
    // 这里需要根据你的食物更新逻辑来添加
    
    return game_over;
}