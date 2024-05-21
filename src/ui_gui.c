#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include "ui_gui.h"

// SDL_Window and SDL_Renderer pointers
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

void init_ui() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        fprintf(stderr, "SDL_image could not initialize! IMG_Error: %s\n", IMG_GetError());
        exit(1);
    }

    window = SDL_CreateWindow("Interactive Fiction Engine",
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
}

void cleanup_ui() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

SDL_Surface* load_image(const char* path) {
    SDL_Surface* img = IMG_Load(path);
    if (!img) {
        fprintf(stderr, "IMG_Load: Couldn't open %s: %s\n", path, IMG_GetError());
    }
    return img;
}

void display_scene(GameData* gameData, int scene_index) {
    const char* background_path = gameData->scenes[scene_index].background;
    SDL_Surface* background = load_image(background_path);
    if (!background) {
        fprintf(stderr, "Failed to load image: %s\n", background_path);
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, background);
    if (!texture) {
        fprintf(stderr, "Unable to create texture from %s! SDL_Error: %s\n", background_path, SDL_GetError());
        SDL_FreeSurface(background);
        return;
    }

    SDL_FreeSurface(background);

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(texture);
}

int get_user_choice() {
    int choice;
    printf("Enter your choice: ");
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid input. Please enter a number.\n");
        choice = -1;  // 设置为一个无效的选择值
        while (getchar() != '\n');  // 清除缓冲区
    }
    return choice;
}
