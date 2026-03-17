#include <stdlib.h>
#include <stdio.h>
#include "performance_analysis.h"

// Single global storage for performance counters
static PerformanceMetrics perf_metrics;

// Helper macro for reading CSR with proper inline asm syntax
#define READ_CSR(csr_name, dest_var) \
    asm("csrr %0, " #csr_name : "=r" (dest_var))

// Helper macro for writing CSR with proper inline asm syntax
#define WRITE_CSR(csr_name, src_val) \
    asm volatile("csrw " #csr_name ", %0" : : "r" (src_val))

void performance_counter_start(void) {
    // Reset all 9 hardware counters to zero
    WRITE_CSR(mcycle, 0);
    WRITE_CSR(minstret, 0);
    WRITE_CSR(mhpmcounter3, 0);
    WRITE_CSR(mhpmcounter4, 0);
    WRITE_CSR(mhpmcounter5, 0);
    WRITE_CSR(mhpmcounter6, 0);
    WRITE_CSR(mhpmcounter7, 0);
    WRITE_CSR(mhpmcounter8, 0);
    WRITE_CSR(mhpmcounter9, 0);
    
    // Read initial values to verify reset and account for reset overhead
    READ_CSR(mcycle, perf_metrics.start_cycles);
    READ_CSR(minstret, perf_metrics.start_instructions);
    READ_CSR(mhpmcounter3, perf_metrics.start_mem_instr);
    READ_CSR(mhpmcounter4, perf_metrics.start_icache_misses);
    READ_CSR(mhpmcounter5, perf_metrics.start_dcache_misses);
    READ_CSR(mhpmcounter6, perf_metrics.start_icache_stalls);
    READ_CSR(mhpmcounter7, perf_metrics.start_dcache_stalls);
    READ_CSR(mhpmcounter8, perf_metrics.start_hazard_stalls);
    READ_CSR(mhpmcounter9, perf_metrics.start_alu_stalls);
}

void performance_counter_end(void) {
    // Read final counter values
    READ_CSR(mcycle, perf_metrics.end_cycles);
    READ_CSR(minstret, perf_metrics.end_instructions);
    READ_CSR(mhpmcounter3, perf_metrics.end_mem_instr);
    READ_CSR(mhpmcounter4, perf_metrics.end_icache_misses);
    READ_CSR(mhpmcounter5, perf_metrics.end_dcache_misses);
    READ_CSR(mhpmcounter6, perf_metrics.end_icache_stalls);
    READ_CSR(mhpmcounter7, perf_metrics.end_dcache_stalls);
    READ_CSR(mhpmcounter8, perf_metrics.end_hazard_stalls);
    READ_CSR(mhpmcounter9, perf_metrics.end_alu_stalls);
    
    // Calculate deltas (accounting for start values)
    perf_metrics.total_cycles = perf_metrics.end_cycles - perf_metrics.start_cycles;
    perf_metrics.total_instructions = perf_metrics.end_instructions - perf_metrics.start_instructions;
    perf_metrics.total_mem_instr = perf_metrics.end_mem_instr - perf_metrics.start_mem_instr;
    perf_metrics.total_icache_misses = perf_metrics.end_icache_misses - perf_metrics.start_icache_misses;
    perf_metrics.total_dcache_misses = perf_metrics.end_dcache_misses - perf_metrics.start_dcache_misses;
    perf_metrics.total_icache_stalls = perf_metrics.end_icache_stalls - perf_metrics.start_icache_stalls;
    perf_metrics.total_dcache_stalls = perf_metrics.end_dcache_stalls - perf_metrics.start_dcache_stalls;
    perf_metrics.total_hazard_stalls = perf_metrics.end_hazard_stalls - perf_metrics.start_hazard_stalls;
    perf_metrics.total_alu_stalls = perf_metrics.end_alu_stalls - perf_metrics.start_alu_stalls;
}

void performance_counter_print(void) {
    print("\n============ PERFORMANCE ANALYSIS ============\n\n");
    
    // --- RAW COUNTER VALUES TABLE ---
    print("RAW COUNTER VALUES\n");
    print("------------------\n");
    
    print("Cycles:              ");
    print_dec(perf_metrics.total_cycles);
    print("\n");
    
    print("Instructions:        ");
    print_dec(perf_metrics.total_instructions);
    print("\n");
    
    print("Memory Instructions: ");
    print_dec(perf_metrics.total_mem_instr);
    print("\n");
    
    print("I-Cache Misses:      ");
    print_dec(perf_metrics.total_icache_misses);
    print("\n");
    
    print("D-Cache Misses:      ");
    print_dec(perf_metrics.total_dcache_misses);
    print("\n");
    
    print("I-Cache Stalls:      ");
    print_dec(perf_metrics.total_icache_stalls);
    print("\n");
    
    print("D-Cache Stalls:      ");
    print_dec(perf_metrics.total_dcache_stalls);
    print("\n");
    
    print("Data Hazard Stalls:  ");
    print_dec(perf_metrics.total_hazard_stalls);
    print("\n");
    
    print("ALU Stalls:          ");
    print_dec(perf_metrics.total_alu_stalls);
    print("\n\n");
    
    // --- DERIVED METRICS ---
    print("DERIVED METRICS\n");
    print("---------------\n");
    
    // 1. Execution time in milliseconds (30 MHz = 30M cycles/sec)
    // time_ms = cycles / 30000
    unsigned int time_ms = perf_metrics.total_cycles / 30000;
    unsigned int time_remainder = (perf_metrics.total_cycles % 30000) / 30;
    print("Execution Time:      ");
    print_dec(time_ms);
    print(".");
    if (time_remainder < 10) print("0");
    if (time_remainder < 100) print("0");
    print_dec(time_remainder);
    print(" ms\n");
    
    // 2. IPC (Instructions Per Cycle)
    unsigned int ipc_num = perf_metrics.total_instructions;
    unsigned int ipc_den = perf_metrics.total_cycles > 0 ? perf_metrics.total_cycles : 1;
    unsigned int ipc_value = (ipc_num * 100) / ipc_den;
    print("IPC:                 0.");
    if (ipc_value < 10) print("0");
    print_dec(ipc_value);
    print("\n");
    
    // 3. I-Cache Hit Rate (%)
    // hit_rate = 1 - (misses / total_instructions) 
    // For display: (total_instructions - misses) / total_instructions * 100
    unsigned int icache_hits = perf_metrics.total_instructions > perf_metrics.total_icache_misses ? 
                               perf_metrics.total_instructions - perf_metrics.total_icache_misses : 0;
    unsigned int icache_hit_pct = perf_metrics.total_instructions > 0 ? 
                                  (icache_hits * 100) / perf_metrics.total_instructions : 100;
    print("I-Cache Hit Rate:    ");
    print_dec(icache_hit_pct);
    print("%\n");
    
    // 4. D-Cache Hit Rate (%)
    // hit_rate = 1 - (misses / memory_instructions)
    unsigned int dcache_hits = perf_metrics.total_mem_instr > perf_metrics.total_dcache_misses ? 
                               perf_metrics.total_mem_instr - perf_metrics.total_dcache_misses : 0;
    unsigned int dcache_hit_pct = perf_metrics.total_mem_instr > 0 ? 
                                  (dcache_hits * 100) / perf_metrics.total_mem_instr : 100;
    print("D-Cache Hit Rate:    ");
    print_dec(dcache_hit_pct);
    print("%\n");
    
    // 5. Stall percentages
    unsigned int icache_stall_pct = perf_metrics.total_cycles > 0 ? 
                                    (perf_metrics.total_icache_stalls * 100) / perf_metrics.total_cycles : 0;
    print("I-Cache Stall %:     ");
    print_dec(icache_stall_pct);
    print("%\n");
    
    unsigned int dcache_stall_pct = perf_metrics.total_cycles > 0 ? 
                                    (perf_metrics.total_dcache_stalls * 100) / perf_metrics.total_cycles : 0;
    print("D-Cache Stall %:     ");
    print_dec(dcache_stall_pct);
    print("%\n");
    
    unsigned int hazard_stall_pct = perf_metrics.total_cycles > 0 ? 
                                    (perf_metrics.total_hazard_stalls * 100) / perf_metrics.total_cycles : 0;
    print("Data Hazard Stall %: ");
    print_dec(hazard_stall_pct);
    print("%\n");
    
    unsigned int alu_stall_pct = perf_metrics.total_cycles > 0 ? 
                                 (perf_metrics.total_alu_stalls * 100) / perf_metrics.total_cycles : 0;
    print("ALU Stall %:         ");
    print_dec(alu_stall_pct);
    print("%\n");
    
    print("\n=============================================\n\n");
}

void performance_counter_reset(void) {
    // Clear all fields
    perf_metrics.start_cycles = 0;
    perf_metrics.start_instructions = 0;
    perf_metrics.start_mem_instr = 0;
    perf_metrics.start_icache_misses = 0;
    perf_metrics.start_dcache_misses = 0;
    perf_metrics.start_icache_stalls = 0;
    perf_metrics.start_dcache_stalls = 0;
    perf_metrics.start_hazard_stalls = 0;
    perf_metrics.start_alu_stalls = 0;
    
    perf_metrics.end_cycles = 0;
    perf_metrics.end_instructions = 0;
    perf_metrics.end_mem_instr = 0;
    perf_metrics.end_icache_misses = 0;
    perf_metrics.end_dcache_misses = 0;
    perf_metrics.end_icache_stalls = 0;
    perf_metrics.end_dcache_stalls = 0;
    perf_metrics.end_hazard_stalls = 0;
    perf_metrics.end_alu_stalls = 0;
    
    perf_metrics.total_cycles = 0;
    perf_metrics.total_instructions = 0;
    perf_metrics.total_mem_instr = 0;
    perf_metrics.total_icache_misses = 0;
    perf_metrics.total_dcache_misses = 0;
    perf_metrics.total_icache_stalls = 0;
    perf_metrics.total_dcache_stalls = 0;
    perf_metrics.total_hazard_stalls = 0;
    perf_metrics.total_alu_stalls = 0;
}