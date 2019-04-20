#include "MovementSystem.hpp"
#include "CollisionSystem.hpp"
#include "Components/Cmp.hpp"
#include <string>
#include <iostream>
#include <Components/GameStateCmp.hpp>
#include <ParticleSystem.hpp>

// System to handle movement for ALL relevant entities, including non-Sam entities.
//
// Has access to MovementCmp to know at what rate to move each and every entity.
// Has access to CollisionCmp to know if an entity will obstruct movement.
// Has access to TransformCmp to know where everything is.
void MovementSystem::init(ObjectManager* om, TransformCmp* tc, CollisionCmp* cc, MovementCmp* mc, GameStateCmp* gameStateCmp)
{
    objectManager = om;
    transformComponent = tc;
    collisionComponent = cc;
    movementComponent = mc;
    gameState = gameStateCmp;
}

// Attempt moving the relevant entities to a new position based on their movementDirection,
// If movement to a new position will be interrupted, cancel movement
void MovementSystem::update(float elapsed_ms)
{
	for (auto& it : movementComponent->getmap())
  {
    Entity* entity = objectManager->getEntity(it.first);
    if (!entity->active) continue;

    // Calculate step
    float step = movementComponent->getStep(entity, elapsed_ms);

    // For non-linear entities, check if it came to a halt
    // and rotate it sideways for comedic effect
    vec2 zeroVec = {0, 0};

    if (step < 0) {
      movementComponent->setCurrSpeed(entity, 0);
      movementComponent->resetMovementDirection(entity);
      movementComponent->setVecDirection(entity, zeroVec);
    }

    // If Sam is dead, continue
    if (entity->id == SAMS_GUID && !gameState->sam_is_alive)
    {
        continue;
    }

    Transform* entityTransform = transformComponent->getTransform(entity);

    if (entityTransform->body != NULL) {
        // Handled by bullet physics, ignore other stuff
        btTransform transform;
        entityTransform->body->getMotionState()->getWorldTransform(transform);
        btVector3 pos = transform.getOrigin();

        entityTransform->m_position = { pos.getX(), pos.getY() };
        continue;
    }
    int movementDirection = movementComponent->getMovementDirection(entity);
    vec2 vecDirection = movementComponent->getVecDirection(entity);
    vec2 oldPosition = entityTransform->m_position;

    if (transformComponent->getTransform(entity)->visible)
    {

		if (movementDirection % 99 == 0) {
			//do nothing
		}
      if (vecDirection.x != 0 || vecDirection.y != 0)
      {
          entityTransform->m_position = { oldPosition.x + (step * vecDirection.x), oldPosition.y + (step * vecDirection.y)};
          cauldronCheck(entity, entityTransform);

          if (is_movement_interrupted(entity, entityTransform))
          {
            movementComponent->setCurrSpeed(entity, 0);
            entityTransform->m_position = oldPosition;
          }
          else
          {
            oldPosition = entityTransform->m_position;
          }

          continue;
      }

      if (movementDirection % DOWN == 0)
      {
          entityTransform->m_position = { oldPosition.x, oldPosition.y + step };
          cauldronCheck(entity, entityTransform);
          if (is_movement_interrupted(entity, entityTransform))
          {
            movementComponent->setCurrSpeed(entity, 0);
            entityTransform->m_position = oldPosition;
          }
          else
          {
            oldPosition = entityTransform->m_position;
          }
      }

      if (movementDirection % UP == 0)
      {
          entityTransform->m_position = { oldPosition.x, oldPosition.y - step };
          cauldronCheck(entity, entityTransform);
          if (is_movement_interrupted(entity, entityTransform))
          {
            movementComponent->setCurrSpeed(entity, 0);
            entityTransform->m_position = oldPosition;
          }
          else
          {
            oldPosition = entityTransform->m_position;
          }
      }

      if (movementDirection % LEFT == 0)
        {
            entityTransform->m_position = { oldPosition.x - step, oldPosition.y };
            cauldronCheck(entity, entityTransform);

            if (is_movement_interrupted(entity, entityTransform))
            {
              movementComponent->setCurrSpeed(entity, 0);
              entityTransform->m_position = oldPosition;
            }
            else
            {
              oldPosition = entityTransform->m_position;
            }
        }

      if (movementDirection % RIGHT == 0)
      {
          entityTransform->m_position = { oldPosition.x + step, oldPosition.y };
          cauldronCheck(entity, entityTransform);
          if (is_movement_interrupted(entity, entityTransform))
          {
            movementComponent->setCurrSpeed(entity, 0);
            entityTransform->m_position = oldPosition;
          }
          else
          {
            oldPosition = entityTransform->m_position;
          }
      }
    }
      handleTimer(elapsed_ms);
    }
}

