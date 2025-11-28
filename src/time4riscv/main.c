/* main.c

   This file written 2024 by Artur Podobas and Pedro Antunes

   For copyright and licensing, see file COPYING */

/* Below functions are external and found in other files. */
#include "GameState.h" 
#include "Entities.h"
#include <stdint.h>
#include <stdio.h>

extern void print(const char*);
extern void print_dec(unsigned int);
extern void display_string(char*);
extern void time2string(char*,int);
extern void tick(int*);
extern void delay(int);
extern int nextprime( int );
extern void enable_interrupts(void);
extern void render_game(volatile GameState* gs);
extern GameState run_start_up_seq(void);

// Timer buffer
volatile int* timer = (volatile int*) 0x04000020;

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

void read_swtch_pair(int *pair[], int offset) {
  pair[0] = (volatile int*) 0x04000010 + offset;
  pair[1] = (volatile int*) 0x04000010 + offset + 1;
}

/* Your code goes into main as well as any needed functions. */
int main() {
  // Enable timer
  labinit();

  // Enable interrupts
  enable_interrupts();
  
  // Display a welcome message.
  GameState gs = run_start_up_seq(); // Set the game state

  // MAIN GAME LOOP
  while (1) {
    // READ PLAYER INPUT
    int lsSwtch[2]; // Input from p1
    int msSwtch[2]; // Input from p2
    read_swtch_pair(lsSwtch, 8);
    read_swtch_pair(msSwtch, 0);
    int input_vector[] = {lsSwtch[0], lsSwtch[1], msSwtch[0], msSwtch[1]};
    
    // USE CASES TO UPDATE THE GAME STATE:
    // E.g., move pieces, check collisions, update scores, etc.

    // DELAY FOR A WHILE
    while((timer[0] & 0b1) == 0 ) {
      // Busy wait for TO flag
    }
    timer[0] = 0b1; // Reset TO flag
    
    // RENDER
    //render_game(&gs);
    
  }
}


