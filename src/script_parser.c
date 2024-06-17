#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "script_parser.h"

void copy_string(char *dest, const char *src, size_t dest_size)
{
    if (dest && src)
    {
        strncpy(dest, src, dest_size - 1);
        dest[dest_size - 1] = '\0';
    }
}

void parse_scenes(toml_table_t *root, GameData *gameData)
{
    toml_table_t *scenes = toml_table_in(root, "scene");
    if (scenes)
    {
        const char *scene_key;
        int scene_index = 0;
        while ((scene_key = toml_key_in(scenes, scene_index)) != NULL && scene_index < MAX_SCENES)
        {
            toml_table_t *scene = toml_table_in(scenes, scene_key);
            if (scene)
            {
                toml_raw_t raw_background = toml_raw_in(scene, "background");
                if (raw_background)
                {
                    char *background;
                    if (toml_rtos(raw_background, &background) == 0)
                    {
                        copy_string(gameData->scenes[scene_index].name, scene_key, sizeof(gameData->scenes[scene_index].name));
                        copy_string(gameData->scenes[scene_index].background, background, sizeof(gameData->scenes[scene_index].background));
                        free(background);
                        printf("解析場景: %s, 背景: %s\n", gameData->scenes[scene_index].name, gameData->scenes[scene_index].background);
                    }
                }
            }
            scene_index++;
        }
    }
}

void parse_characters(toml_table_t *root, GameData *gameData)
{
    toml_table_t *characters = toml_table_in(root, "character");
    if (characters)
    {
        const char *char_key;
        int char_index = 0;
        while ((char_key = toml_key_in(characters, char_index)) != NULL && char_index < MAX_CHARACTERS)
        {
            toml_table_t *character = toml_table_in(characters, char_key);
            if (character)
            {
                toml_raw_t raw_name = toml_raw_in(character, "name");
                toml_raw_t raw_avatar = toml_raw_in(character, "avatar");
                toml_raw_t raw_tachie = toml_raw_in(character, "tachie");
                toml_raw_t raw_location = toml_raw_in(character, "location");
                if (raw_name && raw_avatar && raw_tachie)
                {
                    char *name = NULL;
                    char *avatar = NULL;
                    char *tachie = NULL;
                    char *location = NULL;

                    if (toml_rtos(raw_name, &name) == 0 &&
                        toml_rtos(raw_avatar, &avatar) == 0 &&
                        toml_rtos(raw_tachie, &tachie) == 0)
                    {

                        if (raw_location && toml_rtos(raw_location, &location) == 0)
                        {
                            copy_string(gameData->characters[char_index].location, location, sizeof(gameData->characters[char_index].location));
                            free(location);
                        }

                        copy_string(gameData->characters[char_index].name, name, sizeof(gameData->characters[char_index].name));
                        copy_string(gameData->characters[char_index].avatar, avatar, sizeof(gameData->characters[char_index].avatar));
                        copy_string(gameData->characters[char_index].tachie, tachie, sizeof(gameData->characters[char_index].tachie));
                        gameData->characters[char_index].heart = 60; // 預設好感度為60

                        free(name);
                        free(avatar);
                        free(tachie);

                        printf("解析角色: %s, 頭像: %s, 立繪: %s, 位置: %s, 初始好感度: %d\n",
                               gameData->characters[char_index].name,
                               gameData->characters[char_index].avatar,
                               gameData->characters[char_index].tachie,
                               gameData->characters[char_index].location,
                               gameData->characters[char_index].heart);
                    }
                    else
                    {
                        printf("解析角色時失敗: %s\n", char_key);
                    }
                }
            }
            char_index++;
        }
    }
}

