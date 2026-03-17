#ifndef PERFORMANCE_ANALYSIS_H
#define PERFORMANCE_ANALYSIS_H

/**
 * PerformanceMetrics - Structure to hold all hardware counter measurements
 * 
 * All fields are 32-bit unsigned integers containing the lower 32 bits of the
 * 64-bit RISC-V CSR performance counters. For measurements taking < ~2 minutes
 * at 30 MHz, this provides sufficient precision.
 * 
 * Hardware counters (all 9 available):
 * - mcycle:        CPU cycle counter
 * - minstret:      Instructions retired
 * - mhpmcounter3:  Memory instructions (loads/stores)
 * - mhpmcounter4:  I-cache misses
 * - mhpmcounter5:  D-cache misses
 * - mhpmcounter6:  I-cache stalls
 * - mhpmcounter7:  D-cache stalls
 * - mhpmcounter8:  Data hazard stalls
 * - mhpmcounter9:  ALU stalls
 */
typedef struct {
    // Start values (raw counter snapshots)
    unsigned int start_cycles;
    unsigned int start_instructions;
    unsigned int start_mem_instr;
    unsigned int start_icache_misses;
    unsigned int start_dcache_misses;
    unsigned int start_icache_stalls;
    unsigned int start_dcache_stalls;
    unsigned int start_hazard_stalls;
    unsigned int start_alu_stalls;
    
    // End values (raw counter snapshots)
    unsigned int end_cycles;
    unsigned int end_instructions;
    unsigned int end_mem_instr;
    unsigned int end_icache_misses;
    unsigned int end_dcache_misses;
    unsigned int end_icache_stalls;
    unsigned int end_dcache_stalls;
    unsigned int end_hazard_stalls;
    unsigned int end_alu_stalls;
    
    // Derived metrics (deltas and calculations)
    unsigned int total_cycles;
    unsigned int total_instructions;
    unsigned int total_mem_instr;
    unsigned int total_icache_misses;
    unsigned int total_dcache_misses;
    unsigned int total_icache_stalls;
    unsigned int total_dcache_stalls;
    unsigned int total_hazard_stalls;
    unsigned int total_alu_stalls;
} PerformanceMetrics;


/**
 * performance_counter_start - Initialize and reset all performance counters
 * 
 * Writes zero to all 9 CSR counters (mcycle, minstret, mhpmcounter3-9),
 * then reads back the values to verify they were reset.
 */
void performance_counter_start(void);

/**
 * performance_counter_end - Capture final counter values
 * 
 * Reads current values of all 9 CSRs, computes the deltas from start values,
 * and stores results. These values are preserved until the next call to
 * performance_counter_start.
 */
void performance_counter_end(void);

/**
 * performance_counter_print - Output comprehensive performance metrics
 * 
 * Displays all raw counter values and derived metrics in formatted tables:
 * - Raw counter totals
 * - Calculated metrics (IPC, cache hit rates, stall percentages)
 * - Estimated execution time
 */
void performance_counter_print(void);

/**
 * performance_counter_reset - Clear all performance counter fields
 * 
 * Resets all stored counter values to zero for cleanup or fresh measurements.
 */
void performance_counter_reset(void);

#endif /* PERFORMANCE_ANALYSIS_H */
