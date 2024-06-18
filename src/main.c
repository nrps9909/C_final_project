#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "game_engine.h"
#include "script_parser.h"
#include "ui_gui.h"
#include "sound_manager.h"

// Function prototypes
void play_next_track();
void update_music();

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

    // Initialize SDL and other systems
    init_ui();
    init_sound();

    // Initialize player (add this if you have a Player structure)
    Player player;
    memset(&player, 0, sizeof(Player));
    // Populate player with initial data if necessary

    // Play the first track
    play_next_track();

    // Your game logic loop
    while (1)
    {
        // Handle events and update game state
        if (!play_game(&gameData)) // Ensure play_game returns an int
        {
            break;
        }

        // Update music
        update_music();

        // Display the scene with the player data
        display_scene(&gameData, "some_scene_name", &player);

        // Delay to control frame rate
        SDL_Delay(16); // Approximately 60 FPS
    }

    // Cleanup sound system
    cleanup_sound();

    // Cleanup SDL and other systems
    cleanup_ui();

    return 0;
}
