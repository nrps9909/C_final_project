#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "ui_gui.h"
#include "game_engine.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

void init_ui() {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    window = SDL_CreateWindow("Interactive Fiction Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
}

void cleanup_ui() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

void render_texture(const char *file_path, int x, int y, int w, int h) {
    SDL_Surface *surface = IMG_Load(file_path);
    if (!surface) {
        printf("IMG_Load: %s\n", IMG_GetError());
        return;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    dest.w = w;
    dest.h = h;

    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
}

void display_scene(GameData *gameData, int scene_index) {
    SDL_RenderClear(renderer);
    render_texture(gameData->scenes[scene_index].background, 0, 0, 800, 600);
    // 渲染對話選項
    //SDL_Color textColor = {255, 255, 255, 0};
    // 假設有文字渲染函數
    // render_text(gameData->dialogues[scene_index].options[0].text, textColor, 10, 500);
    // render_text(gameData->dialogues[scene_index].options[1].text, textColor, 10, 550);
    SDL_RenderPresent(renderer);
}

int get_user_choice() {
    SDL_Event event;
    while (SDL_WaitEvent(&event)) {
        if (event.type == SDL_QUIT) {
            cleanup_ui();
            exit(0);
        } else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_1) {
                return 1;
            } else if (event.key.keysym.sym == SDLK_2) {
                return 2;
            }
        }
    }
    return 0;
}
