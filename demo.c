#include <SDL2/SDL.h>
#include <stdio.h>

#include "dungen.h"
#define RECT_SIZE 8
#define D_WIDTH 40
#define D_HEIGHT 40

int running = 1;
SDL_Event e;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

void render_cell(dg_dungeon d, int x, int y, enum dg_cell_kind k)
{
    SDL_Rect rect = {
        .x = x * RECT_SIZE,
        .y = y * RECT_SIZE,
        .w = RECT_SIZE,
        .h = RECT_SIZE 
    };

    switch (k) {
        case dg_cell_stone:
            SDL_SetRenderDrawColor(renderer, 66, 66, 66, 255); break;
        case dg_cell_wall:
            SDL_SetRenderDrawColor(renderer, 32, 128, 32, 255); break;
        case dg_cell_floor:
            SDL_SetRenderDrawColor(renderer, 33, 33, 44, 255); break;
    }

    SDL_RenderFillRect(renderer, &rect);

    switch (k) {
        case dg_cell_stone:
            SDL_SetRenderDrawColor(renderer, 54, 54, 54, 255); break;
        case dg_cell_wall:
            SDL_SetRenderDrawColor(renderer, 20, 116, 20, 255); break;
        case dg_cell_floor:
            SDL_SetRenderDrawColor(renderer, 21, 21, 32, 255); break;
    }

    SDL_RenderDrawRect(renderer, &rect);
}

void render_step(dg_dungeon d, int step)
{
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            running = 0;
        }
    }

    if (!running) {
        SDL_Quit();
        exit(0);
        return;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    dg_each(d, render_cell);

    SDL_RenderPresent(renderer);
}

/* if you'd like to save a screenshot 
static void screenshot(int w, int h, const char *fname)
{

    SDL_Surface *sshot = SDL_CreateRGBSurface(0, w, h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
    SDL_SaveBMP(sshot, "tmp.bmp");
    SDL_FreeSurface(sshot);
}
*/

int main(int argc, char **argv) {
    int d_width = D_WIDTH;
    int d_height = D_HEIGHT;

    if (argc == 3) {
        d_width  = atoi(argv[1]);
        d_height = atoi(argv[2]);
    }

    int w = (d_width * RECT_SIZE);
    int h = (d_height * RECT_SIZE);

    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow(
        "dungen",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        w,
        h,
        SDL_WINDOW_SHOWN
    );

    renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED
    );

    dg_dungeon dungeon = dg_create(d_width, d_height, render_step);

    int generating = 1;

    while (running) {

        while (generating) {
            dg_maze_diagonal(dungeon);

            generating = 0;
        }

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                running = 0;
            }

            if (e.type == SDL_KEYDOWN) {
                dg_reset(dungeon);
                generating = 1;
            }
        }

        SDL_Delay(100);
    }

    dg_free(dungeon);
    SDL_Quit();
}
