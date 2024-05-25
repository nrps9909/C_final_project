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

void display_emotion(Player *player) {
    printf("%s 的情感值: %d\n", player->name, player->emotion);
}

int find_scene_index(GameData *gameData, const char *scene_name) {
    for (int i = 0; i < 10; i++) {
        if (strcmp(gameData->scenes[i].name, scene_name) == 0) {
            return i;
        }
    }
    return -1;
}

int find_dialogue_index(GameData *gameData, const char *dialogue_name) {
    for (int i = 0; i < 20; i++) { // 假設對話陣列長度為20
        if (strcmp(gameData->dialogues[i].name, dialogue_name) == 0) {
            return i;
        }
    }
    return -1;
}

void handle_event(GameData *gameData, Player *player, const char *event_name) {
    for (int i = 0; i < 10; i++) {
        if (strcmp(gameData->events[i].scene, event_name) == 0) {
            if (strcmp(gameData->events[i].action, "add_item") == 0) {
                add_item_to_inventory(player, gameData->events[i].item);
            } else if (strcmp(gameData->events[i].action, "end_game") == 0) {
                printf("遊戲結束：%s\n", gameData->events[i].result);
                exit(0);
            }
        }
    }
}

void play_game(GameData *gameData) {
    Player player = {"Player", 50, {{0}}, 0};  // 初始玩家情感值為50，背包為空
    int current_dialogue = 0; // 起始對話索引

    while (1) {
        fprintf(stderr, "當前對話: %d\n", current_dialogue);
        display_scene(gameData, find_scene_index(gameData, gameData->events[current_dialogue].scene));

        // 顯示對話
        fprintf(stderr, "顯示對話之前\n");
        display_dialogue(gameData, current_dialogue);
        fprintf(stderr, "顯示對話之後\n");

        int choice = get_user_choice();
        if (choice == -1) {
            break;  // 用戶選擇退出
        } else if (choice == 1) {
            const char* next_dialogue = gameData->dialogues[current_dialogue].options[0].next;
            fprintf(stderr, "選項1: 下一個對話 %s\n", next_dialogue);
            current_dialogue = find_dialogue_index(gameData, next_dialogue);
            handle_event(gameData, &player, gameData->dialogues[current_dialogue].options[0].event);
        } else if (choice == 2) {
            const char* next_dialogue = gameData->dialogues[current_dialogue].options[1].next;
            fprintf(stderr, "選項2: 下一個對話 %s\n", next_dialogue);
            current_dialogue = find_dialogue_index(gameData, next_dialogue);
            handle_event(gameData, &player, gameData->dialogues[current_dialogue].options[1].event);
        } else {
            printf("無效選擇\n");
            continue;
        }

        if (current_dialogue == -1) {
            printf("找不到下一個對話\n");
            return;
        }
    }
}
