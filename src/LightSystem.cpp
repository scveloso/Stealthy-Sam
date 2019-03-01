#include "LightSystem.hpp"
#include "Components/Cmp.hpp"
#include <string>
#include <iostream>
#include <Components/GameStateCmp.hpp>

// System to update entities that show light in the game.
//
// Has access to GameStateCmp to know about whether Sam
// is holding a torch, etc.
//
// Has access to Water to allow enable/disable the shaders
// to show polygons of light
void LightSystem::init(ObjectManager om, GameStateCmp* gameStateCmp, TransformCmp tc, Water* water)
{
  objectManager = om;
  gameState = gameStateCmp;
  transformCmp = tc;
  this->water = water;
}

void LightSystem::update()
{
  // If torch exists and is active, draw circle of light around it
  Entity* torchEntity = objectManager.getEntityByLabel("Torch");
  if (torchEntity && torchEntity->active) {
    Transform* torchTransform = transformCmp.getTransform(torchEntity);
    water->add_position(torchTransform->m_position);
  }

  // If Sam holding a candle, draw circle of light around Sam
  Entity* heldEntity = gameState->held_entity;
  if (heldEntity && gameState->held_entity->label.compare("Torch") == 0) {
    vec2 s_position = gameState->sam_position;
    water->add_position(s_position);
  }
}
