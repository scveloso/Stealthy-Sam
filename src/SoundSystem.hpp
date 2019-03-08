#include <SDL_mixer.h>
#include "common.hpp"

class SoundSystem {
public:
    SoundSystem();
    void haltMusic();
    void playBackgroundMusic();
    void playDeath();
    void playBossMusic();
private:
    Mix_Music* background_music;
    Mix_Chunk* death_sound;
    Mix_Music* boss_background_music;
};
