#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <SDL2/SDL.h>
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

void display_inventory(Player *player) {
    printf("背包內容:\n");
    for (int i = 0; i < player->inventory_count; i++) {
        printf("%d. %s\n", i + 1, player->inventory[i]);
    }
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
    for (int i = 0; i < 19; i++) {
        if (strcmp(gameData->dialogues[i].name, dialogue_name) == 0) {
            return i;
        }
    }
    return -1;
}

void handle_event(GameData *gameData, Player *player, const char *event_name) {
    printf("處理事件: %s\n", event_name);
    for (int i = 0; i < 10; i++) {
        if (strcmp(gameData->events[i].name, event_name) == 0) {
            printf("事件名稱匹配: %s\n", gameData->events[i].name);
            if (strcmp(gameData->events[i].action, "change_scene") == 0) {
                printf("事件動作是切換場景\n");
                int scene_index = find_scene_index(gameData, gameData->events[i].scene);
                if (scene_index != -1) {
                    printf("切換場景到: %s\n", gameData->events[i].scene);
                    player->current_scene = scene_index;
                } else {
                    printf("找不到場景: %s\n", gameData->events[i].scene);
                }
            } else if (strcmp(gameData->events[i].action, "add_item") == 0) {
                printf("事件動作是添加物品\n");
                add_item_to_inventory(player, gameData->events[i].item);
            } else if (strcmp(gameData->events[i].action, "end_game") == 0) {
                printf("遊戲結束: %s\n", gameData->events[i].result);
                exit(0); // 或者其他方式結束遊戲
            }
            // 處理事件後轉到對應的對話
            if (strlen(gameData->events[i].dialogue) > 0) {
                int dialogue_index = find_dialogue_index(gameData, gameData->events[i].dialogue);
                if (dialogue_index != -1) {
                    printf("更新對話到: %s\n", gameData->events[i].dialogue);
                    player->current_dialogue = dialogue_index;
                } else {
                    printf("找不到對話: %s\n", gameData->events[i].dialogue);
                }
            }
            break; // 找到匹配的事件後退出循環
        }
    }
}

void play_game(GameData* gameData) {
    Player player = {"Player", 50, {{0}}, 0, 0, 0};  // 添加 0 初始化 current_dialogue
    player.current_scene = find_scene_index(gameData, "library"); 
    if (player.current_scene == -1) {
        printf("找不到初始場景: library\n");
        return;
    }

    int dialogue_index = find_dialogue_index(gameData, "hello");  
    if (dialogue_index == -1) {
        fprintf(stderr, "找不到初始對話: hello\n");
        return;
    }
    player.current_dialogue = dialogue_index;

    while (1) {
        fprintf(stderr, "當前場景: %s\n", gameData->scenes[player.current_scene].name);
        display_scene(gameData, gameData->scenes[player.current_scene].name, &player);  // 傳遞 player
        fprintf(stderr, "顯示對話之前\n");
        display_dialogue(gameData, player.current_dialogue);
        fprintf(stderr, "顯示對話之後\n");
        display_inventory(&player);  // 顯示背包內容
        int choice = get_user_choice();
        fprintf(stderr, "用戶選擇: %d\n", choice);

        if (choice == -1) {
            break;
        }

        if (choice == 0) {  // 如果玩家選擇開啟背包
            display_inventory_screen(&player);
            continue;
        }

        DialogueOption selected_option;
        if (choice == 1) {
            selected_option = gameData->dialogues[player.current_dialogue].options[0];
        } else if (choice == 2) {
            selected_option = gameData->dialogues[player.current_dialogue].options[1];
        } else {
            printf("無效選擇\n");
            continue;
        }

        printf("選中的對話選項: %s\n", selected_option.text);

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

        if (strlen(selected_option.next) > 0) {
            const char *next_dialogue = selected_option.next;
            fprintf(stderr, "查找對話: %s\n", next_dialogue);
            int next_dialogue_index = find_dialogue_index(gameData, next_dialogue);
            if (next_dialogue_index != -1) {
                printf("更新對話到: %s\n", next_dialogue);
                player.current_dialogue = next_dialogue_index; 
                const char *next_scene = gameData->dialogues[next_dialogue_index].scene;
                int next_scene_index = find_scene_index(gameData, next_scene);
                if (next_scene_index != -1) {
                    player.current_scene = next_scene_index;
                } else {
                    fprintf(stderr, "找不到下一個對話的場景: %s\n", next_scene);
                }
            } else {
                fprintf(stderr, "找不到對話: %s\n", next_dialogue);
                break; 
            }
        } else {
            fprintf(stderr, "選項的下一個對話為空\n");
        }
    }
}
