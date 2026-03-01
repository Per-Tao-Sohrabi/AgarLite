#ifndef MATH_TOOLS_H  // 1. Check if the guard symbol is NOT defined
#define MATH_TOOLS_H  // 2. Define the guard symbol

int rand();
int rand_range(int min, int max);

// Q16.16 fixed-point helpers
#define FP_SHIFT     16
#define FP_ONE       (1 << FP_SHIFT)          // 65536 = 1.0
#define FP_HALF      (1 << (FP_SHIFT - 1))    // 32768 = 0.5
#define INT_TO_FP(x) ((x) << FP_SHIFT)        // int -> fixed-point
#define FP_TO_INT(x) ((x) >> FP_SHIFT)        // fixed-point -> int (truncate)
#define FP_MUL(a,b)  (((a) >> 8) * ((b) >> 8)) // fixed * fixed (shift-split to avoid overflow)
// typedef struct {
//     int keys[100];
//     int values[100];
//     int size;
// } Dictionary;
// void Dict_insert(Dictionary* dict, int key, int value);
// int Dict_get_value(Dictionary* dict, int key);
// int Dict_get_key(Dictionary* dict, int value);
// int Dict_set_value(Dictionary* dict, int key, int value);

#endif