#ifndef UI_GUI_H
#define UI_GUI_H

#include "game_engine.h"

void init_ui();
void cleanup_ui();
void display_scene(GameData *gameData, const char* scene_name, Player *player);
void display_dialogue(GameData* gameData, int dialogue_index);
int get_user_choice();
void display_inventory(Player *player);
void display_inventory_screen(Player *player);

#endif // UI_GUI_H
