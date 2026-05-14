#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include "Audio.h"

ma_engine Audio::engine;

void Audio::Init() 
{ 
	ma_engine_init(NULL, &engine); 
}

void Audio::Shutdown() 
{ 
	ma_engine_uninit(&engine); 
}

void Audio::Play(const char* path) 
{
	ma_engine_play_sound(&engine, path, NULL); 
}

void Audio::PlayLooping(ma_sound& sound, const char* path)
{
    ma_sound_init_from_file(&engine, path, 0, NULL, NULL, &sound);
    ma_sound_set_looping(&sound, MA_TRUE);
    ma_sound_start(&sound);
}

void Audio::StopSound(ma_sound& sound)
{
    ma_sound_stop(&sound);
    ma_sound_uninit(&sound);
}

void Audio::SetVolume(float volume) 
{
	ma_engine_set_volume(&engine, volume);
}