void parse_items(toml_table_t *root, GameData *gameData)
{
    toml_table_t *items = toml_table_in(root, "item");
    if (items)
    {
        const char *item_key;
        int item_index = 0;
        while ((item_key = toml_key_in(items, item_index)) != NULL && item_index < MAX_ITEMS)
        {
            toml_table_t *item = toml_table_in(items, item_key);
            if (item)
            {
                toml_raw_t raw_name = toml_raw_in(item, "name");
                toml_raw_t raw_description = toml_raw_in(item, "description");
                toml_raw_t raw_icon = toml_raw_in(item, "icon");
                toml_raw_t raw_scene = toml_raw_in(item, "scene");
                if (raw_name && raw_description && raw_icon && raw_scene)
                {
                    char *name = NULL;
                    char *description = NULL;
                    char *icon = NULL;
                    char *scene = NULL;

                    if (toml_rtos(raw_name, &name) == 0 &&
                        toml_rtos(raw_description, &description) == 0 &&
                        toml_rtos(raw_icon, &icon) == 0 &&
                        toml_rtos(raw_scene, &scene) == 0)
                    {

                        copy_string(gameData->items[item_index].name, name, sizeof(gameData->items[item_index].name));
                        copy_string(gameData->items[item_index].description, description, sizeof(gameData->items[item_index].description));
                        copy_string(gameData->items[item_index].icon, icon, sizeof(gameData->items[item_index].icon));
                        copy_string(gameData->items[item_index].scene, scene, sizeof(gameData->items[item_index].scene));

                        free(name);
                        free(description);
                        free(icon);
                        free(scene);

                        printf("解析物品: %s, 描述: %s\n", gameData->items[item_index].name, gameData->items[item_index].description);
                    }
                }
            }
            item_index++;
        }
    }
}

