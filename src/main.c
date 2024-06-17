#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game_engine.h"
#include "ui_gui.h"
#include "sound_manager.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <script.toml>\n", argv[0]);
        return 1;
    }

    GameData gameData;
    memset(&gameData, 0, sizeof(GameData));
    parse_script(argv[1], &gameData);

    init_ui();
    init_sound();

    play_next_track(); // Start playing the first music track

    play_game(&gameData);

    cleanup_sound();
    cleanup_ui();

    return 0;
}
