#ifndef ENTITIES_H  // 1. Check if the guard symbol is NOT defined
#define ENTITIES_H  // 2. Define the guard symbol

typedef struct {
    int x_pos;
    int y_pos;
    int hp;
    int color;
    int velocity;
    int dx, dy;
} Player;

typedef struct {
    int x_pos;
    int y_pos;
    int nutrition;
} Food;

typedef struct {
    int x_pos;
    int y_pos;
    int hp;
    int color;
    int velocity;
    int dx, dy;
} Ai;

#endif // ENTITIES_H  // 3. Close the conditional block