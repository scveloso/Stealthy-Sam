#include "MovementSystem.hpp"
#include "CollisionSystem.hpp"
#include "Components/Cmp.hpp"
#include <string>
#include <iostream>
#include <Components/GameStateCmp.hpp>

// System to handle movement for ALL relevant entities, including non-Sam entities.
//
// Has access to MovementCmp to know at what rate to move each and every entity.
// Has access to CollisionCmp to know if an entity will obstruct movement.
// Has access to TransformCmp to know where everything is.
void MovementSystem::init(ObjectManager om, TransformCmp tc, CollisionCmp cc, MovementCmp mc, GameStateCmp* gameStateCmp)
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
	for (auto& it : movementComponent.getmap())
  {
    Entity* entity = objectManager.getEntity(it.first);

    // Calculate step
    float step = movementComponent.getStep(entity, elapsed_ms);

    // For non-linear entities, check if it came to a halt
    // and rotate it sideways for comedic effect
    if (step < 0) {
      movementComponent.setCurrSpeed(entity, 0);
      movementComponent.resetMovementDirection(entity);
    }

    // If Sam is dead, continue
    if (entity->id == SAMS_GUID && !gameState->sam_is_alive)
    {
        continue;
    }

    Transform* entityTransform = transformComponent.getTransform(entity);
    int movementDirection = movementComponent.getMovementDirection(entity);
    vec2 oldPosition = entityTransform->m_position;

    if (transformComponent.getTransform(entity)->visible)
    {
      if (movementDirection % LEFT == 0)
      {
          entityTransform->m_position = { oldPosition.x - step, oldPosition.y };
          if (is_movement_interrupted(entity->id, entityTransform))
          {
            movementComponent.setCurrSpeed(entity, 0);
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
          if (is_movement_interrupted(entity->id, entityTransform))
          {
            movementComponent.setCurrSpeed(entity, 0);
            entityTransform->m_position = oldPosition;
          }
          else
          {
            oldPosition = entityTransform->m_position;
          }
      }

      if (movementDirection % DOWN == 0)
      {
          entityTransform->m_position = { oldPosition.x, oldPosition.y + step };
          if (is_movement_interrupted(entity->id, entityTransform))
          {
            movementComponent.setCurrSpeed(entity, 0);
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
          if (is_movement_interrupted(entity->id, entityTransform))
          {
            movementComponent.setCurrSpeed(entity, 0);
            entityTransform->m_position = oldPosition;
          }
          else
          {
            oldPosition = entityTransform->m_position;
          }
      }
    }
  }
}

void MovementSystem::stopEntityMovement(Entity* entity)
{
  movementComponent.resetMovementDirection(entity);
  movementComponent.setCurrSpeed(entity, 0);
}

// Checks if movement to new position will be interrupted by a Wall entity
bool MovementSystem::is_movement_interrupted(int entityId, Transform* entityTransform)
{
    for (auto& it2 : collisionComponent.getmap())
    {
        int otherEntityId = it2.first;
        if (otherEntityId != entityId)
        {
            Entity* otherEntity = objectManager.getEntity(otherEntityId);

      if ((otherEntity->label.compare("Wall") == 0) || (otherEntity->label.compare("Closet") == 0))
      {
        Transform *otherEntityTransform = transformComponent.getTransform(otherEntity);

                if (CollisionSystem::AABB(entityTransform, otherEntityTransform))
                {
                    return true;
                }
            }
        }
    }

    return false;
}
