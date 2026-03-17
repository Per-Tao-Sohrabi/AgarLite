#include <stdlib.h>
#include <stdio.h>
#include "performance_analysis.h"

// Storage for performance counters - persists across start/end calls
static PerformanceMetrics perf_metrics;

void performance_counter_start(void) {
    // Reset both mcycle and minstret counters to zero
    // This ensures we start counting from a known state
    asm volatile("csrw mcycle, x0");
    asm volatile("csrw minstret, x0");
    
    // Read initial values to verify reset
    asm("csrr %0, mcycle" : "=r" (perf_metrics.start_cycles));
    asm("csrr %0, minstret" : "=r" (perf_metrics.start_instructions));
}

void performance_counter_end(void) {
    // Read final counter values
    asm("csrr %0, mcycle" : "=r" (perf_metrics.end_cycles));
    asm("csrr %0, minstret" : "=r" (perf_metrics.end_instructions));
    
    // Calculate deltas (accounting for reset at start)
    perf_metrics.total_cycles = perf_metrics.end_cycles - perf_metrics.start_cycles;
    perf_metrics.total_instructions = perf_metrics.end_instructions - perf_metrics.start_instructions;
}

void performance_counter_print(void) {
    // Calculate IPC (Instructions Per Cycle)
    unsigned int ipc_num = perf_metrics.total_instructions;
    unsigned int ipc_den = perf_metrics.total_cycles > 0 ? perf_metrics.total_cycles : 1;
    
    print("\n=== Performance Analysis ===\n");
    print("Cycles: ");
    print_dec(perf_metrics.total_cycles);
    print("\n");
    
    print("Instructions: ");
    print_dec(perf_metrics.total_instructions);
    print("\n");
    
    // Display IPC as fixed-point (instructions.fractional per cycle)
    // Example: 1000 instructions / 1200 cycles = 0.833 IPC displayed as 833 (thousandths)
    unsigned int ipc_thousandths = (ipc_num * 1000) / ipc_den;
    print("IPC: 0.");
    print_dec(ipc_thousandths);
    print("\n");
}

void performance_counter_reset(void) {
    // Reset all performance counter fields
    perf_metrics.start_cycles = 0;
    perf_metrics.start_instructions = 0;
    perf_metrics.end_cycles = 0;
    perf_metrics.end_instructions = 0;
    perf_metrics.total_cycles = 0;
    perf_metrics.total_instructions = 0;
}