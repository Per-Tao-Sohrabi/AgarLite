#ifndef ENTITIES_H  // 1. Check if the guard symbol is NOT defined
#define ENTITIES_H  // 2. Define the guard symbol

#define Food_count 50
#define MIN_RADIUS 10
typedef struct {
    int x_pos;
    int y_pos;
    int radius;
    int hp;
    int dx;
    int dy;
    int velocity;
    int color;
} Player;

typedef struct {
    int x_pos;
    int y_pos;
    int radius;
    int nutrition;
} Food;

#endif // ENTITIES_H  // 3. Close the conditional block