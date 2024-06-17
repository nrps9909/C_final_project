#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include "../../third-party/openal/al.h"
#include "../../third-party/openal/alc.h"
#include <stdio.h>
#include "dr_wav.h"

void init_sound();
void cleanup_sound();
ALuint load_wav_file(const char *filename);
void play_sound(ALuint buffer);
void play_next_track();
void update_music();
void shuffle_tracks();
void start_music_thread();
void stop_music_thread();

#endif // SOUND_MANAGER_H
