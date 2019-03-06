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
// to show polygons of light.
//
// Has access to TransformCmp to know where everything is.
void LightSystem::init(ObjectManager om, GameStateCmp* gameStateCmp, TransformCmp tc, Water* water)
{
  objectManager = om;
  gameState = gameStateCmp;
  transformCmp = tc;
  this->water = water;
}

void LightSystem::update()
{
  // Grab all existing and active torches, draw circles of light around them
  Entity* torchEntity = objectManager.getEntityByLabel("Torch");
  std::vector<Entity*> torchEntities = objectManager.getEntitiesByLabel("Torch");

 // grab all existing enemy
 Entity* enemyEntity= objectManager.getEntityByLabel("Enemy");
 std::vector<Entity*> enemyEntities= objectManager.getEntitiesByLabel("Enemy");

  // TODO: Get water and shader to support multiple circle of light positions
  for (auto& torchEntity : torchEntities)
	{
    if (torchEntity->active) {
      Transform* torchTransform = transformCmp.getTransform(torchEntity);
      water->add_position(torchTransform->m_position);
    }
	}

  int i = 0;
  for (auto& enemyEntity : enemyEntities)
  {
    if (enemyEntity->active) {
      Transform* enemyTransform = transformCmp.getTransform(enemyEntity);
      water->add_enemy_position(i, enemyTransform->m_position);
      i= i+2;
    }

	}

  // If Sam holding a torch, draw circle of light around Sam
  Entity* heldEntity = gameState->held_entity;
  if (heldEntity && gameState->held_entity->label.compare("Torch") == 0) {
    vec2 s_position = gameState->sam_position;
    water->add_position(s_position);
  }
}
