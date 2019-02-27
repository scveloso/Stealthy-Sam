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
// Has access to Water to allow enable/disable the shaders
// from showing text box entities.
void TextSystem::init(ObjectManager om, GameStateCmp* gameStateCmp, Water* water)
{
  objectManager = om;
  gameState = gameStateCmp;
  this->water = water;
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
    water->showEText = 0;
    water->showWASDText = 0;
  }

  // If Sam died, disable tutorial text, enable death text
  if (!gameState->sam_is_alive)
  {
    disableEText();
    water->death = 1;
    objectManager.getEntityByLabel(USE_R_RESTART)->active = true;
  }
}

void TextSystem::disableWASDText() {
  Entity* wasdEntity = objectManager.getEntityByLabel(USE_WASD_TEXT_LABEL);
  if (wasdEntity) {
    wasdEntity->active = false;
    water->showWASDText = 0;
  }
}

void TextSystem::enableEText() {
  Entity* eEntity = objectManager.getEntityByLabel(USE_E_INTERACT_LABEL);
  if (eEntity) {
    eEntity->active = true;
    water->showEText = 1;
  }
}

void TextSystem::disableEText() {
  Entity* eEntity = objectManager.getEntityByLabel(USE_E_INTERACT_LABEL);
  if (eEntity) {
    eEntity->active = false;
    water->showEText = 0;
  }
}
