#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game_engine.h"
#include "ui_gui.h"

void add_item_to_inventory(Player *player, Item *item)
{
    if (player->inventory_count < MAX_INVENTORY_SIZE)
    {
        size_t name_length = strlen(item->name);
        if (name_length >= MAX_ITEM_NAME_LENGTH)
        {
            name_length = MAX_ITEM_NAME_LENGTH - 1;
        }
        strncpy(player->inventory[player->inventory_count], item->name, name_length);
        player->inventory[player->inventory_count][name_length] = '\0'; // 確保字符串終止
        player->inventory_count++;
        printf("物品 %s 已添加到背包\n", item->name);
    }
    else
    {
        printf("背包已滿,無法添加物品 %s\n", item->name);
    }
}

void update_emotion(Player *player, int change)
{
    player->emotion += change;
    if (player->emotion > 100)
        player->emotion = 100;
    if (player->emotion < 0)
        player->emotion = 0;
}

void display_emotion(Player *player)
{
    printf("%s 的情感值: %d\n", player->name, player->emotion);
}

void display_inventory(Player *player)
{
    printf("背包內容:\n");
    for (int i = 0; i < player->inventory_count; i++)
    {
        printf("%s\n", player->inventory[i]);
    }
}

int find_scene_index(GameData *gameData, const char *scene_name)
{
    for (int i = 0; i < MAX_SCENES; i++)
    {
        if (strcmp(gameData->scenes[i].name, scene_name) == 0)
        {
            return i;
        }
    }
    return -1;
}

int find_dialogue_index(GameData *gameData, const char *dialogue_name)
{
    for (int i = 0; i < MAX_DIALOGUES; i++)
    {
        if (strcmp(gameData->dialogues[i].name, dialogue_name) == 0)
        {
            return i;
        }
    }
    return -1;
}

int find_item_index(GameData *gameData, const char *item_name)
{
    for (int i = 0; i < MAX_ITEMS; i++)
    {
        if (strcmp(gameData->items[i].name, item_name) == 0)
        {
            return i;
        }
    }
    return -1;
}

int find_character_index(GameData *gameData, const char *character_name)
{
    for (int i = 0; i < MAX_CHARACTERS; i++)
    {
        if (strcmp(gameData->characters[i].name, character_name) == 0)
        {
            return i;
        }
    }
    return -1;
}

void update_character_heart(GameData *gameData, const char *character_name, int amount)
{
    int char_index = find_character_index(gameData, character_name);
    if (char_index != -1)
    {
        gameData->characters[char_index].heart += amount;
        printf("更新 %s 的好感度: %d\n", character_name, gameData->characters[char_index].heart);
    }
    else
    {
        printf("找不到角色: %s\n", character_name);
    }
}

void display_character_hearts(GameData *gameData)
{
    printf("角色好感度:\n");
    for (int i = 0; i < MAX_CHARACTERS; i++)
    {
        if (strlen(gameData->characters[i].name) > 0)
        {
            printf("%s: %d\n", gameData->characters[i].name, gameData->characters[i].heart);
        }
    }
}

void handle_event(GameData *gameData, Player *player, const char *event_name)
{
    fprintf(stderr, "進入 handle_event 函數, 事件名稱: %s\n", event_name);

    int found_event = 0;
    for (int i = 0; i < MAX_EVENTS; i++)
    {
        if (strcmp(gameData->events[i].name, event_name) == 0)
        {
            found_event = 1;
            fprintf(stderr, "事件名稱匹配: %s\n", gameData->events[i].name);
            fprintf(stderr, "事件動作: %s\n", gameData->events[i].action);
            if (strcmp(gameData->events[i].action, "change_scene") == 0)
            {
                fprintf(stderr, "事件動作是切換場景\n");
                int scene_index = find_scene_index(gameData, gameData->events[i].scene);
                if (scene_index != -1)
                {
                    fprintf(stderr, "切換場景到: %s\n", gameData->events[i].scene);
                    player->current_scene = scene_index;
                }
                else
                {
                    fprintf(stderr, "找不到場景: %s\n", gameData->events[i].scene);
                }
            }
            else if (strcmp(gameData->events[i].action, "add_item") == 0)
            {
                fprintf(stderr, "事件動作是添加物品\n");
                int item_index = find_item_index(gameData, gameData->events[i].item);
                if (item_index != -1)
                {
                    fprintf(stderr, "找到物品: %s, 索引: %d\n", gameData->events[i].item, item_index);
                    add_item_to_inventory(player, &gameData->items[item_index]);
                    fprintf(stderr, "添加物品後的背包內容:\n");
                    display_inventory(player);
                }
                else
                {
                    fprintf(stderr, "無效的物品名稱: %s\n", gameData->events[i].item);
                }
            }
            else if (strcmp(gameData->events[i].action, "update_heart") == 0)
            {
                fprintf(stderr, "事件動作是更新好感度\n");
                update_character_heart(gameData, gameData->events[i].character, gameData->events[i].amount);
            }
            else if (strcmp(gameData->events[i].action, "end_game") == 0)
            {
                fprintf(stderr, "遊戲結束: %s\n", gameData->events[i].result);
                exit(0); // 或者其他方式結束遊戲
            }
            if (strlen(gameData->events[i].dialogue) > 0)
            {
                int dialogue_index = find_dialogue_index(gameData, gameData->events[i].dialogue);
                if (dialogue_index != -1)
                {
                    fprintf(stderr, "更新對話到: %s\n", gameData->events[i].dialogue);
                    player->current_dialogue = dialogue_index;
                }
                else
                {
                    fprintf(stderr, "找不到對話: %s\n", gameData->events[i].dialogue);
                }
            }
            break;
        }
    }

    if (!found_event)
    {
        fprintf(stderr, "找不到名為 %s 的事件\n", event_name);
    }

    fprintf(stderr, "離開 handle_event 函數\n");
    fflush(stdout);
}

