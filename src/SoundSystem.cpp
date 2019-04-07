#include "SoundSystem.hpp"

// Class to create and store entities for a given room
SoundSystem::SoundSystem()
{
    // Music
    background_music = Mix_LoadMUS(audio_path("spooky_background.wav"));
    boss_background_music = Mix_LoadMUS(audio_path("boss_background.wav"));

    // Sam
    death_sound = Mix_LoadWAV(audio_path("death_sound.wav"));
    step_sound = Mix_LoadWAV(audio_path("step.wav"));

    // Items
    key_pickup_sound = Mix_LoadWAV(audio_path("key_pickup.wav"));
    item_pickup_sound = Mix_LoadWAV(audio_path("item_pick_up.wav"));
    item_throw_sound = Mix_LoadWAV(audio_path("item_throw.wav"));

    // Closets
    open_closet_sound = Mix_LoadWAV(audio_path("open_closet.wav"));
    close_closet_sound = Mix_LoadWAV(audio_path("close_closet.wav"));

    // Fire
    cauldron_light_up_sound = Mix_LoadWAV(audio_path("cauldron_light_up.wav"));
    torch_dying_sound = Mix_LoadWAV(audio_path("lights_out.wav"));

    if (background_music == nullptr || death_sound == nullptr || boss_background_music == nullptr)
    {
        fprintf(stderr, "Failed to load sounds\n %s\n %s\n %s\n %s\nmake sure the data directory is present",
                audio_path("spooky_background.wav"),
                audio_path("boss_background.wav"),
                audio_path("death_sound.wav"),
                audio_path("key_pickup.wav"));
    }

    if (item_pickup_sound == nullptr || item_throw_sound == nullptr) {
      fprintf(stderr, "Failed to load sounds\n %s, %s\nmake sure the data directory is present",
              audio_path("item_pick_up.wav"), audio_path("item_throw.wav"));
    }

    if (open_closet_sound == nullptr || close_closet_sound == nullptr) {
      fprintf(stderr, "Failed to load sounds\n %s, %s\nmake sure the data directory is present",
              audio_path("open_closet.wav"), audio_path("close_closet.wav"));
    }

    if (cauldron_light_up_sound == nullptr || torch_dying_sound == nullptr) {
      fprintf(stderr, "Failed to load sounds\n %s, %s\nmake sure the data directory is present",
              audio_path("cauldron_light_up.wav"), audio_path("lights_out.wav"));
    }

    if (step_sound == nullptr) {
      fprintf(stderr, "Failed to load sounds\n %s\nmake sure the data directory is present",
              audio_path("step.wav"));
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

void SoundSystem::playItemPickup()
{
  Mix_PlayChannel(-1, item_pickup_sound, 0);
}

void SoundSystem::playItemThrow()
{
  Mix_PlayChannel(-1, item_throw_sound, 0);
}

void SoundSystem::playOpenCloset()
{
  Mix_PlayChannel(-1, open_closet_sound, 0);
}

void SoundSystem::playCloseCloset()
{
  Mix_PlayChannel(-1, close_closet_sound, 0);
}

void SoundSystem::playCauldronLightUp()
{
  Mix_PlayChannel(-1, cauldron_light_up_sound, 0);
}

void SoundSystem::playStep()
{
  Mix_PlayChannel(-1, step_sound, 0);
}

void SoundSystem::playTorchDying()
{
  Mix_PlayChannel(-1, torch_dying_sound, 0);
}
