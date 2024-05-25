#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include "ui_gui.h"

// SDL_Window and SDL_Renderer pointers
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static TTF_Font* font = NULL;
static SDL_Texture* dialogue_box_texture = NULL;
static int window_width = 1920;
static int window_height = 1080;

void init_ui() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    if (TTF_Init() == -1) {
        fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
        exit(1);
    }

    const char* font_path = "third-party/TTF/setofont.ttf";
    font = TTF_OpenFont(font_path, 24);
    if (!font) {
        fprintf(stderr, "Failed to load font! TTF_Error: %s\n", TTF_GetError());
        exit(1);
    }

    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        fprintf(stderr, "SDL_image could not initialize! IMG_Error: %s\n", IMG_GetError());
        exit(1);
    }

    window = SDL_CreateWindow("Interactive Fiction Engine",
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              window_width, window_height, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (!window) {
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    // 獲取全螢幕模式下的實際窗口大小
    SDL_GetWindowSize(window, &window_width, &window_height);

    // 加載對話框圖像
    SDL_Surface* dialogue_box_surface = IMG_Load("third-party/Picture/text_rect.png");
    if (!dialogue_box_surface) {
        fprintf(stderr, "Unable to load image %s! SDL_image Error: %s\n", "path/to/your/dialogue_box_image.png", IMG_GetError());
        exit(1);
    }
    dialogue_box_texture = SDL_CreateTextureFromSurface(renderer, dialogue_box_surface);
    SDL_FreeSurface(dialogue_box_surface);
}

void cleanup_ui() {
    if (dialogue_box_texture) {
        SDL_DestroyTexture(dialogue_box_texture);
    }
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

SDL_Surface* load_image(const char* path) {
    SDL_Surface* img = IMG_Load(path);
    if (!img) {
        fprintf(stderr, "IMG_Load: Couldn't open %s: %s\n", path, IMG_GetError());
    } else {
        fprintf(stderr, "IMG_Load: Successfully loaded %s\n", path);
    }
    return img;
}

void render_text(const char* message, int x, int y) {
    if (!message || !renderer || !font) {
        fprintf(stderr, "render_text 的參數無效\n");
        return;
    }

    SDL_Color color = {0, 0, 0, 255};  // 白色文字，初始化 alpha 通道
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, message, color);
    if (!surface) {
        fprintf(stderr, "TTF_RenderUTF8_Blended 失敗: %s\n", TTF_GetError());
        return;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        fprintf(stderr, "SDL_CreateTextureFromSurface 失敗: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect dstrect = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void display_scene(GameData* gameData, int scene_index) {
    const char* background_path = gameData->scenes[scene_index].background;
    fprintf(stderr, "嘗試加載場景背景: %s\n", background_path);
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

    SDL_GetWindowSize(window, &window_width, &window_height);
    SDL_RenderClear(renderer);
    SDL_Rect dstrect = {0, 0, window_width, window_height};
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);
    SDL_DestroyTexture(texture);

    // 顯示角色立繪
    for (int i = 0; i < 10; i++) {
        if (strlen(gameData->characters[i].location) > 0 &&
            strcmp(gameData->characters[i].location, gameData->scenes[scene_index].name) == 0) {
            const char* tachie_path = gameData->characters[i].tachie;
            fprintf(stderr, "嘗試加載角色立繪: %s\n", tachie_path);
            SDL_Surface* tachie = load_image(tachie_path);
            if (!tachie) {
                fprintf(stderr, "Failed to load image: %s\n", tachie_path);
                continue;
            }

            SDL_Texture* tachie_texture = SDL_CreateTextureFromSurface(renderer, tachie);
            if (!tachie_texture) {
                fprintf(stderr, "Unable to create texture from %s! SDL_Error: %s\n", tachie_path, SDL_GetError());
                SDL_FreeSurface(tachie);
                continue;
            }

            int tachie_x = (int)(window_width * 0.75);
            int tachie_y = (int)(window_height * 0.1);
            SDL_Rect tachie_rect = { tachie_x, tachie_y, tachie->w, tachie->h };  // 設置立繪顯示位置和大小
            SDL_RenderCopy(renderer, tachie_texture, NULL, &tachie_rect);
            SDL_DestroyTexture(tachie_texture);

            SDL_FreeSurface(tachie);  // 創建紋理後釋放表面
        }
    }

    SDL_RenderPresent(renderer);
}

void display_dialogue(GameData* gameData, int dialogue_index) {
    if (!gameData) {
        fprintf(stderr, "gameData 為空指針\n");
        return;
    }

    if (dialogue_index < 0 || dialogue_index >= 20) { // 假設 dialogues 數組長度為 20
        fprintf(stderr, "無效的 dialogue_index: %d\n", dialogue_index);
        return;
    }

    fprintf(stderr, "顯示對話: %s\n", gameData->dialogues[dialogue_index].text);

    SDL_GetWindowSize(window, &window_width, &window_height);
    // 設置對話框背景
    SDL_Rect dialogueRect = {0, window_height - 300, 600, 200};  // 對話框位置和大小
    SDL_RenderCopy(renderer, dialogue_box_texture, NULL, &dialogueRect);  // 渲染對話框圖像

    // 顯示角色對話
    int text_x = dialogueRect.x + 18;
    int text_y = dialogueRect.y + 18;
    render_text(gameData->dialogues[dialogue_index].character, text_x, text_y); 
    text_x += 20;
    text_y += 50;  // 調整對話文本的位置
    render_text(gameData->dialogues[dialogue_index].text, text_x, text_y);

    // 顯示選項
    text_x += 30;
    text_y += 50;  // 調整選項文本的位置
    for (int i = 0; i < 2; i++) {
        if (strlen(gameData->dialogues[dialogue_index].options[i].text) > 0) {
            char option_text[256];
            snprintf(option_text, sizeof(option_text), "%d. %s", i + 1, gameData->dialogues[dialogue_index].options[i].text);
            render_text(option_text, text_x, text_y + i * 30);
        }
    }

    SDL_RenderPresent(renderer);
}
int get_user_choice() {
    SDL_Event e;
    while (1) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                return -1;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_1:
                        return 1;
                    case SDLK_2:
                        return 2;
                    default:
                        break;
                }
            }
        }
    }
}
