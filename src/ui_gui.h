#ifndef UI_GUI_H
#define UI_GUI_H

#include "game_engine.h"

// Function prototypes
void init_ui();
void cleanup_ui();
void display_scene(GameData *gameData, const char *scene_name, Player *player); // Updated signature
void display_inventory_screen(GameData *gameData, Player *player);
void start_dialogue(GameData *gameData, int dialogue_index);
void display_dialogue(GameData *gameData, int dialogue_index);
int get_user_choice();

#endif // UI_GUI_H
