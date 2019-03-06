#include "CollisionSystem.hpp"
#include "Components/Cmp.hpp"
#include "common.hpp"
#include "UpdateAction.hpp"
#include "TileConstants.hpp"

// System to handle Sam colliding with other entities. Responsible for:
// - Updating when keys/torches/other items are picked up
// - Updating when enemies are collided with
// - Updating when Sam can interact with an object
//
// Has access to CollisionCmp and GameStateCmp to update.
// Has access to TransformCmp to know where everything is.
// Has access to ItemCmp to toggle items as held. 
void CollisionSystem::init(ObjectManager om, CollisionCmp cc, TransformCmp tc, ItemCmp itc, GameStateCmp* gsc)
{
	objectManager = om;
	collisionComponent = cc;
	transformComponent = tc;
	itemComponent = itc;
	gameStateComponent = gsc;
}

// Checks for collisions between Sam and other entities
// Returns an UpdateAction back to World
int CollisionSystem::update(float elapsed_ms)
{
	Transform *samTransform;
	Transform *entityTransform;
	Collision *samCollision;

	// Get Sam and only check his collisions
	Entity *sam = objectManager.getEntity(SAMS_GUID);
	samTransform = transformComponent.getTransform(sam);
	samCollision = collisionComponent.getmap()[SAMS_GUID];

	bool collisionEvent = false;

	for (auto& it2 : collisionComponent.getmap())
	{
		int entityId = it2.first;
		if (entityId != SAMS_GUID)
		{
			entityTransform = transformComponent.getTransform(objectManager.getEntity(entityId));

			// Check for Sam collisions with other entities
			if (AABB(samTransform, entityTransform))
			{
				collisionEvent = true;
				Entity* entity = objectManager.getEntity(entityId);

				// Handle door collisions
				int doorUpdateAction = handleDoors(entity);
				if (doorUpdateAction != NO_CHANGE)
				{
					return doorUpdateAction;
				}

				// Handle enemy collisions
				int enemyUpdateAction = handleEnemies(entity);
				if (enemyUpdateAction != NO_CHANGE)
				{
					return enemyUpdateAction;
				}

				// Handle key collisions
        int keyUpdateAction = handleKeys(entity);


				if (handleClosets(entity))
				{
					samCollision->closet = true;
				}

				// Handle grabbing a torch
				handleTorches(entity);
			}
		}
	}

	if (collisionEvent == false)
	{
		samCollision->closet = false;
	}

	return NO_CHANGE;
}

// Returns an UpdateAction to change rooms if a door is collided with
int CollisionSystem::handleDoors(Entity* entity)
{
	if (entity->label.compare("DoorRoom1To2") == 0)
	{
		return CHANGE_ROOM_ONE_TO_TWO;
	}
	else if (entity->label.compare("DoorRoom2To1") == 0)
	{
		return CHANGE_ROOM_TWO_TO_ONE;
	}
	else if (entity->label.compare("DoorRoom2To3") == 0)
	{
		return CHANGE_ROOM_TWO_TO_THREE;
	}
	else if (entity->label.compare("DoorRoom3To2") == 0)
	{
		return CHANGE_ROOM_THREE_TO_TWO;
	}
	else if (entity->label.compare("DoorRoom1To4") == 0)
	{
		return CHANGE_ROOM_ONE_TO_FOUR;
	}

	return NO_CHANGE;
}

// Returns an UpdateAction to trigger death if an enemy is collided with
int CollisionSystem::handleEnemies(Entity* entity)
{
	if (entity->label.compare("Enemy") == 0)
	{
		gameStateComponent->sam_is_alive = false;
	}

	return NO_CHANGE;
}

// Updates key count or returns NO_CHANGE if not a key
int CollisionSystem::handleKeys(Entity* entity)
{
	if (entity->label.compare("Key") == 0 && entity->active)
	{
		entity->active = false;
		if (gameStateComponent->current_room == ROOM_TWO_GUID)
		{
			gameStateComponent->level_two_key = true;
		}
		else if (gameStateComponent->current_room == ROOM_THREE_GUID)
		{
			gameStateComponent->level_three_key = true;
		}
	}

	return NO_CHANGE;
}

bool CollisionSystem::handleClosets(Entity* entity)
{
	if (entity->label.compare("ClosetArea") == 0)
	{
		return true;
	}

	return false;
}

// TODO?: Maybe an inventory or more general way of holding items
void CollisionSystem::handleTorches(Entity* entity)
{
	if (entity->label.compare("Torch") == 0)
	{
		// Stop drawing the picked up item
		entity->active = false;
		// Set Sam's held item to this entity
		gameStateComponent->held_item = TORCH;
		gameStateComponent->held_entity = entity;
		itemComponent.pickUpItem(entity);
	}
}

bool CollisionSystem::AABB(Transform *tr1, Transform *tr2) {

	float half_width_obj1 = (tr1->m_scale.x) * (tr1->width / 2);
	float half_height_obj1 = (tr1->m_scale.y) * (tr1->height / 2);

	float half_width_obj2 = (tr2->m_scale.x) * (tr2->width / 2);
	float half_height_obj2 = (tr2->m_scale.y) * (tr2->height / 2);

	// Grab object's edges
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
