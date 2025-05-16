#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"


int SDL_main(int argc, char *argv[]) {
    CPU cpu;
    cpu_init(&cpu);

    printf("Starting CPU execution...\n");

    // Симуляция бесконечного выполнения команд
    while (1) {
        cpu_step(&cpu);
        SDL_Delay(100); // Чтобы не грузить процессор полностью
    }

    return 0;
}