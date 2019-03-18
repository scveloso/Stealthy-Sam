#include "SoundSystem.hpp"

// Class to create and store entities for a given room
SoundSystem::SoundSystem()
{
    background_music = Mix_LoadMUS(audio_path("spooky_background.wav"));
    boss_background_music = Mix_LoadMUS(audio_path("boss_background.wav"));
    death_sound = Mix_LoadWAV(audio_path("death_sound.wav"));
    key_pickup_sound = Mix_LoadWAV(audio_path("key_pickup.wav"));
    if (background_music == nullptr || death_sound == nullptr || boss_background_music == nullptr)
    {
        fprintf(stderr, "Failed to load sounds\n %s\n %s\n %s\n %s\nmake sure the data directory is present",
                audio_path("spooky_background.wav"),
                audio_path("boss_background.wav"),
                audio_path("death_sound.wav"),
                audio_path("key_pickup.wav"));
    }
}

// Stops all music being played
void SoundSystem::haltMusic() {
    Mix_HaltMusic();
}

void SoundSystem::playBackgroundMusic()
{
    // Playing background music indefinitely
    Mix_PlayMusic(background_music, -1);
}

void SoundSystem::playKeyPickup()
{
    Mix_PlayChannel(-1, key_pickup_sound, 0);
}

void SoundSystem::playDeath()
{
    Mix_PlayChannel(-1, death_sound, 0);
}

void SoundSystem::playBossMusic()
{
    Mix_PlayMusic(boss_background_music, -1);
}