int play_game(GameData *gameData)
{
    Player player = {"Player", 50, {{0}}, 0, 0, 0};
    player.current_scene = find_scene_index(gameData, "school-gate");
    if (player.current_scene == -1)
    {
        fprintf(stderr, "找不到初始場景: school-bus\n");
        return 1;
    }

    int dialogue_index = find_dialogue_index(gameData, "bus_introduction");
    if (dialogue_index == -1)
    {
        fprintf(stderr, "找不到初始對話: bus_introduction\n");
        return 1;
    }
    player.current_dialogue = dialogue_index;

    while (1)
    {
        fprintf(stderr, "當前場景: %s\n", gameData->scenes[player.current_scene].name);
        display_scene(gameData, gameData->scenes[player.current_scene].name, &player);
        fprintf(stderr, "顯示對話之前\n");
        start_dialogue(gameData, player.current_dialogue);
        fprintf(stderr, "顯示對話之後\n");
        display_inventory(&player);
        int choice = get_user_choice();
        fprintf(stderr, "用戶選擇: %d\n", choice);

        if (choice == -1)
        {
            break;
        }

        if (choice == 0)
        {
            display_inventory_screen(gameData, &player);
            continue;
        }

        DialogueOption selected_option;
        if (choice == 1)
        {
            selected_option = gameData->dialogues[player.current_dialogue].options[0];
        }
        else if (choice == 2)
        {
            selected_option = gameData->dialogues[player.current_dialogue].options[1];
        }
        else if (choice == 3)
        {
            selected_option = gameData->dialogues[player.current_dialogue].options[2];
        }
        else if (choice == 4)
        {
            selected_option = gameData->dialogues[player.current_dialogue].options[3];
        }
        else
        {
            fprintf(stderr, "無效選擇\n");
            continue;
        }

        fprintf(stderr, "選中的對話選項: %s\n", selected_option.text);

        if (strlen(selected_option.event) > 0)
        {
            fprintf(stderr, "處理事件: %s\n", selected_option.event);
            handle_event(gameData, &player, selected_option.event);
        }
        else
        {
            fprintf(stderr, "選項沒有關聯的事件\n");
        }

        if (strlen(selected_option.next) > 0)
        {
            const char *next_dialogue = selected_option.next;
            fprintf(stderr, "查找對話: %s\n", next_dialogue);
            int next_dialogue_index = find_dialogue_index(gameData, next_dialogue);
            if (next_dialogue_index != -1)
            {
                fprintf(stderr, "更新對話到: %s\n", next_dialogue);
                player.current_dialogue = next_dialogue_index;
                const char *next_scene = gameData->dialogues[next_dialogue_index].scene;
                int next_scene_index = find_scene_index(gameData, next_scene);
                if (next_scene_index != -1)
                {
                    player.current_scene = next_scene_index;
                    fprintf(stderr, "更新場景到: %s\n", next_scene);
                }
                else
                {
                    fprintf(stderr, "找不到下一個對話的場景: %s\n", next_scene);
                }
            }
            else
            {
                fprintf(stderr, "找不到對話: %s\n", next_dialogue);
                break;
            }
        }
        else
        {
            fprintf(stderr, "選項的下一個對話為空\n");
        }
    }
    return 1;
}
