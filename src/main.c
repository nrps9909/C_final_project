#include <stdio.h>
#include <stdlib.h>
#include "game_engine.h"
#include "script_parser.h"
#include "ui_gui.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <script.toml>\n", argv[0]);
        return 1;
    }

    GameData gameData;
    parse_script(argv[1], &gameData);

    init_ui();

    play_game(&gameData);

    cleanup_ui();

    return 0;
}
