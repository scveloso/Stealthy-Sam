#include "CollisionSystem.hpp"
#include "Components/Component.hpp"
#include "common.hpp"

CollisionSystem::CollisionSystem(ObjectManager om, CollisionCmp cc, TransformCmp tc)
{
    objectManager = om;
	collisionComponent = cc;
	transformComponent = tc;
}

void CollisionSystem::update(float elapsed_ms)
{
	Transform *tr1;
	Transform *tr2;

	// Get Sam and only check his collisions
	Entity *sam = objectManager.getEntity(SAMS_GUID);

	// Get Sam's transform
	tr1 = transformComponent.getTransform(sam);

	for (auto& it2 : collisionComponent.getmap())
	{
		if (it2.first != SAMS_GUID) {

			tr2 = transformComponent.getTransform(objectManager.getEntity(it2.first));

			if (AABB(tr1, tr2))
			{
				//std::cout << sam->label << " colliding with " << objectManager.getEntity(it2.first)->label << std::endl;
				if (objectManager.getEntity(it2.first)->label == "Closet")
				{
					collisionComponent.getmap().at(SAMS_GUID)->closet = true;
				}
				else
				{
					collisionComponent.getmap().at(SAMS_GUID)->closet = false;
				}
			}
		}
	}
}

bool CollisionSystem::AABB(Transform *tr1, Transform *tr2) {

	float half_width_obj1 = (tr1->m_scale.x) * (tr1->width / 2);
	float half_height_obj1 = (tr1->m_scale.y) * (tr1->height / 2);

	float half_width_obj2 = (tr2->m_scale.x) * (tr2->width / 2);
	float half_height_obj2 = (tr2->m_scale.y) * (tr2->height / 2);

	// Grab object's edges
	// (Note: we don't want the edges of the texture but rather the edges of the actual sprite)
	float obj1_x1 = tr1->m_position.x - half_width_obj1;
	float obj1_x2 = tr1->m_position.x + half_width_obj1;
	float obj1_y1 = tr1->m_position.y - half_height_obj1;
	float obj1_y2 = tr1->m_position.y + half_height_obj1;

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
