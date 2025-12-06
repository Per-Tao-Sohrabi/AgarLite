/* main.c

   This file written 2024 by Artur Podobas and Pedro Antunes

   For copyright and licensing, see file COPYING */

/* Below functions are external and found in other files. */
#include "GameState.h" 
#include "Entities.h"
#include "inputs.h"
#include "prog_states.h"

#include <stdbool.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

extern void enable_interrupts(void);

//#include "render.c"
//extern void print(const char*);
//extern void print_dec(unsigned int);
//extern void display_string(char*);
//extern void time2string(char*,int);
//extern void tick(int*);
//extern void delay(int);
//extern int nextprime( int );
// extern void render_game(volatile GameState* gs);
//extern GameState run_start_up(void);
//extern void run_pause(void);
//extern void run_game_over(void);

// Timer buffer
volatile int* timer = (volatile int*) 0x04000020;

/* Initiates the timer and sets its attributes*/
void labinit(void) {
  printf("---- Initializing timer...\n");
  // Set period to 3 MHz:
  int period_val = 3000000 -1; // Subtract 1 because timer counts from 0
  timer[2] = period_val & 0xFFFF; //  Lower 16 bits
  timer[3] = (period_val >> 16) & 0xFFFF;

  // Set start status
  timer[0] = 0b110; // Enable timer, sets ito = off, cont = on, start = on, stop. = off. 
  printf("---- Timer start status set.\n");
}

/* Below is the function that will be called when an interrupt is triggered. */
void handle_interrupt(unsigned cause) 
{};

/* Helper function for getting the pause switch (which is switch n.4)*/
int get_pause_swtch() {
  return get_switch_states(4);
}

/* Helper function that
    Reads the switch states and returns them in an array,
   RETURNS 
    An array with the switch values.
*/
void read_inputs(int* input_vector) {
  // LsB Pair
  int ls_sw1 = get_switch_states(0);
  int ls_sw2 = get_switch_states(1);
  // MsB Pair
  int ms_sw1 = get_switch_states(8);
  int ms_sw2 = get_switch_states(9);
  // Pause Switch
  int pause_swtch = get_pause_swtch(4);
  // Combine into input vector
  input_vector[0] = ls_sw1;
  input_vector[1] = ls_sw2;
  input_vector[2] = ms_sw1;
  input_vector[3] = ms_sw2; 
  input_vector[4] = pause_swtch;
}

/* Your code goes into main as well as any needed functions. */
int main() {
  printf("- Starting Time4RiscV...\n");
  // Enable timer
  labinit();
  printf("- Timer enabled.\n");

  // Enable interrupts
  enable_interrupts();
  printf("- Interrupts enabled.\n");
  // Display a welcome message.
  printf('- Running startup sequence...\n');
  GameState gs = run_start_up(); // Set the game state and diffuculty mode.
  volatile GameState* gs_ptr = &gs;
  // Start game query ...
  // MAIN GAME LOOP
  int input_vector[5] = {0}; // Input vector to hold switch states
  while (1) {

    // READ PLAYER INPUT
    read_inputs(input_vector);
    
    // CHECK FOR PAUSE
    if (get_pause_swtch() == 1) {
      run_pause();
    }  
    // UPDATE THE GAME STATE:
    bool game_over = GameState_update(gs_ptr, input_vector); 

    if (game_over) {
      run_game_over();
    }
    // DELAY FOR A WHILE (UNTIL TIMER TO FLAG IS RAISED)
    while((timer[0] & 0b1) == 0 ) {
    }
    timer[0] = 0b1; // Reset TO flag
    
    // RENDER GAME STATE
    //render_game(gs_ptr);

  }
}


