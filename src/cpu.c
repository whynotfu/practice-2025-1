#include "cpu.h"
#include <stdio.h>

void cpu_init(CPU *cpu) {
    cpu->PC = 0x0100; //Program Counter
    cpu->SP = 0xFFFE; //Stack Pointer 
    for (int i = 0; i < 0x10000; i++) { //Очищает всю память 64 КБ (0x10000 байт), заполняя нулями.
        // Подготавливает память для игры (ROM загружается сюда).
        cpu->memory[i] = 0;
    }
}

void cpu_step(CPU *cpu) {
    uint8_t opcode = cpu->memory[cpu->PC++];
    printf("Executing opcode: %02X at PC: %04X\n", opcode, cpu->PC - 1);

    switch (opcode) {
        case 0x00: // NOP
            break;
        case 0x06: // LD B, n
            cpu->B = cpu->memory[cpu->PC++];
            printf("Loaded %02X into B\n", cpu->B);
            break;
        default:
            printf("Unknown opcode: %02X\n", opcode);
            break;
    }
}
