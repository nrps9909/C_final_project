# Interactive Fiction Engine (IFE)

## 簡介
這是一個用於創建和遊玩互動式小說的引擎，允許遊戲創作者以人類可讀的劇本格式編寫故事，並能顯示場景背景、角色立繪和對話選項。

## 劇本格式
劇本使用 TOML 格式編寫，包含場景、角色、物品和事件等元素。範例劇本請參見 `example-game/script.toml`。

## 環境建設
sudo apt-get install libcurl4-openssl-dev
sudo apt-get install libsdl2-dev
sudo apt-get install libsdl2-image-dev
cd .. 返回上一個資料夾
## 使用方法
make
cd example-game
../bin/interactive_fiction_engine script.toml

1. 將遊戲劇本放置在 `example-game` 目錄下。
2. 使用 `make` 編譯程式：
    ```bash
    make
    ```
3. 運行遊戲：
    ```bash
    ./engine ./example-game/
    ```

## 引擎功能
- 支援場景背景、角色立繪、物品圖標的顯示。
- 支援對話選項和多支線劇情設定。
- 支援角色的情感變化和物品管理。
- 支援多個結局。

## 已知問題
- 當前版本未實現完整的圖形介面，僅支援終端顯示。
- 圖片資源需手動放置於 `assets` 目錄中，且路徑需在劇本中正確指定。

## 注意事項
- 劇本檔格式需嚴格遵守 TOML 語法，避免出現格式錯誤。
- 在 `README` 文件中詳細說明劇本檔格式、命名方式、存放地點等，以便遊戲創作者使用。

## 參考資料
- [tomlc99](https://github.com/cktan/tomlc99)
- [ncurses](https://invisible-island.net/ncurses/)
- [SDL](https://www.libsdl.org/)
