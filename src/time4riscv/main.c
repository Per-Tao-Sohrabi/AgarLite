/* main.c

   This file written 2024 by Artur Podobas and Pedro Antunes

   For copyright and licensing, see file COPYING */

/* Below functions are external and found in other files. */
#include "GameState.h" 
#include "Entities.h"
#include <stdint.h>
#include <stdio.h>
#include "inputs.c"
#include "prog_states.c"

extern void print(const char*);
extern void print_dec(unsigned int);
extern void display_string(char*);
//extern void time2string(char*,int);
//extern void tick(int*);
//extern void delay(int);
extern int nextprime( int );
extern void enable_interrupts(void);
extern void render_game(volatile GameState* gs);
extern GameState run_start_up_seq(void);

// Timer buffer
volatile int* timer = (volatile int*) 0x04000020;

/*Initiates the timer and sets its attributes*/
void labinit(void) {
  // Set period to 3 MHz:
  int period_val = 3000000 -1; // Subtract 1 because timer counts from 0
  timer[2] = period_val & 0xFFFF; //  Lower 16 bits
  timer[3] = (period_val >> 16) & 0xFFFF;
  
  // Set start status
  timer[0] = 0b110; // Enable timer, sets ito = off, cont = on, start = on, stop. = off. 
}

/* Below is the function that will be called when an interrupt is triggered. */
void handle_interrupt(unsigned cause) 
{};

/* Helper function for getting the pause switch (which is switch n.4)*/
int get_pause_swtch() {
  return get_switch_states(4);
}

int read_inputs() {
  // LsB Pair
  int ls_sw1 = get_switch_states(0);
  int ls_sw2 = get_switch_states(1);
  // MsB Pair
  int ms_sw1 = get_switch_states(8);
  int ms_sw2 = get_switch_states(9);
  // Pause Switch
  int pause_swtch = get_pause_swtch(4);
  // Combine into input vector
  int input_vector[] = {ls_sw1, ls_sw2, ms_sw1, ms_sw2, pause_swtch};
  return input_vector;
}

/* Your code goes into main as well as any needed functions. */
int main() {
  // Enable timer
  labinit();

  // Enable interrupts
  enable_interrupts();
  
  // Display a welcome message.
  GameState gs = run_start_up(); // Set the game state and diffuculty mode.
  // Start game query ...

  // MAIN GAME LOOP
  while (1) {

    // READ PLAYER INPUT
    int input_vector[] = read_inputs();
    
    // UPDATE THE GAME STATE:
    GameState_update(&gs, input_vector);

    // DELAY FOR A WHILE
    while((timer[0] & 0b1) == 0 ) {
      // Busy wait for TO flag
    }
    timer[0] = 0b1; // Reset TO flag
    
    // RENDER GAME STATE
    render_game(&gs);

    // CHECK FOR PAUSE
    if (get_pause_swtch() == 1) {
      run_pause();
    }  


    
  }
}


