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
// Has access to cone to allow enable/disable the shaders
// to show polygons of light.
//
// Has access to TransformCmp to know where everything is.
void LightSystem::init(ObjectManager* om, GameStateCmp* gameStateCmp, TransformCmp* tc, Light* light, EnemyCone* cone, EnemyCmp* ec)
{
  objectManager = om;
  gameState = gameStateCmp;
  transformCmp = tc;
  this->light = light;
  this->cone= cone;
  enemy= ec;
}

void LightSystem::update()
{
  // Grab all existing and active torches, draw circles of light around them
  std::vector<Entity*> torchEntities = objectManager->getEntitiesByLabel("Torch");
  int e = 0;
  light->clearTorchPositions();
  for (auto& torchEntity : torchEntities)
  {
    if (torchEntity->active) {
      Transform* torchTransform = transformCmp->getTransform(torchEntity);
      light->addTorchPosition(e, torchTransform->m_position);
      e = e + 2;
    }
  }

  // Grab all existing and active cauldrons, draw circles of light around them
  std::vector<Entity*> cauldronEntities = objectManager->getEntitiesByLabel("Cauldron");
  for (auto& cauldronEntity : cauldronEntities)
  {
    if (cauldronEntity->active) {
      Transform* cauldronTransform = transformCmp->getTransform(cauldronEntity);
      light->addTorchPosition(e, cauldronTransform->m_position);
      e = e + 2;
    }
  }

  // Grab all existing enemies and send their directions and positions to shader
  std::vector<Entity*> enemyEntities= objectManager->getEntitiesByLabel(ENEMY_LABEL);

  int i = 0;
  for (auto& enemyEntity : enemyEntities)
  {
    if (enemyEntity->active) {
      Transform* enemyTransform = transformCmp->getTransform(enemyEntity);
      cone->add_enemy_position(i, enemyTransform->m_position);
      int ecAction= enemy->getEnemyAction(enemyEntity->id);
      if(ecAction == 2 || ecAction == 4){
        cone->update_color(i, 1);
      }else{
        cone->update_color(i, 0);
      }
      // left =1; right =2; up =3; down=4;
      if (transformCmp->isFacingLeft(enemyEntity)){
        cone->add_enemy_direction(i, 1.0);
      }
      else if (transformCmp->isFacingRight(enemyEntity)){
        cone->add_enemy_direction(i, 2.0);
      }
      else if (transformCmp->isFacingUp(enemyEntity)){
        cone->add_enemy_direction(i, 3.0);
      }
      else if (transformCmp->isFacingDown(enemyEntity)){
        cone->add_enemy_direction(i, 4.0);
      }
      i = i + 2;
    }
  }

  // If Sam holding a torch and not hiding in closet, draw circle of light around Sam
  Entity* heldEntity = gameState->held_entity;
  if (heldEntity && gameState->held_entity->label.compare("Torch") == 0 && !gameState->hidden) {
    vec2 s_position = gameState->sam_position;
    light->add_position(s_position);
  } else {
    light->clearSamLight();
  }
}
