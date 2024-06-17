#include "sound_manager.h"
#define DR_WAV_IMPLEMENTATION
#include "dr_wav.h"
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

static ALCdevice *device;
static ALCcontext *context;
static ALuint sources[1];
static ALuint buffers[5];
static int current_track = 0;
static int track_order[5];
static pthread_t music_thread;
static int keep_playing = 1;

void init_sound()
{
    device = alcOpenDevice(NULL); // Open default device
    if (!device)
    {
        fprintf(stderr, "Failed to open sound device\n");
        exit(1);
    }

    context = alcCreateContext(device, NULL);
    if (!context)
    {
        fprintf(stderr, "Failed to create sound context\n");
        alcCloseDevice(device);
        exit(1);
    }
    alcMakeContextCurrent(context);

    // Generate sound sources
    alGenSources(1, sources);

    // Load all music tracks
    buffers[0] = load_wav_file("example-game/assets/music/1.wav");
    buffers[1] = load_wav_file("example-game/assets/music/2.wav");
    buffers[2] = load_wav_file("example-game/assets/music/3.wav");
    buffers[3] = load_wav_file("example-game/assets/music/4.wav");
    buffers[4] = load_wav_file("example-game/assets/music/5.wav");

    // Initialize track order
    for (int i = 0; i < 5; i++)
    {
        track_order[i] = i;
    }

    shuffle_tracks();
}

void cleanup_sound()
{
    keep_playing = 0;
    pthread_join(music_thread, NULL);

    alDeleteSources(1, sources);
    alDeleteBuffers(5, buffers);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);
}

ALuint load_wav_file(const char *filename)
{
    drwav wav;
    drwav_uint64 totalPCMFrameCount;

    // Initialize the WAV file
    if (!drwav_init_file(&wav, filename, NULL))
    {
        fprintf(stderr, "Failed to load WAV file: %s\n", filename);
        return 0;
    }

    totalPCMFrameCount = wav.totalPCMFrameCount;

    // Allocate memory for the audio data
    drwav_int16 *pSampleData = malloc((size_t)totalPCMFrameCount * wav.channels * sizeof(drwav_int16));
    if (pSampleData == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for WAV data\n");
        drwav_uninit(&wav);
        return 0;
    }

    // Read the audio data
    drwav_read_pcm_frames_s16(&wav, totalPCMFrameCount, pSampleData);

    // Determine the OpenAL format
    ALenum format;
    if (wav.channels == 1)
    {
        format = wav.bitsPerSample == 16 ? AL_FORMAT_MONO16 : AL_FORMAT_MONO8;
    }
    else
    {
        format = wav.bitsPerSample == 16 ? AL_FORMAT_STEREO16 : AL_FORMAT_STEREO8;
    }

    // Create a buffer and copy the audio data to it
    ALuint buffer;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, format, pSampleData, (ALsizei)(totalPCMFrameCount * wav.channels * sizeof(drwav_int16)), wav.sampleRate);

    // Clean up
    free(pSampleData);
    drwav_uninit(&wav);

    return buffer;
}

void play_sound(ALuint buffer)
{
    alSourcei(sources[0], AL_BUFFER, buffer);
    alSourcePlay(sources[0]);
}

void play_next_track()
{
    play_sound(buffers[track_order[current_track]]);
    current_track = (current_track + 1) % 5;
}

void update_music()
{
    ALint state;
    alGetSourcei(sources[0], AL_SOURCE_STATE, &state);

    if (state != AL_PLAYING)
    {
        play_next_track();
    }
}

void shuffle_tracks()
{
    srand(time(NULL)); // Seed the random number generator
    for (int i = 0; i < 5; i++)
    {
        int j = rand() % 5;
        int temp = track_order[i];
        track_order[i] = track_order[j];
        track_order[j] = temp;
    }
}

void *music_thread_func(void *arg)
{
    while (keep_playing)
    {
        update_music();
        usleep(100000); // Sleep for 100 ms
    }
    return NULL;
}

void start_music_thread()
{
    keep_playing = 1;
    pthread_create(&music_thread, NULL, music_thread_func, NULL);
}

void stop_music_thread()
{
    keep_playing = 0;
    pthread_join(music_thread, NULL);
}