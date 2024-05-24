#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include "game_engine.h"
#include "script_parser.h"
#include "ui_gui.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <script.toml>\n", argv[0]);
        return 1;
    }

    GameData gameData;
    memset(&gameData, 0, sizeof(GameData));
    parse_script(argv[1], &gameData);

    char exec_path[PATH_MAX];
    if (!realpath(argv[0], exec_path)) {
        perror("realpath");
        return 1;
    }

    init_ui(exec_path);
    play_game(&gameData);
    cleanup_ui();

    return 0;
}
