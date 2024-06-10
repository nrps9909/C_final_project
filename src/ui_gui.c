#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include "ui_gui.h"
#include "game_engine.h"

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

    SDL_GetWindowSize(window, &window_width, &window_height);

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
        dialogue_box_texture = NULL;
    }
    if (font) {
        TTF_CloseFont(font);
        font = NULL;
    }
    TTF_Quit();
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = NULL;
    }
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

    SDL_Color color = {0, 0, 0, 255};
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

void display_scene(GameData* gameData, const char* scene_name, Player *player) {
    int scene_index = find_scene_index(gameData, scene_name);
    if (scene_index < 0 || scene_index >= 10) {
        fprintf(stderr, "無效的場景索引: %d\n", scene_index);
        return;
    }

    const char* background_path = gameData->scenes[scene_index].background;
    SDL_Surface* background = load_image(background_path);
    if (!background) {
        fprintf(stderr, "無法加載背景圖像: %s\n", background_path);
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

    // Display characters
    for (int i = 0; i < 10; i++) {
        if (strlen(gameData->characters[i].location) > 0 &&
            strcmp(gameData->characters[i].location, gameData->scenes[scene_index].name) == 0) {
            const char* tachie_path = gameData->characters[i].tachie;
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
            SDL_Rect tachie_rect = { tachie_x, tachie_y, tachie->w, tachie->h };  
            SDL_RenderCopy(renderer, tachie_texture, NULL, &tachie_rect);
            SDL_DestroyTexture(tachie_texture);
            SDL_FreeSurface(tachie);  
        }
    }

    // Display items in the scene
    for (int i = 0; i < 10; i++) {
        if (strlen(gameData->items[i].icon) > 0) {
            const char* icon_path = gameData->items[i].icon;
            SDL_Surface* icon = load_image(icon_path);
            if (!icon) {
                fprintf(stderr, "Failed to load image: %s\n", icon_path);
                continue;
            }

            SDL_Texture* icon_texture = SDL_CreateTextureFromSurface(renderer, icon);
            if (!icon_texture) {
                fprintf(stderr, "Unable to create texture from %s! SDL_Error: %s\n", icon_path, SDL_GetError());
                SDL_FreeSurface(icon);
                continue;
            }

            int icon_x = (int)(window_width * 0.1) * (i + 1);
            int icon_y = (int)(window_height * 0.8);
            SDL_Rect icon_rect = { icon_x, icon_y, icon->w, icon->h };  
            SDL_RenderCopy(renderer, icon_texture, NULL, &icon_rect);
            SDL_DestroyTexture(icon_texture);
            SDL_FreeSurface(icon);  
        }
    }

    SDL_RenderPresent(renderer);

    // Display inventory on the screen
    int text_x = window_width * 0.8;
    int text_y = window_height * 0.1;
    render_text("背包:", text_x, text_y);
    text_y += 30;
    for (int i = 0; i < player->inventory_count; i++) {
        render_text(player->inventory[i], text_x, text_y + i * 30);
    }
    SDL_RenderPresent(renderer);
}

void display_inventory_screen(Player *player) {
    SDL_GetWindowSize(window, &window_width, &window_height);
    SDL_RenderClear(renderer);

    // Render inventory background
    SDL_Rect bgRect = {0, 0, window_width, window_height};
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &bgRect);

    // Render inventory title
    render_text("背包", window_width / 2 - 50, 50);

    // Render each inventory item
    int text_x = 100;
    int text_y = 150;
    for (int i = 0; i < player->inventory_count; i++) {
        render_text(player->inventory[i], text_x, text_y + i * 30);
    }

    // Render exit instruction
    render_text("按ESC退出背包", text_x, text_y + player->inventory_count * 30 + 50);
    SDL_RenderPresent(renderer);

    // Wait for user to exit inventory
    SDL_Event e;
    while (1) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                cleanup_ui();
                exit(0);
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    return;
                }
            }
        }
        SDL_Delay(100);
    }
}

void display_dialogue(GameData* gameData, int dialogue_index) {
    if (!gameData) {
        fprintf(stderr, "gameData 為空指針\n");
        return;
    }

    if (dialogue_index < 0 || dialogue_index >= 20) { 
        fprintf(stderr, "無效的 dialogue_index: %d\n", dialogue_index);
        return;
    }

    SDL_GetWindowSize(window, &window_width, &window_height);
    SDL_Rect dialogueRect = {window_width / 4, window_height - 300, 800, 200};
    SDL_RenderCopy(renderer, dialogue_box_texture, NULL, &dialogueRect);

    int text_x = dialogueRect.x + 18;
    int text_y = dialogueRect.y + 18;
    render_text(gameData->dialogues[dialogue_index].character, text_x, text_y); 
    text_x += 20;
    text_y += 50;  
    render_text(gameData->dialogues[dialogue_index].text, text_x, text_y);

    text_x += 30;
    text_y += 50;  
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
                cleanup_ui();
                exit(0);
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_1:
                        return 1;
                    case SDLK_2:
                        return 2;
                    case SDLK_i:  // 使用 i 鍵打開背包
                        return 0;
                    case SDLK_ESCAPE:  
                        cleanup_ui();
                        exit(0);
                    default:
                        break;
                }
            }
        }
        SDL_Delay(100);
    }
    return -1;
}
