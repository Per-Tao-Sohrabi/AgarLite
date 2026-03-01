
#include "math_tools.h"
#include <stdint.h> 

// For fast rand
extern volatile int* timer; 

// State for our Xorshift PRNG (initialized to a non-zero value)
static uint32_t xorshift_state = 1; 

// Define the correct index for the current count register
#define TIMER_COUNT_REG 1

int rand() {
    uint32_t x = xorshift_state;
    
    // Pure Xorshift operations (only fast XOR and Shift)
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    
    xorshift_state = x;

    // Mask to the required range [0, 32767]
    return (int)(x & 0x7FFF); 
}

int rand_range(int min, int max) {
    return min + rand() % (max - min + 1);
}

// void Dict_insert(Dictionary* dict, int key, int value) {
//     for (int i = 0; i < dict->size; i++) {
//         if (dict->keys[i] == key) {
//             dict->values[i] = value;
//             return;
//         }
//     }
//     dict->keys[dict->size] = key;
//     dict->values[dict->size] = value;
//     dict->size++;
// }

// int Dict_get_value(Dictionary* dict, int key) {
//     for (int i = 0; i < dict->size; i++) {
//         if (dict->keys[i] == key) {
//             return dict->values[i];
//         }
//     }
//     return -1; // Not found
// }

// int Dict_get_key(Dictionary* dict, int value) {
//     for (int i = 0; i < dict->size; i++) {
//         if (dict->values[i] == value) {
//             return dict->keys[i];
//         }
//     }
//     return -1; // Not found
// }

// int Dict_set_value(Dictionary* dict, int key, int value) {
//     for (int i = 0; i < dict->size; i++) {
//         if (dict->keys[i] == key) {
//             dict->values[i] = value;
//             return 0; // Success
//         }
//     }
//     return -1; // Key not found
// }
