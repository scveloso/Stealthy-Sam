#include "SoundManager.hpp"

SoundManager& SoundManager::getInstance() {
  static SoundManager theInstance;

  return theInstance;
}

void SoundManager::init()
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

    // Enemies
    ghost_spot_sam_sound = Mix_LoadWAV(audio_path("ghost_spot_sam_sound.wav"));

    // Menu
    menu_sound = Mix_LoadWAV(audio_path("play_menu.wav"));
    game_end_sound = Mix_LoadWAV(audio_path("game_end_sound.wav"));

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

    if (ghost_spot_sam_sound == nullptr) {
      fprintf(stderr, "Failed to load sounds\n %s \nmake sure the data directory is present",
              audio_path("ghost_spot_sam_sound.wav"));
    }

    if (step_sound == nullptr || menu_sound == nullptr || game_end_sound == nullptr) {
      fprintf(stderr, "Failed to load sounds\n %s, %s, %s\nmake sure the data directory is present",
              audio_path("play_menu.wav"), audio_path("step.wav"), audio_path("game_end_sound.wav"));
    }
}

// Stops all music being played
void SoundManager::haltMusic() {
    Mix_HaltMusic();
}

void SoundManager::pauseMusic() {
    Mix_PauseMusic();
}

void SoundManager::resumeMusic() {
    Mix_ResumeMusic();
}

void SoundManager::playBackgroundMusic()
{
    // Playing background music indefinitely
    Mix_PlayMusic(background_music, -1);
}

void SoundManager::playKeyPickup()
{
    Mix_PlayChannel(-1, key_pickup_sound, 0);
}

void SoundManager::playDeath()
{
    Mix_PlayChannel(-1, death_sound, 0);
}

void SoundManager::playBossMusic()
{
    Mix_PlayMusic(boss_background_music, -1);
}

void SoundManager::playItemPickup()
{
  Mix_PlayChannel(-1, item_pickup_sound, 0);
}

void SoundManager::playItemThrow()
{
  Mix_PlayChannel(-1, item_throw_sound, 0);
}

void SoundManager::playOpenCloset()
{
  Mix_PlayChannel(-1, open_closet_sound, 0);
}

void SoundManager::playCloseCloset()
{
  Mix_PlayChannel(-1, close_closet_sound, 0);
}

void SoundManager::playCauldronLightUp()
{
  Mix_PlayChannel(-1, cauldron_light_up_sound, 0);
}

void SoundManager::playStep()
{
  Mix_PlayChannel(-1, step_sound, 0);
}

void SoundManager::playTorchDying()
{
  Mix_PlayChannel(-1, torch_dying_sound, 0);
}

void SoundManager::playMenuSound()
{
  Mix_PlayChannel(-1, menu_sound, 0);
}

void SoundManager::playGameEndSound()
{
  Mix_PlayChannel(-1, game_end_sound, 0);
}

void SoundManager::playGhostSpotSamSound()
{
  Mix_PlayChannel(-1, ghost_spot_sam_sound, 0);
}
