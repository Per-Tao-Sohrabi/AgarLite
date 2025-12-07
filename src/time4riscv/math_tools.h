#ifndef MATH_TOOLS_H  // 1. Check if the guard symbol is NOT defined
#define MATH_TOOLS_H  // 2. Define the guard symbol

int rand();
int rand_range(int min, int max);
typedef struct {
    int keys[100];
    int values[100];
    int size;
} Dictionary;
void Dict_insert(Dictionary* dict, int key, int value);
int Dict_get_value(Dictionary* dict, int key);
int Dict_get_key(Dictionary* dict, int value);
int Dict_set_value(Dictionary* dict, int key, int value);

#endif