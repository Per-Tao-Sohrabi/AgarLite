#ifndef PERFORMANCE_ANALYSIS_H
#define PERFORMANCE_ANALYSIS_H

/**
 * PerformanceMetrics - Structure to hold hardware counter measurements
 * 
 * Fields are 32-bit unsigned integers containing the lower 32 bits of the
 * 64-bit RISC-V CSR performance counters. For measurements taking < ~2 minutes
 * at 30 MHz, this provides sufficient precision.
 * 
 * mcycle increments on every clock cycle
 * minstret increments when an instruction is retired
 */
typedef struct {
    unsigned int start_cycles;        // mcycle value at start
    unsigned int start_instructions;  // minstret value at start
    unsigned int end_cycles;          // mcycle value at end
    unsigned int end_instructions;    // minstret value at end
    unsigned int total_cycles;        // end_cycles - start_cycles
    unsigned int total_instructions;  // end_instructions - start_instructions
} PerformanceMetrics;

/**
 * performance_counter_start - Initialize and reset performance counters
 * 
 * Writes zero to mcycle and minstret CSRs, then reads back the values
 * to verify they were reset. This ensures measurements start from a known state.
 */
void performance_counter_start(void);

/**
 * performance_counter_end - Capture final counter values
 * 
 * Reads current values of mcycle and minstret CSRs, computes the deltas from
 * the start values, and stores results. These values are preserved until the
 * next call to performance_counter_start.
 */
void performance_counter_end(void);

/**
 * performance_counter_print - Output performance metrics to console
 * 
 * Prints the total cycles, instructions executed, and computed IPC
 * (Instructions Per Cycle) to the output stream. IPC is displayed as
 * a fractional value.
 */
void performance_counter_print(void);

/**
 * performance_counter_reset - Clear all performance counter fields
 * 
 * Resets all stored counter values to zero. Useful for cleanup or
 * preparing for a fresh measurement session.
 */
void performance_counter_reset(void);

#endif /* PERFORMANCE_ANALYSIS_H */
