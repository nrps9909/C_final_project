#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

typedef struct {
    char name[50];
    int emotion;
    char inventory[10][50];
    int inventory_count;
    int current_scene;
    int current_dialogue;  // 添加 current_dialogue 成員
} Player;

typedef struct {
    char name[50];
    char background[100];
} Scene;

typedef struct {
    char name[50];
    char avatar[100];
    char tachie[100];
    char location[50];
} Character;

typedef struct {
    char name[50];
    char description[200];
    char icon[100];
} Item;

typedef struct {
    char text[200];
    char next[50];
    char event[50];
} DialogueOption;

typedef struct {
    char name[50];
    char scene[50];
    char character[50];
    char text[200];
    DialogueOption options[2];
} Dialogue;

typedef struct {
    char name[50];
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
} GameData;

void play_game(GameData* gameData);
void handle_event(GameData *gameData, Player *player, const char *event_name);
int find_scene_index(GameData *gameData, const char *scene_name);
int find_dialogue_index(GameData *gameData, const char *dialogue_name);
void add_item_to_inventory(Player *player, const char *item);
void update_emotion(Player *player, int change);
void display_emotion(Player* player);
void display_inventory(Player *player);

#endif // GAME_ENGINE_H
