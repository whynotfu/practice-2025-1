#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

int SDL_main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s rom.gb\n", argv[0]);
        return 1;
    }
    
    FILE *rom = fopen(argv[1], "rb");
    if (!rom) {
        perror("Failed to open ROM");
        return 1;
    }

    CPU cpu;
    cpu_init(&cpu);

    fread(cpu.memory + 0x100, 1, 0x8000, rom);
    fclose(rom);

    printf("Starting CPU execution...\n");

    while (1) {
        cpu_step(&cpu);
        SDL_Delay(100); // чтобы замедлить выполнение
    }

    return 0;
}
