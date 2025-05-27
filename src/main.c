#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 10
#define HEIGHT 20
#define BLOCK_SIZE 30

int field[HEIGHT][WIDTH] = {0};
int currentX = 0, currentY = 0;
int currentBlock = 0, rotation = 0;
int gameOver = 0;

// Описание фигур (7 фигур, 4 поворота, 4x4)
int blocks[7][4][4][4] = {
    // I
    {
        { {1,0,0,0}, {1,0,0,0}, {1,0,0,0}, {1,0,0,0} },
        { {0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0} },
        { {1,0,0,0}, {1,0,0,0}, {1,0,0,0}, {1,0,0,0} },
        { {0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0} },
    },
    // O
    {
        { {1,1,0,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0} },
        { {1,1,0,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0} },
        { {1,1,0,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0} },
        { {1,1,0,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0} },
    },
    // T
    {
        { {0,1,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0} },
        { {0,1,0,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0} },
        { {0,0,0,0}, {1,1,1,0}, {0,1,0,0}, {0,0,0,0} },
        { {0,1,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0} },
    },
    // L
    {
        { {0,0,1,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0} },
        { {0,1,0,0}, {0,1,0,0}, {0,1,1,0}, {0,0,0,0} },
        { {0,0,0,0}, {1,1,1,0}, {1,0,0,0}, {0,0,0,0} },
        { {1,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0} },
    },
    // J
    {
        { {1,0,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0} },
        { {0,1,1,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0} },
        { {0,0,0,0}, {1,1,1,0}, {0,0,1,0}, {0,0,0,0} },
        { {0,1,0,0}, {0,1,0,0}, {1,1,0,0}, {0,0,0,0} },
    },
    // S
    {
        { {0,1,1,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0} },
        { {0,1,0,0}, {0,1,1,0}, {0,0,1,0}, {0,0,0,0} },
        { {0,0,0,0}, {0,1,1,0}, {1,1,0,0}, {0,0,0,0} },
        { {1,0,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0} },
    },
    // Z
    {
        { {1,1,0,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0} },
        { {0,0,1,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0} },
        { {0,0,0,0}, {1,1,0,0}, {0,1,1,0}, {0,0,0,0} },
        { {0,1,0,0}, {1,1,0,0}, {1,0,0,0}, {0,0,0,0} },
    }
};

int canMove(int dx, int dy, int nextRotation);
void newBlock() {
    currentBlock = rand() % 7;
    rotation = 0;
    currentX = WIDTH / 2 - 2;
    currentY = 0;
    if (!canMove(0, 0, rotation)) {
        gameOver = 1;
    }
}

int canMove(int dx, int dy, int nextRotation) {
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            if (blocks[currentBlock][nextRotation][y][x]) {
                int fx = currentX + x + dx;
                int fy = currentY + y + dy;
                if (fx < 0 || fx >= WIDTH || fy >= HEIGHT || (fy >= 0 && field[fy][fx]))
                    return 0;
            }
    return 1;
}

void placeBlock() {
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            if (blocks[currentBlock][rotation][y][x]) {
                int fx = currentX + x;
                int fy = currentY + y;
                if (fy >= 0 && fy < HEIGHT && fx >= 0 && fx < WIDTH)
                    field[fy][fx] = 1;
            }
}

void clearLines() {
    for (int y = HEIGHT - 1; y >= 0; y--) {
        int full = 1;
        for (int x = 0; x < WIDTH; x++)
            if (field[y][x] == 0) {
                full = 0;
                break;
            }
        if (full) {
            for (int ny = y; ny > 0; ny--)
                for (int x = 0; x < WIDTH; x++)
                    field[ny][x] = field[ny-1][x];
            for (int x = 0; x < WIDTH; x++)
                field[0][x] = 0;
            y++;
        }
    }
}

void resetGame() {
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
            field[y][x] = 0;
    gameOver = 0;
    newBlock();
}

void draw(SDL_Renderer* renderer, SDL_Texture* gameOverTexture) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Поле
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
            if (field[y][x]) {
                SDL_Rect r = {x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
                SDL_RenderFillRect(renderer, &r);
            }

    // Падающий блок
    if (!gameOver) {
        SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
        for (int y = 0; y < 4; y++)
            for (int x = 0; x < 4; x++)
                if (blocks[currentBlock][rotation][y][x]) {
                    int fx = currentX + x;
                    int fy = currentY + y;
                    SDL_Rect r = {fx * BLOCK_SIZE, fy * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
                    SDL_RenderFillRect(renderer, &r);
                }
    }

    // Экран проигрыша
    if (gameOver) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 200);
        SDL_Rect r = {0, HEIGHT/2*BLOCK_SIZE - 40, WIDTH*BLOCK_SIZE, 80};
        SDL_RenderFillRect(renderer, &r);

        // Текст (просто белый прямоугольник как заглушка)
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect textRect = {WIDTH*BLOCK_SIZE/2 - 60, HEIGHT/2*BLOCK_SIZE - 10, 120, 20};
        SDL_RenderFillRect(renderer, &textRect);
    }

    SDL_RenderPresent(renderer);
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Tetris SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH*BLOCK_SIZE, HEIGHT*BLOCK_SIZE, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    srand(time(NULL));
    newBlock();

    Uint32 lastTick = SDL_GetTicks();
    int running = 1;

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = 0;

            if (e.type == SDL_KEYDOWN) {
                if (!gameOver) {
                    if (e.key.keysym.sym == SDLK_LEFT && canMove(-1, 0, rotation)) currentX--;
                    else if (e.key.keysym.sym == SDLK_RIGHT && canMove(1, 0, rotation)) currentX++;
                    else if (e.key.keysym.sym == SDLK_DOWN && canMove(0, 1, rotation)) currentY++;
                    else if (e.key.keysym.sym == SDLK_UP && canMove(0, 0, (rotation+1)%4))
                        rotation = (rotation+1)%4;
                } else if (e.key.keysym.sym == SDLK_r) {
                    resetGame();
                }
            }
        }

        if (!gameOver && SDL_GetTicks() - lastTick > 500) {
            if (canMove(0, 1, rotation)) currentY++;
            else {
                placeBlock();
                clearLines();
                newBlock();
            }
            lastTick = SDL_GetTicks();
        }

        draw(renderer, NULL);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}