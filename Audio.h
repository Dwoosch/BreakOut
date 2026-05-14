#pragma once
#include "miniaudio.h"

class Audio
{
public:
    static void Init();
    static void Shutdown();
    static void Play(const char* path);
    static void PlayLooping(ma_sound& sound, const char* path);
    static void StopSound(ma_sound& sound);
    static void SetVolume(float volume);
private:
    static ma_engine engine;
};