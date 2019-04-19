#pragma once

#include <SDL_mixer.h>
#include "common.hpp"

class SoundManager {

private:
  Mix_Music* background_music;
  Mix_Chunk* death_sound;
  Mix_Music* boss_background_music;
  Mix_Chunk* key_pickup_sound;

  Mix_Chunk* item_pickup_sound;
  Mix_Chunk* item_throw_sound;

  Mix_Chunk* open_closet_sound;
  Mix_Chunk* close_closet_sound;

  Mix_Chunk* cauldron_light_up_sound;

  Mix_Chunk* step_sound;
  Mix_Chunk* torch_dying_sound;

  Mix_Chunk* ghost_spot_sam_sound;

  Mix_Chunk* menu_sound;

  Mix_Chunk* game_end_sound;

public:
  static SoundManager& getInstance();

  void init();

  void haltMusic();

  void pauseMusic();
  void resumeMusic();

  void playBackgroundMusic();
  void playBossMusic();
  void playDeath();
  void playKeyPickup();

  void playItemPickup();
  void playItemThrow();

  void playOpenCloset();
  void playCloseCloset();

  void playCauldronLightUp();
  void playTorchDying();

  void playStep();

  void playMenuSound();
  void playGameEndSound();

  void playGhostSpotSamSound();
};
