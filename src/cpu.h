#ifndef CPU_H
#define CPU_H

#include <stdint.h>

typedef struct {
    uint8_t A, B, C, D, E, F, H, L;
    uint16_t PC, SP;
    uint8_t memory[0x10000];
} CPU;

void cpu_init(CPU *cpu);
void cpu_step(CPU *cpu);

#endif
