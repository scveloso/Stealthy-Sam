#include "MovementSystem.hpp"
#include "Components/Component.hpp"

MovementSystem::MovementSystem(ObjectManager om, InputCmp ic, TransformCmp tc, CollisionCmp cc)
{
    objectManager = om;
    inputComponent = ic;
    transformComponent = tc;
    collisionComponent = cc;
}

void MovementSystem::update(float elapsed_ms)
{
	const float SAM_SPEED = 200.f;
	float step = SAM_SPEED * (elapsed_ms / 1000);

	for (auto& it : inputComponent.getmap())
	{
		Entity* entity = objectManager.getEntity(it.first);
		int direction = transformComponent.getTransform(entity)->direction;
		vec2 new_position = transformComponent.getTransform(entity)->m_position;

    if (transformComponent.getTransform(entity)->visible == true)
    {
      if (direction % LEFT == 0)
  		{
        new_position.x = new_position.x - step;
        if (is_movement_interrupted(new_position))
        {
          new_position.x = new_position.x + step;
        }
  		}

      if (direction % RIGHT == 0)
  		{
        new_position.x = new_position.x + step;
        if (is_movement_interrupted(new_position))
        {
          new_position.x = new_position.x - step;
        }
  		}

      if (direction % DOWN == 0)
  		{
        new_position.y = new_position.y + step;
        if (is_movement_interrupted(new_position))
        {
          new_position.y = new_position.y - step;
        }
  		}

      if (direction % UP == 0)
  		{
        new_position.y = new_position.y - step;
        if (is_movement_interrupted(new_position))
        {
          new_position.y = new_position.y + step;
        }
  		}

      transformComponent.setPosition(entity, new_position);
    }
	}
}

// Checks if movement to new position will be interrupted by a Wall entity
bool MovementSystem::is_movement_interrupted(vec2 new_position)
{
	Entity *sam = objectManager.getEntity(SAMS_GUID);
	Transform *samTransform = transformComponent.getTransform(sam);

	for (auto& it2 : collisionComponent.getmap())
	{
		int entityId = it2.first;
		if (entityId != SAMS_GUID)
		{
      Entity* entity = objectManager.getEntity(entityId);

      if (entity->label.compare("Wall") == 0)
      {
        Transform *entityTransform = transformComponent.getTransform(entity);

  			if (AABB(samTransform, entityTransform, new_position))
        {
          return true;
        }
      }
		}
	}

  return false;
}

bool MovementSystem::AABB(Transform *tr1, Transform *tr2, vec2 new_position) {

	float half_width_obj1 = (tr1->m_scale.x) * (tr1->width / 2);
	float half_height_obj1 = (tr1->m_scale.y) * (tr1->height / 2);

	float half_width_obj2 = (tr2->m_scale.x) * (tr2->width / 2);
	float half_height_obj2 = (tr2->m_scale.y) * (tr2->height / 2);

	// Grab object's edges
	float obj1_x1 = new_position.x - half_width_obj1;
	float obj1_x2 = new_position.x + half_width_obj1;
	float obj1_y1 = new_position.y - half_height_obj1;
	float obj1_y2 = new_position.y + half_height_obj1;

	float obj2_x1 = tr2->m_position.x - half_width_obj2;
	float obj2_x2 = tr2->m_position.x + half_width_obj2;
	float obj2_y1 = tr2->m_position.y - half_height_obj2;
	float obj2_y2 = tr2->m_position.y + half_height_obj2;

	// Collision case 1: top right corner will be inside the wall
	if (obj1_x2 >= obj2_x1 && obj1_x2 <= obj2_x2 &&
		obj1_y1 >= obj2_y1 && obj1_y1 <= obj2_y2)
	{
		return true;
	}

	// Collision case 2: top left corner will be inside the wall
	if (obj1_x1 >= obj2_x1 && obj1_x1 <= obj2_x2 &&
		obj1_y1 >= obj2_y1 && obj1_y1 <= obj2_y2)
	{
		return true;
	}

	// Collision case 3: bottom right corner will be inside the wall
	if (obj1_x2 >= obj2_x1 && obj1_x2 <= obj2_x2 &&
		obj1_y2 >= obj2_y1 && obj1_y2 <= obj2_y2)
	{
		return true;
	}

	// Collision case 4: bottom left corner will be inside the wall
	if (obj1_x1 >= obj2_x1 && obj1_x1 <= obj2_x2 &&
		obj1_y2 >= obj2_y1 && obj1_y2 <= obj2_y2)
	{
		return true;
	}

	// Collision case 5: prevent fat obj1 from going through thin wall from the bottom
	if (obj1_x1 <= obj2_x1 && obj1_x2 >= obj2_x2 &&
		obj1_y1 <= obj2_y2 && obj1_y2 >= obj2_y2)
	{
		return true;
	}

	// Collision case 6: prevent fat obj1 from going through thin wall from the top
	if (obj1_x1 <= obj2_x1 && obj1_x2 >= obj2_x2 &&
		obj1_y2 >= obj2_y1 && obj1_y1 <= obj2_y1)
	{
		return true;
	}

	// Collision case 7: prevent tall obj1 from going through short wall from the left
	if (obj1_y1 <= obj2_y1 && obj1_y2 >= obj2_y2 &&
		obj1_x2 >= obj2_x1 && obj1_x1 <= obj2_x1)
	{
		return true;
	}

	// Collision case 8: prevent tall obj1 from going through short wall from the right
	if (obj1_y1 <= obj2_y1 && obj1_y2 >= obj2_y2 &&
		obj1_x1 <= obj2_x2 && obj1_x2 >= obj2_x2)
	{
		return true;
	}

	return false;
}
