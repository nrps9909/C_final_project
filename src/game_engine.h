#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H
#pragma GCC diagnostic ignored "-Wstringop-truncation"

// 定義常量
#define MAX_INVENTORY_SIZE 10
#define MAX_ITEM_NAME_LENGTH 50
#define MAX_CHARACTERS 6
#define MAX_DIALOGUE_OPTIONS 4
#define MAX_SCENES 50
#define MAX_ITEMS 10
#define MAX_DIALOGUES 100
#define MAX_EVENTS 50

// 定義玩家結構體
typedef struct
{
    char name[MAX_ITEM_NAME_LENGTH];                          // 玩家名稱
    int emotion;                                              // 玩家情緒值
    char inventory[MAX_INVENTORY_SIZE][MAX_ITEM_NAME_LENGTH]; // 玩家物品清單
    int inventory_count;                                      // 玩家物品數量
    int current_scene;                                        // 當前場景
    int current_dialogue;                                     // 當前對話
} Player;

// 定義場景結構體
typedef struct
{
    char name[50];        // 場景名稱
    char background[100]; // 場景背景
} Scene;

// 定義角色結構體
typedef struct
{
    char name[50];     // 角色名稱
    char avatar[100];  // 角色頭像
    char tachie[100];  // 角色立繪
    char location[50]; // 角色位置
    int heart;         // 角色好感度
} Character;

// 定義物品結構體
typedef struct
{
    char name[50];         // 物品名稱
    char description[200]; // 物品描述
    char icon[100];        // 物品圖標
    char scene[50];        // 物品所屬場景
} Item;

// 定義對話選項結構體
typedef struct
{
    char text[200];  // 對話選項文本
    char next[50];   // 下一個對話名稱
    char event[50];  // 觸發事件
    char action[50]; // 動作
} DialogueOption;

// 定義對話結構體
typedef struct
{
    char name[50];      // 對話名稱
    char scene[50];     // 對話所屬場景
    char character[50]; // 參與對話的角色
    char text1[256];
    char text2[256];
    char text3[256];
    char text4[256];                              // 新增這一行
    DialogueOption options[MAX_DIALOGUE_OPTIONS]; // 對話選項
} Dialogue;

// 定義事件結構體
typedef struct
{
    char name[50];      // 事件名稱
    char scene[50];     // 事件所屬場景
    char dialogue[100]; // 事件所屬對話
    char action[50];    // 事件動作
    char result[50];    // 事件結果
    char item[50];      // 事件物品
    char character[50]; // 事件角色
    int amount;         // 事件影響的數量
} Event;

// 定義遊戲資料結構體
typedef struct
{
    Scene scenes[MAX_SCENES];             // 所有場景
    Character characters[MAX_CHARACTERS]; // 所有角色
    Item items[MAX_ITEMS];                // 所有物品
    Dialogue dialogues[MAX_DIALOGUES];    // 所有對話
    Event events[MAX_EVENTS];             // 所有事件
} GameData;

// 函式原型
void play_game(GameData *gameData);                                                      // 遊戲主函式
void handle_event(GameData *gameData, Player *player, const char *event_name);           // 處理事件
int find_scene_index(GameData *gameData, const char *scene_name);                        // 找到場景索引
int find_dialogue_index(GameData *gameData, const char *dialogue_name);                  // 找到對話索引
int find_item_index(GameData *gameData, const char *item_name);                          // 找到物品索引
int find_character_index(GameData *gameData, const char *character_name);                // 找到角色索引
void add_item_to_inventory(Player *player, Item *item);                                  // 將物品加入玩家物品欄
void update_emotion(Player *player, int change);                                         // 更新玩家情緒
void display_emotion(Player *player);                                                    // 顯示玩家情緒
void display_inventory(Player *player);                                                  // 顯示玩家物品欄
void update_character_heart(GameData *gameData, const char *character_name, int amount); // 更新角色好感度
void display_character_hearts(GameData *gameData);                                       // 顯示角色好感度
void parse_script(const char *filename, GameData *gameData);                             // 解析腳本
int find_dialogue_index_by_name(GameData *gameData, const char *name);                   // 根據名稱查找對話索引

#endif // GAME_ENGINE_H
