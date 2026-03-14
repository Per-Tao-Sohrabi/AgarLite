/* main.c

   This file written 2024 by Artur Podobas and Pedro Antunes

   For copyright and licensing, see file COPYING */

#include "GameState.h"
#include "Entities.h"
#include "inputs.h"
#include "prog_states.h"
#include "render.h"

#include <stdbool.h>
#include <stdint.h>

extern void enable_interrupts(void);
extern void init_buffers(void);
extern void print(const char*);

// Hardware: timer registers (memory-mapped)
volatile int* timer = (volatile int*) 0x04000020;

// Frame sync flag — set by timer interrupt, cleared each frame
volatile bool game_tick = false;

/* Initiates the timer and sets its attributes */
void labinit(void) {
  print("---- Initializing timer...\n");
  int period_val = 3000000 - 1;
  timer[2] = period_val & 0xFFFF;
  timer[3] = (period_val >> 16) & 0xFFFF;
  timer[1] = 0b0111; // ITO=on, CONT=on, START=on
  print("---- Timer start status set.\n");
}

/* Timer interrupt handler */
void handle_interrupt(unsigned cause) {
  timer[0] = 0b1; // Reset TO flag
  game_tick = true;
}

/* Read all input switches into a vector */
void read_inputs(int* input_vector) {
  input_vector[0] = get_switch_state(0);  // P1 X
  input_vector[1] = get_switch_state(1);  // P1 Y
  input_vector[2] = get_switch_state(8);  // P2 X
  input_vector[3] = get_switch_state(9);  // P2 Y
  input_vector[4] = get_switch_state(4);  // Pause
}

/* Main entry point */
int main() {
  init_buffers();
  labinit();
  enable_interrupts();
  print("- Time4RiscV started.\n");

  GameState gs;
  int input_vector[5] = {0};
  ProgramState state = STATE_SPLASH;

  // Single frame loop — all states are non-blocking
  while (1) {
    while (!game_tick) {}  // Wait for timer interrupt
    game_tick = false;

    read_inputs(input_vector);

    switch (state) {
      case STATE_SPLASH:
        state = state_splash();
        break;
      case STATE_MENU_START:
        state = state_menu_start(&gs);
        break;
      case STATE_TRANSITION:
        state = state_transition();
        break;
      case STATE_PLAYING:
        state = state_playing(&gs, input_vector);
        break;
      case STATE_PAUSED:
        state = state_paused();
        break;
      case STATE_GAME_OVER:
        state = state_game_over();
        break;
    }
  }
}
