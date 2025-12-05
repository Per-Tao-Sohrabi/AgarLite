#ifndef ENTITIES_H  // 1. Check if the guard symbol is NOT defined
#define ENTITIES_H  // 2. Define the guard symbol

char* APPLE = "apple";
char* BANANA = "banana";
char* PEAR = "pear";
char* GRAPE = "grape";

typedef struct {
    int x_pos;
    int y_pos;
    int hp;
    int color;
    int velocity;
    int dx, dy;
    int radius;
    int id;
} Player;

typedef struct {
    int x_pos;
    int y_pos;
    int nutrition;
    enum { APPLE, BANANA, PEAR, GRAPE } type;
    int id;
} Food;

typedef struct {
    int x_pos;
    int y_pos;
    int hp;
    int color;
    int velocity;
    int dx, dy;
    int radius;
    int id;
} Ai;

#endif // ENTITIES_H  // 3. Close the conditional block