#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game_engine.h"
#include "script_parser.h"
#include "ui_gui.h"  // 添加這一行

typedef struct {
    char name[100];
    int emotion; // 情感數值
    char inventory[10][50]; // 背包
    int inventory_count;
} Player;

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
    int current_scene = find_scene_index(gameData, "library");
    while (1) {
        display_scene(gameData, current_scene);
        int choice = get_user_choice();
        if (choice == 1) {
            current_scene = find_scene_index(gameData, gameData->dialogues[current_scene].options[0].next);
        } else if (choice == 2) {
            current_scene = find_scene_index(gameData, gameData->dialogues[current_scene].options[1].next);
        }

        handle_event(gameData, &player, gameData->dialogues[current_scene].text);
    }
}
