#include "TextSystem.hpp"
#include "Components/Cmp.hpp"
#include <string>
#include <iostream>
#include <Components/GameStateCmp.hpp>

// System to update text box entities in the game.
//
// Has access to GameStateCmp to know about whether Sam
// has done tutorial actions, changed rooms, or has died, etc.
//
// Has access to text to allow enable/disable the shaders
// from showing text box entities.
void TextSystem::init(ObjectManager* om, GameStateCmp* gameStateCmp, Text* text, Light* light, EnemyCone* enemy)
{
  objectManager = om;
  gameState = gameStateCmp;
  this->text = text;
  this->light = light;
  this->enemy= enemy;
}

void TextSystem::update()
{
  // If player has pressed H, enable or disable tutorial screen
  if (gameState->has_pressed_H)
  {
    enableTutorialScreen();
  } else
  {
    disableTutorialScreen();
  }
  // If Sam died, disable tutorial text, enable death text
  if (!gameState->sam_is_alive)
  {
    // disableEText();
    disableTutorialScreen();
    text->death = 1;
    light->death=1;
    enemy->death=1;
    objectManager->getEntityByLabel(USE_P_RESTART)->active = true;
  }
}

void TextSystem::enableTutorialScreen() {
  Entity* tutEntity = objectManager->getEntityByLabel(TUTORIAL_SCREEN_LABEL);
  if (tutEntity) {
    tutEntity->active = true;
    tutEntity->ui = true;
  }
}

void TextSystem::disableTutorialScreen() {
  Entity* tutEntity = objectManager->getEntityByLabel(TUTORIAL_SCREEN_LABEL);
  if (tutEntity) {
    tutEntity->active = false;
    tutEntity->ui = false;
  }
}
