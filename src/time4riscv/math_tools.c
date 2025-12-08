
#include "math_tools.h"
#include <stdint.h> 

// For fast rand
extern volatile int* timer; 

// State for our Xorshift PRNG (initialized to a non-zero value)
static uint32_t xorshift_state = 1; 

// Define the correct index for the current count register
#define TIMER_COUNT_REG 1

// This is the fastest possible PRNG using Xorshift
// It relies on xorshift_state being initialized (seeded) elsewhere.
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

// You MUST define the max capacity of your dictionary arrays.
#define MAX_DICT_SIZE 500 // <--- REPLACE 500 WITH YOUR ACTUAL DICTIONARY ARRAY SIZE

void Dict_insert(Dictionary* dict, int key, int value) {
    // ... [Existing key update logic here] ...
    
    // CRITICAL: Check if the dictionary is full before insertion
    if (dict->size < MAX_DICT_SIZE) { 
        dict->keys[dict->size] = key;
        dict->values[dict->size] = value;
        dict->size++;
    } else {
        // Optional: print a message to help debug if capacity is hit
        // print("ERROR: Dictionary overflow prevented.\n");
    }
}

int Dict_get_value(Dictionary* dict, int key) {
    for (int i = 0; i < dict->size; i++) {
        if (dict->keys[i] == key) {
            return dict->values[i];
        }
    }
    return -1; // Not found
}

int Dict_get_key(Dictionary* dict, int value) {
    for (int i = 0; i < dict->size; i++) {
        if (dict->values[i] == value) {
            return dict->keys[i];
        }
    }
    return -1; // Not found
}

int Dict_set_value(Dictionary* dict, int key, int value) {
    for (int i = 0; i < dict->size; i++) {
        if (dict->keys[i] == key) {
            dict->values[i] = value;
            return 0; // Success
        }
    }
    return -1; // Key not found
}