#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../third-party/tomlc99/toml.h"
#include "script_parser.h"

void parse_script(const char *filename, GameData *gameData) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("fopen");
        exit(1);
    }

    char errbuf[200];
    toml_table_t *root = toml_parse_file(fp, errbuf, sizeof(errbuf));
    fclose(fp);
    if (!root) {
        fprintf(stderr, "Error parsing TOML file: %s\n", errbuf);
        exit(1);
    }

    // 解析場景
    toml_table_t *scenes = toml_table_in(root, "scene");
    if (scenes) {
        int i = 0;
        for (i = 0; i < toml_table_nkval(scenes); i++) {
            toml_raw_t raw_name = toml_raw_in(scenes, "name");
            toml_raw_t raw_background = toml_raw_in(scenes, "background");
            if (raw_name && raw_background) {
                char* name;
                char* background;
                toml_rtos(raw_name, &name);
                toml_rtos(raw_background, &background);
                strncpy(gameData->scenes[i].name, name, sizeof(gameData->scenes[i].name));
                strncpy(gameData->scenes[i].background, background, sizeof(gameData->scenes[i].background));
                free(name);
                free(background);
            }
        }
    }

    // 解析角色
    toml_table_t *characters = toml_table_in(root, "character");
    if (characters) {
        int i = 0;
        for (i = 0; i < toml_table_nkval(characters); i++) {
            toml_raw_t raw_name = toml_raw_in(characters, "name");
            toml_raw_t raw_avatar = toml_raw_in(characters, "avatar");
            toml_raw_t raw_tachie = toml_raw_in(characters, "tachie");
            if (raw_name && raw_avatar && raw_tachie) {
                char* name;
                char* avatar;
                char* tachie;
                toml_rtos(raw_name, &name);
                toml_rtos(raw_avatar, &avatar);
                toml_rtos(raw_tachie, &tachie);
                strncpy(gameData->characters[i].name, name, sizeof(gameData->characters[i].name));
                strncpy(gameData->characters[i].avatar, avatar, sizeof(gameData->characters[i].avatar));
                strncpy(gameData->characters[i].tachie, tachie, sizeof(gameData->characters[i].tachie));
                free(name);
                free(avatar);
                free(tachie);
            }
        }
    }

    // 解析物品
    toml_table_t *items = toml_table_in(root, "item");
    if (items) {
        int i = 0;
        for (i = 0; i < toml_table_nkval(items); i++) {
            toml_raw_t raw_name = toml_raw_in(items, "name");
            toml_raw_t raw_description = toml_raw_in(items, "description");
            toml_raw_t raw_icon = toml_raw_in(items, "icon");
            if (raw_name && raw_description && raw_icon) {
                char* name;
                char* description;
                char* icon;
                toml_rtos(raw_name, &name);
                toml_rtos(raw_description, &description);
                toml_rtos(raw_icon, &icon);
                strncpy(gameData->items[i].name, name, sizeof(gameData->items[i].name));
                strncpy(gameData->items[i].description, description, sizeof(gameData->items[i].description));
                strncpy(gameData->items[i].icon, icon, sizeof(gameData->items[i].icon));
                free(name);
                free(description);
                free(icon);
            }
        }
    }

    // 解析對話
    toml_table_t *dialogues = toml_table_in(root, "dialogue");
    if (dialogues) {
        int i = 0;
        for (i = 0; i < toml_table_nkval(dialogues); i++) {
            toml_table_t *dialogue = toml_table_in(dialogues, "dialogue");
            toml_raw_t raw_character = toml_raw_in(dialogue, "character");
            toml_raw_t raw_text = toml_raw_in(dialogue, "text");
            toml_array_t *options = toml_array_in(dialogue, "options");

            if (raw_character && raw_text) {
                char* character;
                char* text;
                toml_rtos(raw_character, &character);
                toml_rtos(raw_text, &text);
                strncpy(gameData->dialogues[i].character, character, sizeof(gameData->dialogues[i].character));
                strncpy(gameData->dialogues[i].text, text, sizeof(gameData->dialogues[i].text));
                free(character);
                free(text);

                for (int j = 0; j < toml_array_nelem(options); j++) {
                    toml_table_t *option = toml_table_at(options, j);
                    toml_raw_t raw_option_text = toml_raw_in(option, "text");
                    toml_raw_t raw_next = toml_raw_in(option, "next");
                    if (raw_option_text && raw_next) {
                        char* option_text;
                        char* next;
                        toml_rtos(raw_option_text, &option_text);
                        toml_rtos(raw_next, &next);
                        strncpy(gameData->dialogues[i].options[j].text, option_text, sizeof(gameData->dialogues[i].options[j].text));
                        strncpy(gameData->dialogues[i].options[j].next, next, sizeof(gameData->dialogues[i].options[j].next));
                        free(option_text);
                        free(next);
                    }
                }
            }
        }
    }

    // 解析事件
    toml_table_t *events = toml_table_in(root, "event");
    if (events) {
        int i = 0;
        for (i = 0; i < toml_table_nkval(events); i++) {
            toml_raw_t raw_scene = toml_raw_in(events, "scene");
            toml_raw_t raw_dialogue = toml_raw_in(events, "dialogue");
            toml_raw_t raw_action = toml_raw_in(events, "action");
            toml_raw_t raw_result = toml_raw_in(events, "result");
            toml_raw_t raw_item = toml_raw_in(events, "item");

            if (raw_scene && raw_dialogue && raw_action && raw_result && raw_item) {
                char* scene;
                char* dialogue;
                char* action;
                char* result;
                char* item;
                toml_rtos(raw_scene, &scene);
                toml_rtos(raw_dialogue, &dialogue);
                toml_rtos(raw_action, &action);
                toml_rtos(raw_result, &result);
                toml_rtos(raw_item, &item);
                strncpy(gameData->events[i].scene, scene, sizeof(gameData->events[i].scene));
                strncpy(gameData->events[i].dialogue, dialogue, sizeof(gameData->events[i].dialogue));
                strncpy(gameData->events[i].action, action, sizeof(gameData->events[i].action));
                strncpy(gameData->events[i].result, result, sizeof(gameData->events[i].result));
                strncpy(gameData->events[i].item, item, sizeof(gameData->events[i].item));
                free(scene);
                free(dialogue);
                free(action);
                free(result);
                free(item);
            }
        }
    }

    toml_free(root);
}
