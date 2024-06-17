#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include "ui_gui.h"
#include "game_engine.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static TTF_Font *font = NULL;
static SDL_Texture *dialogue_box_texture = NULL;
static int window_width = 1920;
static int window_height = 1080;

typedef enum
{
    TEXT1,
    TEXT2,
    TEXT3,
    OPTIONS
} DialogueState;

static DialogueState dialogue_state = TEXT1;
static int current_dialogue_index = 0;
static GameData *current_game_data = NULL;

void init_ui()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    if (TTF_Init() == -1)
    {
        fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
        SDL_Quit();
        exit(1);
    }

    const char *font_path = "third-party/TTF/setofont.ttf";
    font = TTF_OpenFont(font_path, 30);
    if (!font)
    {
        fprintf(stderr, "Failed to load font! TTF_Error: %s\n", TTF_GetError());
        TTF_Quit();
        SDL_Quit();
        exit(1);
    }

    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        fprintf(stderr, "SDL_image could not initialize! IMG_Error: %s\n", IMG_GetError());
        TTF_Quit();
        SDL_Quit();
        exit(1);
    }

    window = SDL_CreateWindow("Interactive Fiction Engine",
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              window_width, window_height, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (!window)
    {
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        exit(1);
    }

    SDL_GetWindowSize(window, &window_width, &window_height);

    SDL_Surface *dialogue_box_surface = IMG_Load("third-party/Picture/text_rect.png");
    if (!dialogue_box_surface)
    {
        fprintf(stderr, "Unable to load image %s! SDL_image Error: %s\n", "third-party/Picture/text_rect.png", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        exit(1);
    }
    dialogue_box_texture = SDL_CreateTextureFromSurface(renderer, dialogue_box_surface);
    SDL_FreeSurface(dialogue_box_surface);
}

void cleanup_ui()
{
    if (dialogue_box_texture)
    {
        SDL_DestroyTexture(dialogue_box_texture);
        dialogue_box_texture = NULL;
    }
    if (font)
    {
        TTF_CloseFont(font);
        font = NULL;
    }
    TTF_Quit();
    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }
    if (window)
    {
        SDL_DestroyWindow(window);
        window = NULL;
    }
    IMG_Quit();
    SDL_Quit();
}

SDL_Surface *load_image(const char *path)
{
    SDL_Surface *img = IMG_Load(path);
    if (!img)
    {
        fprintf(stderr, "IMG_Load: Couldn't open %s: %s\n", path, IMG_GetError());
    }
    else
    {
        fprintf(stderr, "IMG_Load: Successfully loaded %s\n", path);
    }
    return img;
}

