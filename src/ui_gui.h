#ifndef UI_GUI_H
#define UI_GUI_H

#include "game_engine.h"

void init_ui();
void cleanup_ui();
void display_scene(GameData *gameData, int scene_index);
void display_dialogue(GameData* gameData, int dialogue_index);
int get_user_choice();

#endif // UI_GUI_H
