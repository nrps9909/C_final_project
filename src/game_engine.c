#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game_engine.h"
#include "script_parser.h"
#include "ui_gui.h"

void add_item_to_inventory(Player *player, const char *item) {
    if (player->inventory_count < 10) {
        strcpy(player->inventory[player->inventory_count], item);
        player->inventory_count++;
    } else {
        printf("背包已滿！\n");
    }
}

void update_emotion(Player *player, int change) {
    player->emotion += change;
    if (player->emotion > 100) player->emotion = 100;
    if (player->emotion < 0) player->emotion = 0;
}

void display_emotion(Player* player) {
    printf("%s 的情感值: %d\n", player->name, player->emotion);
}

int find_scene_index(GameData *gameData, const char *scene_name) {
    for (int i = 0; i < 10; i++) {
        fprintf(stderr, "比較場景: %s\n", gameData->scenes[i].name);
        if (strcmp(gameData->scenes[i].name, scene_name) == 0) {
            return i;
        }
    }
    return -1;
}

int find_dialogue_index(GameData *gameData, const char *dialogue_name) {
    for (int i = 0; i < 16; i++) { // 假設對話數組長度為 16
        if (strcmp(gameData->dialogues[i].name, dialogue_name) == 0) {
            return i;
        }
    }
    return -1;
}

void handle_event(GameData *gameData, Player* player, const char *event_name) {
    printf("處理事件: %s\n", event_name);
    for (int i = 0; i < 10; i++) {
        if (strcmp(gameData->events[i].name, event_name) == 0) {
            printf("事件名稱匹配: %s\n", gameData->events[i].name);
            if (strcmp(gameData->events[i].action, "change_scene") == 0 ||
                strcmp(gameData->events[i].action, "change_scene_to_meeting_room") == 0) {
                printf("事件動作是切換場景\n");
                // 檢查事件中指定的場景是否與當前對話的場景一致
                int dialogue_index = find_dialogue_index(gameData, gameData->dialogues[player->current_scene].name);
                if (dialogue_index != -1 && strcmp(gameData->dialogues[dialogue_index].scene, gameData->events[i].scene) == 0) {
                    int scene_index = find_scene_index(gameData, gameData->events[i].scene);
                    if (scene_index != -1) {
                        printf("切換場景到: %s\n", gameData->events[i].scene);
                        player->current_scene = scene_index;
                    } else {
                        printf("找不到場景: %s\n", gameData->events[i].scene);
                    }
                } else {
                    printf("事件中指定的場景與當前對話的場景不一致,不進行場景切換\n");
                }
            } else if (strcmp(gameData->events[i].action, "add_item") == 0) {
                printf("事件動作是添加物品\n");
                add_item_to_inventory(player, gameData->events[i].item);
            } else if (strcmp(gameData->events[i].action, "end_game") == 0) {
                printf("遊戲結束: %s\n", gameData->events[i].result);
                exit(0);
            }
        }
    }
}


void play_game(GameData* gameData) {
    Player player = {"Player", 50, {{0}}, 0, 0}; // 初始化所有成員,包括 current_scene
    player.current_scene = find_scene_index(gameData, "library"); // 設置初始場景
    if (player.current_scene == -1) {
        printf("找不到初始場景: library\n");
        return;
    }

    int dialogue_index = find_dialogue_index(gameData, "hello");  // 初始對話索引
    if (dialogue_index == -1) {
        fprintf(stderr, "找不到初始對話: hello\n");
        return;
    }

    while (1) {
        fprintf(stderr, "當前場景: %s\n", gameData->scenes[player.current_scene].name);
        display_scene(gameData, gameData->scenes[player.current_scene].name);
        fprintf(stderr, "顯示對話之前\n");
        display_dialogue(gameData, dialogue_index);
        fprintf(stderr, "顯示對話之後\n");
        int choice = get_user_choice();
        fprintf(stderr, "用戶選擇: %d\n", choice);
        if (choice == -1) {
            break;
        }

        // 根據用戶選擇執行對應的事件
        DialogueOption selected_option;
        if (choice == 1) {
            selected_option = gameData->dialogues[dialogue_index].options[0];
        } else if (choice == 2) {
            selected_option = gameData->dialogues[dialogue_index].options[1];
        } else {
            printf("無效選擇\n");
            continue;
        }

        // 打印選中的對話選項
        printf("選中的對話選項: %s\n", selected_option.text);

        // 處理事件 (如果有)
        if (strlen(selected_option.event) > 0) {
            printf("處理事件: %s\n", selected_option.event);
            int old_scene = player.current_scene;
            handle_event(gameData, &player, selected_option.event);
            if (old_scene != player.current_scene) {
                fprintf(stderr, "事件導致場景變化,從 %s 變為 %s\n", gameData->scenes[old_scene].name, gameData->scenes[player.current_scene].name);
            } else {
                fprintf(stderr, "事件沒有導致場景變化\n");
            }
        } else {
            fprintf(stderr, "選項沒有關聯的事件\n");
        }

        // 更新對話
        if (strlen(selected_option.next) > 0) {
            const char *next_dialogue = selected_option.next;
            fprintf(stderr, "查找對話: %s\n", next_dialogue);
            int next_dialogue_index = find_dialogue_index(gameData, next_dialogue);
            if (next_dialogue_index != -1) {
                printf("更新對話到: %s\n", next_dialogue);
                dialogue_index = next_dialogue_index;  // 更新對話索引
                // 根據下一個對話的場景屬性更新 player.current_scene
                const char *next_scene = gameData->dialogues[next_dialogue_index].scene;
                int next_scene_index = find_scene_index(gameData, next_scene);
                if (next_scene_index != -1) {
                    player.current_scene = next_scene_index;
                } else {
                    fprintf(stderr, "找不到下一個對話的場景: %s\n", next_scene);
                }
            } else {
                fprintf(stderr, "找不到對話: %s\n", next_dialogue);
            }
        } else {
            fprintf(stderr, "選項的下一個對話為空\n");
        }
    }
}