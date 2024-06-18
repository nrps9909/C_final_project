Interactive Fiction Engine (IFE)
簡介
這是一個用於創建和遊玩互動式小說的引擎，允許遊戲創作者以人類可讀的劇本格式編寫故事，並能顯示場景背景、角色立繪和對話選項。

環境建設
需求
Linux 作業系統
安裝以下依賴：
bash
Copy code
sudo apt-get install libcurl4-openssl-dev
sudo apt-get install libsdl2-dev
sudo apt-get install libsdl2-image-dev
sudo apt-get install libsdl2-ttf-dev
編譯程式
克隆專案並進入專案目錄：
bash
Copy code
git clone https://github.com/your-repo/interactive_fiction_engine.git
cd interactive_fiction_engine
編譯程式：
bash
Copy code
make
使用方法
將遊戲劇本放置在 example-game 目錄下。劇本文件命名為 script.toml。
運行遊戲：
bash
Copy code
cd example-game
../bin/interactive_fiction_engine script.toml
劇本格式
劇本使用 TOML 格式編寫，包含場景、角色、物品和事件等元素。範例劇本請參見 example-game/script.toml。

劇本說明
本節將詳細介紹如何撰寫和理解我們的 TOML 劇本。劇本主要分為以下幾個部分：初始設定、場景設定、角色設定、物品設定、事件設定和對話設定。

劇本結構
初始設定
在這部分，我們設定了遊戲開始時角色的初始愛心點數。

toml
Copy code
# 初始愛心點數
[initial_heart_points]
anna = 60
patty = 60
eunice = 60
lucy = 60
jamie = 60
場景設定
每個場景都有一個名稱和背景圖像路徑。場景用於定義故事發生的地點。

toml
Copy code
# 場景設定
[scene.white]
name = "White"
background = "example-game/assets/scene/white.png"

[scene.school-bus]
name = "校車"
background = "example-game/assets/scene/school-bus.bmp"

[scene.classroom]
name = "教室"
background = "example-game/assets/scene/classroom.bmp"
角色設定
每個角色都有一個名稱、頭像和立繪圖像路徑。角色設定用於定義故事中的人物。

toml
Copy code
# 角色設定
[character.anna]
name = "Anna"
avatar = "example-game/assets/character/anna/avatar.bmp"
tachie = "example-game/assets/character/anna/tachie.png"
物品設定
物品設定包括物品的名稱、描述、圖標路徑和物品所在位置。

toml
Copy code
# 物品設定
[item.christmas-card]
name = "聖誕卡片"
description = "Patty的聖誕卡片"
icon = "example-game/assets/item/christmas-card.webp"
location = "cafeteria"
事件設定
事件設定定義了場景轉換、對話進行以及物品添加或移除的操作。

toml
Copy code
# 事件設定
[event.start]
scene = "school-bus"
dialogue = "bus_introduction"

[event.change_scene_to_classroom]
scene = "classroom"
dialogue = "classroom_introduction"
對話設定
對話設定包括了場景、角色、對話文本和對話選項。

toml
Copy code
# 對話設定
[dialogue.bus_introduction]
scene = "school-gate"
character = "Lucy"
text1 = "\"掰噗~我們下次訓練見了\" 你校隊的隊友、也是同班三年的好朋友Lucy對你說"
text2 = "你對她揮了揮手，準備要回家"
[[dialogue.bus_introduction.options]]
text = "走，去搭校車了囉~"
next = "bus_introduction2"
event = ""

[dialogue.bus_introduction2]
scene = "school-bus"
character = "Anna"
text1 = "到了校車上，你發現你座位旁邊多了一個新的學妹"
text2 = "嗨！我是Anna，我剛轉來這裡，很高興認識你！"
[[dialogue.bus_introduction2.options]]
text = "很高興認識你，Anna！"
next = "classroom_introduction"
event = "change_scene_to_classroom"
詳細解說
初始設定
[initial_heart_points] 部分用於設定遊戲開始時每個角色的愛心點數。

場景設定
每個場景使用 [scene.<scene_id>] 來定義，其中 <scene_id> 是場景的唯一標識符。例如，[scene.school-bus] 定義了校車場景，name 是場景名稱，background 是場景背景圖像的路徑。

角色設定
每個角色使用 [character.<character_id>] 來定義，其中 <character_id> 是角色的唯一標識符。例如，[character.anna] 定義了角色 Anna，name 是角色名稱，avatar 是角色頭像圖像的路徑，tachie 是角色立繪圖像的路徑。

物品設定
每個物品使用 [item.<item_id>] 來定義，其中 <item_id> 是物品的唯一標識符。例如，[item.christmas-card] 定義了聖誕卡片，name 是物品名稱，description 是物品描述，icon 是物品圖標的路徑，location 是物品所在的位置。

事件設定
每個事件使用 [event.<event_id>] 來定義，其中 <event_id> 是事件的唯一標識符。事件可以用來改變場景、進行對話以及管理物品。例如，[event.start] 定義了遊戲開始時的事件，scene 是事件發生的場景，dialogue 是事件觸發的對話。

對話設定
每個對話使用 [dialogue.<dialogue_id>] 來定義，其中 <dialogue_id> 是對話的唯一標識符。例如，[dialogue.bus_introduction] 定義了校車上的對話，scene 是對話發生的場景，character 是進行對話的角色，text1, text2 等是對話文本。[[dialogue.bus_introduction.options]] 定義了對話選項，text 是選項文本，next 是下一個對話的標識符，event 是選項觸發的事件。


## 注意事項
- 劇本檔格式需嚴格遵守 TOML 語法，避免出現格式錯誤。
- 在 `README` 文件中詳細說明劇本檔格式、命名方式、存放地點等，以便遊戲創作者使用。

## 參考資料
- [tomlc99](https://github.com/cktan/tomlc99)
- [ncurses](https://invisible-island.net/ncurses/)
- [SDL](https://www.libsdl.org/)

## 引擎功能
- 支援場景背景、角色立繪、物品圖標的顯示。
- 支援對話選項和多支線劇情設定。
- 支援角色的情感變化和物品管理。
- 支援多個結局。