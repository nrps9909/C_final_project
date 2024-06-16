#ifndef SCRIPT_PARSER_H
#define SCRIPT_PARSER_H
#include "../../third-party/tomlc99/toml.h"
#include "game_engine.h"

void parse_script(const char *filename, GameData *gameData);
void parse_scenes(toml_table_t *root, GameData *gameData);
void parse_characters(toml_table_t *root, GameData *gameData);
void parse_items(toml_table_t *root, GameData *gameData);
void parse_dialogues(toml_table_t *root, GameData *gameData);
void parse_events(toml_table_t *root, GameData *gameData);
void copy_string(char *dest, const char *src, size_t dest_size);

#endif // SCRIPT_PARSER_H
