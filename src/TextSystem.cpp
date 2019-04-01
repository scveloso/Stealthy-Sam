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
  // If player has moved, disable WASD tutorial and enable the E tutorial
  if (gameState->has_moved)
  {
    disableWASDText();
    enableEText();
  }

  // If player has pressed E, disable the E tutorial
  if (gameState->has_pressed_E)
  {
    disableEText();
  }

  // If player leaves room 1, stop shader from showing tutorial text
  if (gameState->current_room.compare(ROOM_ONE_GUID) != 0)
  {
    text->showEText = 0;
    text->showWASDText = 0;
  }

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
    disableEText();
    disableTutorialScreen();
    text->death = 1;
    light->death=1;
    enemy->death=1;
    objectManager->getEntityByLabel(USE_P_RESTART)->active = true;
  }
}

void TextSystem::disableWASDText() {
  Entity* wasdEntity = objectManager->getEntityByLabel(USE_WASD_TEXT_LABEL);
  if (wasdEntity) {
    wasdEntity->active = false;
    text->showWASDText = 0;
  }
}

void TextSystem::enableEText() {
  Entity* eEntity = objectManager->getEntityByLabel(USE_E_INTERACT_LABEL);
  if (eEntity) {
    eEntity->active = true;
    text->showEText = 1;
  }
}

void TextSystem::disableEText() {
  Entity* eEntity = objectManager->getEntityByLabel(USE_E_INTERACT_LABEL);
  if (eEntity) {
    eEntity->active = false;
    text->showEText = 0;
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
