#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <AL/al.h>
#include <AL/alc.h>
#include <stdio.h>
#include "dr_wav.h"

void init_sound();
void cleanup_sound();
ALuint load_wav_file(const char *filename);
void play_sound(ALuint buffer);
void play_next_track();
void update_music();

#endif // SOUND_MANAGER_H