void render_text(const char *message, int x, int y)
{
    if (!message || !renderer || !font)
    {
        fprintf(stderr, "render_text 的參數無效\n");
        return;
    }

    SDL_Color color = {0, 0, 0, 255};
    SDL_Surface *surface = TTF_RenderUTF8_Blended(font, message, color);
    if (!surface)
    {
        fprintf(stderr, "TTF_RenderUTF8_Blended 失敗: %s\n", TTF_GetError());
        return;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture)
    {
        fprintf(stderr, "SDL_CreateTextureFromSurface 失敗: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect dstrect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void display_scene(GameData *gameData, const char *scene_name, Player *player)
{
    if (!gameData)
    {
        fprintf(stderr, "gameData 為空指針\n");
        return;
    }

    int scene_index = find_scene_index(gameData, scene_name);
    if (scene_index < 0 || scene_index >= MAX_SCENES)
    {
        fprintf(stderr, "無效的場景索引: %d\n", scene_index);
        return;
    }

    const char *background_path = gameData->scenes[scene_index].background;
    SDL_Surface *background = load_image(background_path);
    if (!background)
    {
        fprintf(stderr, "無法加載背景圖像: %s\n", background_path);
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, background);
    if (!texture)
    {
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

    // Display items in the scene
    for (int i = 0; i < MAX_ITEMS; i++)
    {
        if (strlen(gameData->items[i].name) > 0 && strcmp(gameData->items[i].scene, gameData->scenes[scene_index].name) == 0)
        {
            const char *icon_path = gameData->items[i].icon;
            SDL_Surface *icon = load_image(icon_path);
            if (!icon)
            {
                fprintf(stderr, "Failed to load image: %s\n", icon_path);
                continue;
            }

            SDL_Texture *icon_texture = SDL_CreateTextureFromSurface(renderer, icon);
            if (!icon_texture)
            {
                fprintf(stderr, "Unable to create texture from %s! SDL_Error: %s\n", icon_path, SDL_GetError());
                SDL_FreeSurface(icon);
                continue;
            }

            int icon_x = (int)(window_width * 0.1) * (i + 1);
            int icon_y = (int)(window_height * 0.8);
            SDL_Rect icon_rect = {icon_x, icon_y, icon->w, icon->h};
            SDL_RenderCopy(renderer, icon_texture, NULL, &icon_rect);
            SDL_DestroyTexture(icon_texture);
            SDL_FreeSurface(icon);
        }
    }

    // Display character avatars, names, and hearts in the top right corner
    int avatar_x = 60;
    int avatar_y = 40;
    for (int i = 1; i < MAX_CHARACTERS; i++) // Start the loop from 1 to skip the first avatar
    {
        if (strlen(gameData->characters[i].name) > 0)
        {
            // Load and render avatar
            SDL_Surface *avatar = load_image(gameData->characters[i].avatar);
            if (avatar)
            {
                SDL_Texture *avatar_texture = SDL_CreateTextureFromSurface(renderer, avatar);
                if (avatar_texture)
                {
                    SDL_Rect avatar_rect = {avatar_x, avatar_y, avatar->w, avatar->h};
                    SDL_RenderCopy(renderer, avatar_texture, NULL, &avatar_rect);
                    SDL_DestroyTexture(avatar_texture);
                }
                SDL_FreeSurface(avatar);
            }

            // Render character name
            render_text(gameData->characters[i].name, avatar_x, avatar_y + 100);

            // Render character heart
            char heart_text[50];
            snprintf(heart_text, sizeof(heart_text), "好感度: %d", gameData->characters[i].heart);
            render_text(heart_text, avatar_x, avatar_y + 130);

            avatar_y += 150;
        }
    }

    SDL_RenderPresent(renderer);

    // Display inventory on the screen
    int text_x = window_width * 0.8;
    int text_y = window_height * 0.1;
    render_text("背包:", text_x, text_y);
    text_y += 30;
    for (int i = 0; i < player->inventory_count; i++)
    {
        render_text(player->inventory[i], text_x, text_y + i * 30);
    }
    SDL_RenderPresent(renderer);
}

void display_inventory_screen(GameData *gameData, Player *player)
{
    if (!gameData)
    {
        fprintf(stderr, "gameData 為空指針\n");
        return;
    }
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
    for (int i = 0; i < player->inventory_count; i++)
    {
        // Render item icon
        int item_index = find_item_index(gameData, player->inventory[i]);
        if (item_index != -1)
        {
            const char *icon_path = gameData->items[item_index].icon;
            SDL_Surface *icon = load_image(icon_path);
            if (icon)
            {
                SDL_Texture *icon_texture = SDL_CreateTextureFromSurface(renderer, icon);
                if (icon_texture)
                {
                    SDL_Rect icon_rect = {text_x, text_y + i * 50, icon->w, icon->h};
                    SDL_RenderCopy(renderer, icon_texture, NULL, &icon_rect);
                    SDL_DestroyTexture(icon_texture);
                }
                SDL_FreeSurface(icon);
            }
        }

        // Render item name
        render_text(player->inventory[i], text_x + 50, text_y + i * 50);
    }

    // Render exit instruction
    render_text("按ESC退出背包", text_x, text_y + player->inventory_count * 30 + 50);
    SDL_RenderPresent(renderer);

    // Wait for user to exit inventory
    SDL_Event e;
    while (1)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                cleanup_ui();
                exit(0);
            }
            else if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    return;
                }
            }
        }
        SDL_Delay(100);
    }
}

void start_dialogue(GameData *gameData, int dialogue_index)
{
    current_game_data = gameData;
    current_dialogue_index = dialogue_index;
    dialogue_state = TEXT1;
    display_dialogue(gameData, dialogue_index);
}