void parse_dialogues(toml_table_t *root, GameData *gameData)
{
    toml_table_t *dialogues = toml_table_in(root, "dialogue");
    if (dialogues)
    {
        const char *dialogue_key;
        int dialogue_index = 0;
        while ((dialogue_key = toml_key_in(dialogues, dialogue_index)) != NULL && dialogue_index < MAX_DIALOGUES)
        {
            toml_table_t *dialogue = toml_table_in(dialogues, dialogue_key);
            if (dialogue)
            {
                copy_string(gameData->dialogues[dialogue_index].name, dialogue_key, sizeof(gameData->dialogues[dialogue_index].name));
                fprintf(stderr, "解析到對話: %s\n", gameData->dialogues[dialogue_index].name);
                toml_raw_t raw_scene = toml_raw_in(dialogue, "scene");
                toml_raw_t raw_character = toml_raw_in(dialogue, "character");
                toml_raw_t raw_text1 = toml_raw_in(dialogue, "text1");
                toml_raw_t raw_text2 = toml_raw_in(dialogue, "text2");
                toml_raw_t raw_text3 = toml_raw_in(dialogue, "text3");
                toml_raw_t raw_text4 = toml_raw_in(dialogue, "text4"); // 新增對text4的解析
                toml_array_t *options = toml_array_in(dialogue, "options");

                if (raw_scene && raw_character && raw_text1)
                {
                    char *scene = NULL;
                    char *character = NULL;
                    char *text1 = NULL;
                    char *text2 = NULL;
                    char *text3 = NULL;
                    char *text4 = NULL; // 新增text4的變量

                    if (toml_rtos(raw_scene, &scene) == 0 &&
                        toml_rtos(raw_character, &character) == 0 &&
                        toml_rtos(raw_text1, &text1) == 0)
                    {

                        copy_string(gameData->dialogues[dialogue_index].scene, scene, sizeof(gameData->dialogues[dialogue_index].scene));
                        copy_string(gameData->dialogues[dialogue_index].character, character, sizeof(gameData->dialogues[dialogue_index].character));
                        copy_string(gameData->dialogues[dialogue_index].text1, text1, sizeof(gameData->dialogues[dialogue_index].text1));

                        if (raw_text2 && toml_rtos(raw_text2, &text2) == 0)
                        {
                            copy_string(gameData->dialogues[dialogue_index].text2, text2, sizeof(gameData->dialogues[dialogue_index].text2));
                            free(text2);
                        }
                        else
                        {
                            gameData->dialogues[dialogue_index].text2[0] = '\0';
                        }

                        if (raw_text3 && toml_rtos(raw_text3, &text3) == 0)
                        {
                            copy_string(gameData->dialogues[dialogue_index].text3, text3, sizeof(gameData->dialogues[dialogue_index].text3));
                            free(text3);
                        }
                        else
                        {
                            gameData->dialogues[dialogue_index].text3[0] = '\0';
                        }

                        if (raw_text4 && toml_rtos(raw_text4, &text4) == 0)
                        { // 新增text4的處理
                            copy_string(gameData->dialogues[dialogue_index].text4, text4, sizeof(gameData->dialogues[dialogue_index].text4));
                            free(text4);
                        }
                        else
                        {
                            gameData->dialogues[dialogue_index].text4[0] = '\0';
                        }

                        free(scene);
                        free(character);
                        free(text1);

                        printf("解析對話: %s, 場景: %s, 角色: %s, 內容1: %s, 內容2: %s, 內容3: %s, 內容4: %s\n",
                               gameData->dialogues[dialogue_index].name,
                               gameData->dialogues[dialogue_index].scene,
                               gameData->dialogues[dialogue_index].character,
                               gameData->dialogues[dialogue_index].text1,
                               gameData->dialogues[dialogue_index].text2,
                               gameData->dialogues[dialogue_index].text3,
                               gameData->dialogues[dialogue_index].text4); // 新增text4的輸出

                        if (options)
                        {
                            for (int j = 0; j < toml_array_nelem(options) && j < MAX_DIALOGUE_OPTIONS; j++)
                            {
                                toml_table_t *option = toml_table_at(options, j);
                                toml_raw_t raw_option_text = toml_raw_in(option, "text");
                                toml_raw_t raw_next = toml_raw_in(option, "next");
                                toml_raw_t raw_event = toml_raw_in(option, "event");
                                if (raw_option_text && raw_next)
                                {
                                    char *option_text = NULL;
                                    char *next = NULL;
                                    char *event = NULL;

                                    if (toml_rtos(raw_option_text, &option_text) == 0 &&
                                        toml_rtos(raw_next, &next) == 0)
                                    {

                                        if (raw_event && toml_rtos(raw_event, &event) == 0)
                                        {
                                            copy_string(gameData->dialogues[dialogue_index].options[j].event, event, sizeof(gameData->dialogues[dialogue_index].options[j].event));
                                            free(event);
                                        }

                                        copy_string(gameData->dialogues[dialogue_index].options[j].text, option_text, sizeof(gameData->dialogues[dialogue_index].options[j].text));
                                        copy_string(gameData->dialogues[dialogue_index].options[j].next, next, sizeof(gameData->dialogues[dialogue_index].options[j].next));

                                        free(option_text);
                                        free(next);

                                        printf("解析對話選項: %s, 下一個對話: %s, 事件: %s\n",
                                               gameData->dialogues[dialogue_index].options[j].text,
                                               gameData->dialogues[dialogue_index].options[j].next,
                                               gameData->dialogues[dialogue_index].options[j].event);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            dialogue_index++;
        }
    }
}

void parse_initial_heart_points(toml_table_t *root, GameData *gameData)
{
    toml_table_t *initial_heart_points = toml_table_in(root, "initial_heart_points");
    if (initial_heart_points)
    {
        const char *char_name;
        int char_index = 0;
        while ((char_name = toml_key_in(initial_heart_points, char_index)) != NULL)
        {
            int64_t heart_points;
            toml_datum_t heart_points_datum = toml_int_in(initial_heart_points, char_name);
            if (!heart_points_datum.ok)
            {
                fprintf(stderr, "錯誤: 無法獲取角色 %s 的初始好感度\n", char_name);
                continue;
            }
            heart_points = heart_points_datum.u.i;
            for (int i = 0; i < MAX_CHARACTERS; i++)
            {
                if (strcmp(gameData->characters[i].name, char_name) == 0)
                {
                    gameData->characters[i].heart = (int)heart_points;
                    printf("設置角色 %s 的初始好感度為 %d\n", char_name, (int)heart_points);
                    break;
                }
            }
            char_index++;
        }
    }
}

void parse_events(toml_table_t *root, GameData *gameData)
{
    toml_table_t *events = toml_table_in(root, "event");
    if (events)
    {
        const char *event_key;
        int event_index = 0;
        while ((event_key = toml_key_in(events, event_index)) != NULL && event_index < MAX_EVENTS)
        {
            toml_table_t *event = toml_table_in(events, event_key);
            if (event)
            {
                copy_string(gameData->events[event_index].name, event_key, sizeof(gameData->events[event_index].name));
                toml_raw_t raw_scene = toml_raw_in(event, "scene");
                toml_raw_t raw_dialogue = toml_raw_in(event, "dialogue");
                toml_raw_t raw_action = toml_raw_in(event, "action");
                toml_raw_t raw_result = toml_raw_in(event, "result");
                toml_raw_t raw_item = toml_raw_in(event, "item");
                toml_raw_t raw_character = toml_raw_in(event, "character");
                toml_raw_t raw_amount = toml_raw_in(event, "amount");

                if (raw_scene)
                {
                    char *scene;
                    if (toml_rtos(raw_scene, &scene) == 0)
                    {
                        copy_string(gameData->events[event_index].scene, scene, sizeof(gameData->events[event_index].scene));
                        free(scene);
                    }
                }

                if (raw_dialogue)
                {
                    char *dialogue;
                    if (toml_rtos(raw_dialogue, &dialogue) == 0)
                    {
                        copy_string(gameData->events[event_index].dialogue, dialogue, sizeof(gameData->events[event_index].dialogue));
                        free(dialogue);
                    }
                }

                if (raw_action)
                {
                    char *action;
                    if (toml_rtos(raw_action, &action) == 0)
                    {
                        copy_string(gameData->events[event_index].action, action, sizeof(gameData->events[event_index].action));
                        free(action);
                    }
                }

                if (raw_result)
                {
                    char *result;
                    if (toml_rtos(raw_result, &result) == 0)
                    {
                        copy_string(gameData->events[event_index].result, result, sizeof(gameData->events[event_index].result));
                        free(result);
                    }
                }

                if (raw_item)
                {
                    char *item;
                    if (toml_rtos(raw_item, &item) == 0)
                    {
                        copy_string(gameData->events[event_index].item, item, sizeof(gameData->events[event_index].item));
                        free(item);
                    }
                }

                if (raw_character)
                {
                    char *character;
                    if (toml_rtos(raw_character, &character) == 0)
                    {
                        copy_string(gameData->events[event_index].character, character, sizeof(gameData->events[event_index].character));
                        free(character);
                    }
                }

                if (raw_amount)
                {
                    int64_t amount;
                    if (toml_rtoi(raw_amount, &amount) == 0)
                    {
                        gameData->events[event_index].amount = (int)amount;
                    }
                }

                if (strcmp(gameData->events[event_index].action, "increase_heart") == 0 ||
                    strcmp(gameData->events[event_index].action, "decrease_heart") == 0)
                {
                    printf("解析好感度事件: %s, 動作: %s, 角色: %s, 數量: %d\n",
                           gameData->events[event_index].name,
                           gameData->events[event_index].action,
                           gameData->events[event_index].character,
                           gameData->events[event_index].amount);
                }

                printf("解析事件: %s, 動作: %s, 物品: %s, 角色: %s, 數量: %d\n", gameData->events[event_index].name, gameData->events[event_index].action, gameData->events[event_index].item, gameData->events[event_index].character, gameData->events[event_index].amount);
            }
            event_index++;
        }
    }
}

void parse_script(const char *filename, GameData *gameData)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        perror("fopen");
        exit(1);
    }

    char errbuf[200];
    toml_table_t *root = toml_parse_file(fp, errbuf, sizeof(errbuf));
    fclose(fp);
    if (!root)
    {
        fprintf(stderr, "Error parsing TOML file: %s\n", errbuf);
        exit(1);
    }

    memset(gameData, 0, sizeof(GameData));
    printf("初始化 GameData 結構體完成\n");

    parse_initial_heart_points(root, gameData);
    parse_scenes(root, gameData);
    parse_characters(root, gameData);
    parse_items(root, gameData);
    parse_dialogues(root, gameData);
    parse_events(root, gameData);

    toml_free(root);
    printf("解析完成並釋放 root\n");

    // 檢查角色資料是否成功解析
    for (int i = 0; i < MAX_CHARACTERS; i++)
    {
        if (strlen(gameData->characters[i].name) > 0)
        {
            printf("角色 %d: %s, 頭像: %s, 立繪: %s, 位置: %s\n",
                   i,
                   gameData->characters[i].name,
                   gameData->characters[i].avatar,
                   gameData->characters[i].tachie,
                   gameData->characters[i].location);
        }
    }
}
