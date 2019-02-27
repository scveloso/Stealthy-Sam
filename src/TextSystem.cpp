#include "TextSystem.hpp"
#include "Components/Cmp.hpp"
#include <string>
#include <iostream>
#include <Components/GameStateCmp.hpp>

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
    Entity* wasdEntity = objectManager.getEntityByLabel(USE_WASD_TEXT_LABEL);
    if (wasdEntity) {
      wasdEntity->active = false;
      water->showWASDText = 0;
    }

    Entity* eEntity = objectManager.getEntityByLabel(USE_E_INTERACT_LABEL);
    if (eEntity) {
      eEntity->active = true;
      water->showEText = 1;
    }
  }

  // If player has pressed E, disable the E tutorial
  if (gameState->has_pressed_E)
  {
    Entity* eEntity = objectManager.getEntityByLabel(USE_E_INTERACT_LABEL);
    if (eEntity) {
      eEntity->active = false;
      water->showEText = 0;
    }
  }

  // If player leaves room 1, stop shader from showing text
  if (gameState->current_room.compare(ROOM_ONE_GUID) != 0)
  {
    water->showEText = 0;
    water->showWASDText = 0;
  }
}
