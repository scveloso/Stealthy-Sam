#include <SDL_mixer.h>
#include "common.hpp"

class SoundSystem {
public:
    SoundSystem();
    void haltMusic();
    void playBackgroundMusic();
    void playBossMusic();
    void playDeath();
    void playKeyPickup();
private:
    Mix_Music* background_music;
    Mix_Chunk* death_sound;
    Mix_Music* boss_background_music;
    Mix_Chunk* key_pickup_sound;
};
