#ifndef UI_GUI_H
#define UI_GUI_H

#include "game_engine.h"

void init_ui();
void cleanup_ui();
void display_scene(GameData *gameData, const char* scene_name, Player *player);
void display_dialogue(GameData* gameData, int dialogue_index);
int get_user_choice();
void display_inventory(Player *player);
void display_inventory_screen(GameData *gameData, Player *player); // 修改函數原型以包含 gameData
void start_dialogue(GameData* gameData, int dialogue_index); // 新增函數原型

#endif // UI_GUI_H
