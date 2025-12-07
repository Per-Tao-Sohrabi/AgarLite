#include "inputs.h"

int get_btn() {
  volatile int *btn = (volatile int*) 0x040000d0;
  return *btn;
}

int get_switches() {
    volatile int *switch_register = (volatile int*) 0x04000010;
    return *switch_register; 
}

int get_switch_state(int bit_position) {
    // 1. Define pointer to the base register (all switches)
    volatile int *switch_register = (volatile int*) 0x04000010;
    
    // 2. Read the full 32-bit register once
    int all_switches = *switch_register;
    
    // 3. Use bitwise shift and mask to isolate the bit
    // (all_switches >> bit_position) moves the desired bit to the far right.
    // & 1 masks out all other bits, leaving only 0 or 1.
    return (all_switches >> bit_position) & 1;
}