#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

typedef struct {
    char name[100];
    int emotion; // 情感數值
    char inventory[10][50]; // 背包
    int inventory_count;
} Player;

typedef struct {
    char name[100];
    char background[100];
} Scene;

typedef struct {
    char name[100];
    char avatar[100];
    char tachie[100];
    char location[100];  // 新增 location
} Character;

typedef struct {
    char name[100];
    char description[200];
    char icon[100];
} Item;

typedef struct {
    char text[200];
    char next[50];
    char event[50];  // 新增 event
} DialogueOption;

typedef struct {
    char character[50];
    char text[200];
    DialogueOption options[2];
} Dialogue;

typedef struct {
    char scene[50];
    char dialogue[50];
    char action[50];
    char result[50];
    char item[50]; // 新增這個字段來處理 "add_item" 事件
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

#endif // GAME_ENGINE_H
