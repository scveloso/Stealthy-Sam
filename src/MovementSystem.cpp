#include "MovementSystem.hpp"
#include "CollisionSystem.hpp"
#include "Components/Cmp.hpp"
#include <string>
#include <iostream>

MovementSystem::MovementSystem(ObjectManager om, InputCmp ic, TransformCmp tc, CollisionCmp cc)
{
    objectManager = om;
    inputComponent = ic;
    transformComponent = tc;
    collisionComponent = cc;
}

// Attempt moving the relevant entities to a new position based on their direction,
// If movement to a new position will be interrupted, cancel movement
void MovementSystem::update(float elapsed_ms)
{
	const float SAM_SPEED = 200.f;
	float step = SAM_SPEED * (elapsed_ms / 1000);

	for (auto& it : inputComponent.getmap())
	{
		Entity* entity = objectManager.getEntity(it.first);
    Transform* entityTransform = transformComponent.getTransform(entity);
		int direction = entityTransform->direction;
		vec2 oldPosition = entityTransform->m_position;

    if (transformComponent.getTransform(entity)->visible == true)
    {
      if (direction % LEFT == 0)
  		{
        entityTransform->m_position = { oldPosition.x - step, oldPosition.y };
        if (is_movement_interrupted(entity->id, entityTransform))
        {
          entityTransform->m_position = oldPosition;
        }
        else
        {
          oldPosition = entityTransform->m_position;
        }
  		}

      if (direction % RIGHT == 0)
  		{
        entityTransform->m_position = { oldPosition.x + step, oldPosition.y };
        if (is_movement_interrupted(entity->id, entityTransform))
        {
          entityTransform->m_position = oldPosition;
        }
        else
        {
          oldPosition = entityTransform->m_position;
        }
  		}

      if (direction % DOWN == 0)
  		{
        entityTransform->m_position = { oldPosition.x, oldPosition.y + step };
        if (is_movement_interrupted(entity->id, entityTransform))
        {
          entityTransform->m_position = oldPosition;
        }
        else
        {
          oldPosition = entityTransform->m_position;
        }
  		}

      if (direction % UP == 0)
  		{
        entityTransform->m_position = { oldPosition.x, oldPosition.y - step };
        if (is_movement_interrupted(entity->id, entityTransform))
        {
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

// Checks if movement to new position will be interrupted by a Wall entity
bool MovementSystem::is_movement_interrupted(int entityId, Transform* entityTransform)
{
	for (auto& it2 : collisionComponent.getmap())
	{
		int otherEntityId = it2.first;
		if (otherEntityId != entityId)
		{
      Entity* otherEntity = objectManager.getEntity(otherEntityId);

      if (otherEntity->label.compare("Wall") == 0)
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
