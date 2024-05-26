#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

typedef struct {
    char name[100];
    int emotion;
    char inventory[10][50];
    int inventory_count;
    int current_scene;  // 添加 current_scene 成員
} Player;

typedef struct {
    char name[100];
    char background[100];
} Scene;

typedef struct {
    char name[100];
    char avatar[100];
    char tachie[100];
    char location[50];  // 將長度改為 50,與場景的 name 屬性一致
} Character;

typedef struct {
    char name[100];
    char description[200];
    char icon[100];
} Item;

typedef struct {
    char text[200];
    char next[50];
    char event[50];
} DialogueOption;

typedef struct {
    char name[100];    // 添加 name 成員
    char scene[50];    // 添加 scene 成員
    char character[50];
    char text[512];
    DialogueOption options[2];
} Dialogue;

typedef struct {
    char name[100];    // 添加 name 成員
    char scene[50];
    char dialogue[50];
    char action[50];
    char result[50];
    char item[50];
} Event;

typedef struct {
    Scene scenes[10];
    Character characters[10];
    Item items[10];
    Dialogue dialogues[20];
    Event events[10];
    int current_emotion;
} GameData;

void play_game(GameData *gameData);
int find_scene_index(GameData *gameData, const char *scene_name);

#endif // GAME_ENGINE_H