void MovementSystem::handleTimer(float elapsed_ms) const
{
    if (gameState->boss_door_text_countdown_ms > 0.f) {
        gameState->boss_door_text_countdown_ms -= elapsed_ms;
        if (gameState->boss_door_text_countdown_ms <= 0.f) {
            objectManager->getEntityByLabel(BOSS_DOOR_TEXT)->active = false;
        }
    }
    if (gameState->boss_text_countdown_ms > 0.f) {
        gameState->boss_text_countdown_ms -= elapsed_ms;
        if (gameState->boss_text_countdown_ms <= 0.f) {
            objectManager->getEntityByLabel(BOSS_TEXT)->active = false;
        }
    }
    if (gameState->intro_text_countdown_ms > 0.f) {
        gameState->intro_text_countdown_ms -= elapsed_ms;
        if (gameState->intro_text_countdown_ms <= 0.f) {
            objectManager->getEntityByLabel(INTRO_TEXT)->active = false;
        }
    }
}

void MovementSystem::cauldronCheck(Entity *entity, Transform *entityTransform) {
    if (entity->label.compare("Torch") == 0)
          {
              torch_cauldron_collision(entity->id, entityTransform);
          }
}

void MovementSystem::stopEntityMovement(Entity* entity)
{
  movementComponent->resetMovementDirection(entity);
  movementComponent->setCurrSpeed(entity, 0);
}

// Checks if movement to new position will be interrupted by a Wall entity
bool MovementSystem::is_movement_interrupted(Entity* entity, Transform* entityTransform)
{
  // Basic ghost enemies can go through walls
  if (entity->label == ENEMY_LABEL || entity->label.rfind(SMOKE_LABEL_PREFIX, 0) == 0) {
    return false;
  }

  for (auto& it2 : collisionComponent->getmap())
  {
      int otherEntityId = it2.first;
      if (otherEntityId != entity->id)
      {
          Entity* otherEntity = objectManager->getEntity(otherEntityId);

          if ((otherEntity->label.compare("Wall") == 0) || (otherEntity->label.compare("Closet") == 0)
                                                           || (otherEntity->label.compare("Boss_Door") == 0))
          {
              Transform *otherEntityTransform = transformComponent->getTransform(otherEntity);

              if (CollisionSystem::AABB(entityTransform, otherEntityTransform))
              {
                  handleBossDoor(entity, otherEntity);
                  return true;
              }
          }

      }
  }

  return false;
}

void MovementSystem::torch_cauldron_collision(int entityId, Transform* entityTransform)
{
    for (auto& it2 : collisionComponent->getmap())
    {
        int otherEntityId = it2.first;
        if (otherEntityId != entityId)
        {
            Entity* otherEntity = objectManager->getEntity(otherEntityId);

            if (otherEntity->label == "Cauldron")
            {
                Transform *otherEntityTransform = transformComponent->getTransform(otherEntity);

                if (CollisionSystem::AABB(entityTransform, otherEntityTransform))
                {
                    if (!otherEntity->active)
                    {
                        // if the torch is being lit for the first time, increment torch count
                        otherEntity->active = true;
                        if (gameState->current_room == ROOM_FOUR_GUID)
                        {
                            gameState->num_lit_cauldrons += 1;
                        }
                        SoundManager::getInstance().playCauldronLightUp();
                    }
                }
            }
        }
    }
}

void MovementSystem::handleBossDoor(Entity *pEntity, Entity *qEntity) {
    if (pEntity->label == SAM_GUID && qEntity->label == "Boss_Door")
    {
        if (gameState->boss_door_text_countdown_ms < 0.f)
        {
            gameState->boss_door_text_countdown_ms = 10000.f;
            objectManager->getEntityByLabel(BOSS_DOOR_TEXT)->active = true;
        }
    }
}