void display_dialogue(GameData *gameData, int dialogue_index)
{
    if (!gameData)
    {
        fprintf(stderr, "gameData 為空指針\n");
        return;
    }

    if (dialogue_index < 0 || dialogue_index >= MAX_DIALOGUES)
    {
        fprintf(stderr, "無效的 dialogue_index: %d\n", dialogue_index);
        return;
    }

    SDL_GetWindowSize(window, &window_width, &window_height);

    // Make the dialogue box bigger and move it more to the right
    SDL_Rect dialogueRect = {window_width / 8, window_height - 280, 1200, 250};

    // Check if dialogue box texture is available
    if (dialogue_box_texture)
    {
        SDL_RenderCopy(renderer, dialogue_box_texture, NULL, &dialogueRect);
    }
    else
    {
        // If no texture is available, fill the rectangle with a solid color
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128); // White color
        SDL_RenderFillRect(renderer, &dialogueRect);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128); // Reset to default draw color (black)
        SDL_RenderDrawRect(renderer, &dialogueRect);    // Draw the border
    }

    // Adjust text positions towards the right
    int text_x = dialogueRect.x + 48;
    int text_y = dialogueRect.y + 20;

    // Render character name
    render_text(gameData->dialogues[dialogue_index].character, text_x, text_y);
    text_y += 62; // Adjust Y coordinate to avoid overlap with character name

    // Render character tachie (standing picture)
    int char_index = find_character_index(gameData, gameData->dialogues[dialogue_index].character);
    if (char_index != -1)
    {
        const char *tachie_path = gameData->characters[char_index].tachie;
        SDL_Surface *tachie = load_image(tachie_path);
        if (tachie)
        {
            SDL_Texture *tachie_texture = SDL_CreateTextureFromSurface(renderer, tachie);
            if (tachie_texture)
            {
                int tachie_x = (int)(window_width * 0.75);
                int tachie_y = (int)(window_height * 0.1);
                SDL_Rect tachie_rect = {tachie_x, tachie_y, tachie->w, tachie->h};
                SDL_RenderCopy(renderer, tachie_texture, NULL, &tachie_rect);
                SDL_DestroyTexture(tachie_texture);
            }
            SDL_FreeSurface(tachie);
        }
    }

    switch (dialogue_state)
    {
    case TEXT1:
        render_text(gameData->dialogues[dialogue_index].text1, text_x, text_y);
        dialogue_state = (gameData->dialogues[dialogue_index].text2[0] != '\0') ? TEXT2 : OPTIONS;
        break;
    case TEXT2:
        render_text(gameData->dialogues[dialogue_index].text2, text_x, text_y);
        dialogue_state = (gameData->dialogues[dialogue_index].text3[0] != '\0') ? TEXT3 : OPTIONS;
        break;
    case TEXT3:
        render_text(gameData->dialogues[dialogue_index].text3, text_x, text_y);
        dialogue_state = OPTIONS;
        break;
    case OPTIONS:
        for (int i = 0; i < MAX_DIALOGUE_OPTIONS; i++)
        {
            if (strlen(gameData->dialogues[dialogue_index].options[i].text) > 0)
            {
                char option_text[256];
                snprintf(option_text, sizeof(option_text), "%d. %s", i + 1, gameData->dialogues[dialogue_index].options[i].text);
                render_text(option_text, text_x, text_y + i * 30);
            }
        }
        break;
    }

    SDL_RenderPresent(renderer);
}

void delayed_quit(int milliseconds)
{
    SDL_Delay(milliseconds);
    cleanup_ui();
    exit(0);
}

int get_user_choice()
{
    SDL_Event e;
    while (1)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                cleanup_ui();
                exit(0);
            }
            else if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                case SDLK_1:
                    return 1;
                case SDLK_2:
                    return 2;
                case SDLK_3:
                    return 3;
                case SDLK_4:
                    return 4;
                case SDLK_5:
                    return 5;
                case SDLK_i: // 使用 i 鍵打開背包
                    return 0;
                case SDLK_ESCAPE:
                    cleanup_ui();
                    exit(0);
                default:
                    break;
                }
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                if (e.button.button == SDL_BUTTON_LEFT && dialogue_state == OPTIONS)
                {
                    int text_x = window_width / 10 + 134;
                    int text_y = window_height - 350 + 150; // Increased Y coordinate for better alignment

                    for (int i = 0; i < MAX_DIALOGUE_OPTIONS; i++)
                    {
                        if (strlen(current_game_data->dialogues[current_dialogue_index].options[i].text) > 0)
                        {
                            int text_width, text_height;
                            TTF_SizeUTF8(font, current_game_data->dialogues[current_dialogue_index].options[i].text, &text_width, &text_height);

                            SDL_Rect option_rect = {text_x, text_y + i * (text_height), text_width, text_height}; // Adjust padding between options

                            // Render the clickable area for debugging
                            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 128); // Red color with 50% opacity
                            SDL_RenderFillRect(renderer, &option_rect);
                            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Reset to black color

                            if (e.button.x >= option_rect.x && e.button.x <= option_rect.x + option_rect.w &&
                                e.button.y >= option_rect.y && e.button.y <= option_rect.y + option_rect.h)
                            {
                                if (strcmp(current_game_data->dialogues[current_dialogue_index].options[i].action, "end_game") == 0)
                                {
                                    delayed_quit(3000); // Delay for 3 seconds before quitting
                                }
                                return i + 1; // Return the option number (1-based index)
                            }
                        }
                    }
                    SDL_RenderPresent(renderer); // Update the renderer to show the debug rectangles
                }
                else
                {
                    if (dialogue_state == TEXT1)
                    {
                        if (strlen(current_game_data->dialogues[current_dialogue_index].text2) > 0)
                        {
                            dialogue_state = TEXT2;
                        }
                        else
                        {
                            dialogue_state = OPTIONS;
                        }
                    }
                    else if (dialogue_state == TEXT2)
                    {
                        if (strlen(current_game_data->dialogues[current_dialogue_index].text3) > 0)
                        {
                            dialogue_state = TEXT3;
                        }
                        else
                        {
                            dialogue_state = OPTIONS;
                        }
                    }
                    else if (dialogue_state == TEXT3)
                    {
                        dialogue_state = OPTIONS;
                    }
                    display_dialogue(current_game_data, current_dialogue_index);
                }
            }
        }
        SDL_Delay(100);
    }
    return -1;
}
