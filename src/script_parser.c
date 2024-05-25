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

    // 初始化 GameData 結構體
    memset(gameData, 0, sizeof(GameData));

    // 解析場景
    toml_table_t *scenes = toml_table_in(root, "scene");
    if (scenes) {
        const char *scene_keys[] = {"library", "meeting-room", "cafeteria"};
        for (int i = 0; i < 3; i++) {
            toml_table_t *scene = toml_table_in(scenes, scene_keys[i]);
            if (scene) {
                toml_raw_t raw_background = toml_raw_in(scene, "background");
                if (raw_background) {
                    char *background;
                    toml_rtos(raw_background, &background);
                    strncpy(gameData->scenes[i].name, scene_keys[i], sizeof(gameData->scenes[i].name) - 1);
                    strncpy(gameData->scenes[i].background, background, sizeof(gameData->scenes[i].background) - 1);
                    free(background);
                }
            }
        }
    }

    // 解析角色
    toml_table_t *characters = toml_table_in(root, "character");
    if (characters) {
        const char *char_keys[] = {"classmate", "ta", "librarian"};
        for (int i = 0; i < 3; i++) {
            toml_table_t *character = toml_table_in(characters, char_keys[i]);
            if (character) {
                toml_raw_t raw_name = toml_raw_in(character, "name");
                toml_raw_t raw_avatar = toml_raw_in(character, "avatar");
                toml_raw_t raw_tachie = toml_raw_in(character, "tachie");
                toml_raw_t raw_location = toml_raw_in(character, "location");  // 解析位置
                if (raw_name && raw_avatar && raw_tachie) {
                    char *name;
                    char *avatar;
                    char *tachie;
                    char *location = NULL;
                    toml_rtos(raw_name, &name);
                    toml_rtos(raw_avatar, &avatar);
                    toml_rtos(raw_tachie, &tachie);
                    if (raw_location) {
                        toml_rtos(raw_location, &location);  // 解析位置
                    }
                    strncpy(gameData->characters[i].name, name, sizeof(gameData->characters[i].name) - 1);
                    strncpy(gameData->characters[i].avatar, avatar, sizeof(gameData->characters[i].avatar) - 1);
                    strncpy(gameData->characters[i].tachie, tachie, sizeof(gameData->characters[i].tachie) - 1);
                    if (location) {
                        strncpy(gameData->characters[i].location, location, sizeof(gameData->characters[i].location) - 1);
                        free(location);
                    }
                    free(name);
                    free(avatar);
                    free(tachie);
                }
            }
        }
    }

    // 解析物品
    toml_table_t *items = toml_table_in(root, "item");
    if (items) {
        const char *item_keys[] = {"vision-pro", "book"};
        for (int i = 0; i < 2; i++) {
            toml_table_t *item = toml_table_in(items, item_keys[i]);
            if (item) {
                toml_raw_t raw_name = toml_raw_in(item, "name");
                toml_raw_t raw_description = toml_raw_in(item, "description");
                toml_raw_t raw_icon = toml_raw_in(item, "icon");
                if (raw_name && raw_description && raw_icon) {
                    char *name;
                    char *description;
                    char *icon;
                    toml_rtos(raw_name, &name);
                    toml_rtos(raw_description, &description);
                    toml_rtos(raw_icon, &icon);
                    strncpy(gameData->items[i].name, name, sizeof(gameData->items[i].name) - 1);
                    strncpy(gameData->items[i].description, description, sizeof(gameData->items[i].description) - 1);
                    strncpy(gameData->items[i].icon, icon, sizeof(gameData->items[i].icon) - 1);
                    free(name);
                    free(description);
                    free(icon);
                }
            }
        }
    }

    // 解析對話
    toml_table_t *dialogues = toml_table_in(root, "dialogue");
    if (dialogues) {
        const char *dialogue_keys[] = {"hello", "teamup", "explanation", "ta_intro", "ta_discuss", "emotion_implementation", "multiple_endings", "finalize_ideas", "simplify_discussion", "time_management", "start_working", "work_division", "task_listing", "ending_good", "ending_bad", "ending_neutral"};
        for (int i = 0; i < 16; i++) {
            toml_table_t *dialogue = toml_table_in(dialogues, dialogue_keys[i]);
            if (dialogue) {
                strncpy(gameData->dialogues[i].name, dialogue_keys[i], sizeof(gameData->dialogues[i].name) - 1); // 添加這一行
                toml_raw_t raw_character = toml_raw_in(dialogue, "character");
                toml_raw_t raw_text = toml_raw_in(dialogue, "text");
                toml_array_t *options = toml_array_in(dialogue, "options");

                if (raw_character && raw_text) {
                    char *character;
                    char *text;
                    toml_rtos(raw_character, &character);
                    toml_rtos(raw_text, &text);
                    strncpy(gameData->dialogues[i].character, character, sizeof(gameData->dialogues[i].character) - 1);
                    strncpy(gameData->dialogues[i].text, text, sizeof(gameData->dialogues[i].text) - 1);
                    free(character);
                    free(text);

                    if (options) {
                        for (int j = 0; j < toml_array_nelem(options) && j < 2; j++) {
                            toml_table_t *option = toml_table_at(options, j);
                            toml_raw_t raw_option_text = toml_raw_in(option, "text");
                            toml_raw_t raw_next = toml_raw_in(option, "next");
                            toml_raw_t raw_event = toml_raw_in(option, "event");  // 解析事件
                            if (raw_option_text && raw_next) {
                                char *option_text;
                                char *next;
                                char *event = NULL;
                                toml_rtos(raw_option_text, &option_text);
                                toml_rtos(raw_next, &next);
                                if (raw_event) {
                                    toml_rtos(raw_event, &event);  // 解析事件
                                }
                                strncpy(gameData->dialogues[i].options[j].text, option_text, sizeof(gameData->dialogues[i].options[j].text) - 1);
                                strncpy(gameData->dialogues[i].options[j].next, next, sizeof(gameData->dialogues[i].options[j].next) - 1);
                                if (event) {
                                    strncpy(gameData->dialogues[i].options[j].event, event, sizeof(gameData->dialogues[i].options[j].event) - 1);  // 存儲事件
                                    free(event);
                                }
                                free(option_text);
                                free(next);
                            }
                        }
                    }
                }
            }
        }
    }

    // 解析事件
    toml_table_t *events = toml_table_in(root, "event");
    if (events) {
        const char *event_keys[] = {"start", "change_scene_to_meeting_room", "project_start", "game_over_good", "game_over_bad", "game_over_neutral"};
        for (int i = 0; i < 6; i++) {
            toml_table_t *event = toml_table_in(events, event_keys[i]);
            if (event) {
                toml_raw_t raw_scene = toml_raw_in(event, "scene");
                toml_raw_t raw_dialogue = toml_raw_in(event, "dialogue");
                toml_raw_t raw_action = toml_raw_in(event, "action");
                toml_raw_t raw_result = toml_raw_in(event, "result");
                toml_raw_t raw_item = toml_raw_in(event, "item");

                if (raw_scene && raw_dialogue && raw_action && raw_result) {
                    char *scene;
                    char *dialogue;
                    char *action;
                    char *result;
                    char *item = NULL;
                    toml_rtos(raw_scene, &scene);
                    toml_rtos(raw_dialogue, &dialogue);
                    toml_rtos(raw_action, &action);
                    toml_rtos(raw_result, &result);
                    if (raw_item) {
                        toml_rtos(raw_item, &item);
                    }
                    strncpy(gameData->events[i].scene, scene, sizeof(gameData->events[i].scene) - 1);
                    strncpy(gameData->events[i].dialogue, dialogue, sizeof(gameData->events[i].dialogue) - 1);
                    strncpy(gameData->events[i].action, action, sizeof(gameData->events[i].action) - 1);
                    strncpy(gameData->events[i].result, result, sizeof(gameData->events[i].result) - 1);
                    if (item) {
                        strncpy(gameData->events[i].item, item, sizeof(gameData->events[i].item) - 1);
                        free(item);
                    }
                    free(scene);
                    free(dialogue);
                    free(action);
                    free(result);
                }
            }
        }
    }

    toml_free(root);
}
