#ifndef GAMESTATE_DIRTY_H
#define GAMESTATE_DIRTY_H

#include "GameState.h"
#include "dirty_rect.h"

// 函数声明
bool GameState_update_with_dirty(GameState *game, int *input_vector, DirtyRectManager *dirty_mgr);
void init_last_positions(void);
void save_player_last_position(int player_index, int x, int y, int radius);
void save_ai_last_position(int ai_index, int x, int y, int radius);

// 获取上一帧位置的函数（用于测试和调试）
void get_player_last_position(int player_index, int *x, int *y, int *radius);
void get_ai_last_position(int ai_index, int *x, int *y, int *radius);

#endif // GAMESTATE_DIRTY_H