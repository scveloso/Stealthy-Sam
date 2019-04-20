#include "CollisionSystem.hpp"
#include "Components/Cmp.hpp"
#include "common.hpp"
#include "UpdateAction.hpp"
#include "TileConstants.hpp"
#include "MissileSystem.hpp"
#include "Strategies/strategies_common.hpp"

// System to handle Sam colliding with other entities. Responsible for:
// - Updating when keys/torches/other items are picked up
// - Updating when enemies are collided with
// - Updating when Sam can interact with an object
//
// Has access to CollisionCmp and GameStateCmp to update.
// Has access to TransformCmp to know where everything is.
// Has access to ItemCmp to toggle items as held.
// Has access to EnemyCmp to set enemies to chase Sam on collide with vision cone.
void CollisionSystem::init(ObjectManager* om, CollisionCmp* cc, TransformCmp* tc, ItemCmp itc, EnemyCmp ec, GameStateCmp* gsc)
{
	objectManager = om;
	enemyComponent = ec;
	collisionComponent = cc;
	transformComponent = tc;
	itemComponent = itc;
	gameStateComponent = gsc;
}

// Checks for collisions between entities
// Returns an UpdateAction back to World
int CollisionSystem::update(float elapsed_ms)
{
	Transform *samTransform;
	Transform *entityTransform;
	Collision *samCollision;

	// Get Sam and only check his collisions
	Entity *sam = objectManager->getEntity(SAMS_GUID);
	samTransform = transformComponent->getTransform(sam);
	samCollision = collisionComponent->getmap()[SAMS_GUID];

	bool collisionEvent = false;

	for (auto& it2 : collisionComponent->getmap())
	{
		int entityId = it2.first;
		if (entityId != SAMS_GUID)
		{
			Entity* entity = objectManager->getEntity(entityId);
			entityTransform = transformComponent->getTransform(entity);
			if (!entity->active) continue;

			// Check if enemy vision cone collides with Sam
			handleEnemyVisionCone(samTransform, entity);

			// Check for Sam collisions with other entities
			if (AABB(samTransform, entityTransform))
			{
				collisionEvent = true;

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
				if (keyUpdateAction != NO_CHANGE)
				{
					return keyUpdateAction;
				}


				if (handleClosets(entity))
				{
					samCollision->closet = true;
				}

				// Handle grabbing a torch
				int torchUpdateAction = handleTorches(entity);
				if (torchUpdateAction != NO_CHANGE)
				{
						return torchUpdateAction;
				}
			}
		}
	}

	if (collisionEvent == false)
	{
		samCollision->closet = false;
	}

	// Check torch collisions with ghosts
	std::vector<Entity*> torchEntities = objectManager->getEntitiesByLabel("Torch");
	for (auto& torchEntity : torchEntities)
	{
		if (torchEntity->active == false) {
			continue;
		}

		std::vector<Entity*> ghostEntities = objectManager->getEntitiesByLabel("Enemy.Ghost");
		for (auto& ghostEntity : ghostEntities)
		{
			if (AABB(transformComponent->getTransform(torchEntity), transformComponent->getTransform(ghostEntity)))
			{
				Collision* torchCmp = collisionComponent->getCollision(torchEntity);
				if (torchCmp->torch_light_countdown_ms < 0.f)
				{
					torchCmp->torch_light_countdown_ms = 2500.f;
				}
			}
		}

		Collision* torchCmp = collisionComponent->getCollision(torchEntity);
		if (torchCmp->torch_light_countdown_ms > 0.f) {
            torchCmp->torch_light_countdown_ms -= elapsed_ms;
            if (torchCmp->torch_light_countdown_ms <= 0.f) {
                torchEntity->active = false;
                SoundManager::getInstance().playTorchDying();
            }
        }
	}

	return NO_CHANGE;
}

// Returns an UpdateAction to change rooms if a door is collided with
int CollisionSystem::handleDoors(Entity* entity)
{
	if (entity->label.compare("DoorRoom1To2") == 0)
	{
		return ROOM_TWO;
	}
	else if (entity->label.compare("DoorRoom2To1") == 0)
	{
		return CHANGE_TO_ROOM_ONE_ACTION;
	}
	else if (entity->label.compare("DoorRoom2To3") == 0)
	{
		return ROOM_THREE;
	}
	else if (entity->label.compare("DoorRoom3To2") == 0)
	{
		return ROOM_TWO;
	}
	else if (entity->label.compare("DoorRoom1To4") == 0)
	{
		return ROOM_FOUR;
	}

	return NO_CHANGE;
}

// Returns an UpdateAction to trigger death if an enemy is collided with
int CollisionSystem::handleEnemies(Entity* entity)
{
    if (entity->label == ENEMY_LABEL || entity->label == BOSS_GUID || entity->label.rfind(MISSILE_LABEL_PREFIX, 0) == 0)
	{
	    if (gameStateComponent->sam_is_alive) {
			gameStateComponent->sam_is_alive = false;
			return SAM_DEATH;
	    }
	}

	return NO_CHANGE;
}

void CollisionSystem::handleEnemyVisionCone(Transform* samTransform, Entity* entity) {
	if (entity->label == ENEMY_LABEL && gameStateComponent->hidden == false) {
		Transform* enemyTransform = transformComponent->getTransform(entity);

		bool collideWithCone = false;
		if (enemyTransform->facingDirection == UP) {
			collideWithCone = AABBCone(samTransform,
																{ enemyTransform->m_position.x, enemyTransform->m_position.y - TILE_HEIGHT },
																TILE_WIDTH,
																(1.5 * TILE_HEIGHT));
		} else if (enemyTransform->facingDirection == DOWN) {
			collideWithCone = AABBCone(samTransform,
																{ enemyTransform->m_position.x, enemyTransform->m_position.y + TILE_HEIGHT },
																TILE_WIDTH,
																(1.5 * TILE_HEIGHT));
		} else if (enemyTransform->facingDirection == LEFT) {
			collideWithCone = AABBCone(samTransform,
																{ enemyTransform->m_position.x - TILE_WIDTH, enemyTransform->m_position.y },
																(1.5 * TILE_WIDTH),
																TILE_HEIGHT);
		} else if (enemyTransform->facingDirection == RIGHT) {
			collideWithCone = AABBCone(samTransform,
																{ enemyTransform->m_position.x + TILE_WIDTH, enemyTransform->m_position.y },
																(1.5 * TILE_WIDTH),
																TILE_HEIGHT);
		}

		// Spotted Sam, chase him
		if (collideWithCone) {
			if (enemyComponent.getEnemyAction(entity->id) != CHASE_SAM) {
				enemyComponent.updateSpecificEnemyAction(entity->id, CHASE_SAM);
				SoundManager::getInstance().playGhostSpotSamSound();
			}
		}
	}
}

// Updates key count or returns NO_CHANGE if not a key
int CollisionSystem::handleKeys(Entity* entity)
{
	if (entity->label.compare("Key") == 0 && entity->active)
	{
		entity->active = false;
		if (gameStateComponent->current_room == ROOM_ONE_GUID)
		{
			gameStateComponent->level_one_key = true;
			increaseKeyCount();
			SoundManager::getInstance().playKeyPickup();
		}
        else if (gameStateComponent->current_room == ROOM_TWO_GUID)
        {
            gameStateComponent->level_two_key = true;
            increaseKeyCount();
            SoundManager::getInstance().playKeyPickup();
        }
		else if (gameStateComponent->current_room == ROOM_THREE_GUID)
		{
			gameStateComponent->level_three_key = true;
			increaseKeyCount();
			SoundManager::getInstance().playKeyPickup();
		}
	}

	return NO_CHANGE;
}


void CollisionSystem::increaseKeyCount() {
	Entity* key03 = objectManager->getEntityByLabel("key0_UI");
	Entity* key13 = objectManager->getEntityByLabel("key1_UI");
	Entity* key23 = objectManager->getEntityByLabel("key2_UI");
    Entity* key33 = objectManager->getEntityByLabel("key3_UI");

	if (key03->active) {
        key03->active = false;
        key03->ui = false;
		key13->active = true;
		key13->ui = true;
	} else if (key13->active) {
		key13->active = false;
		key13->ui = false;
		key23->active = true;
		key23->ui = true;
	} else if (key23->active) {
		key23->active = false;
		key23->ui = false;
		key33->active = true;
		key33->ui = true;
	}
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
int CollisionSystem::handleTorches(Entity* entity)
{
	// Can only pick up item if not holding anything and if item is active (torch is not dead)
	if (!gameStateComponent->held_entity) {
		if (entity->label.compare("Torch") == 0 && entity->active == true)
		{
			// Stop drawing the picked up item
			entity->active = false;
			// Set Sam's held item to this entity
			gameStateComponent->held_item = TORCH;
			gameStateComponent->held_entity = entity;
			itemComponent.pickUpItem(entity);

			SoundManager::getInstance().playItemPickup();
		}
	}

	return NO_CHANGE;
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

bool CollisionSystem::AABBCone(Transform *tr1, vec2 cone_pos, float cone_width, float cone_height) {
	float half_width_obj1 = (tr1->m_scale.x) * (tr1->width / 2);
	float half_height_obj1 = (tr1->m_scale.y) * (tr1->height / 2);

	// Grab object's edges
	float obj1_x1 = tr1->m_position.x - half_width_obj1;
	float obj1_x2 = tr1->m_position.x + half_width_obj1;
	float obj1_y1 = tr1->m_position.y - half_height_obj1;
	float obj1_y2 = tr1->m_position.y + half_height_obj1;

	float obj2_x1 = cone_pos.x - (cone_width / 2);
	float obj2_x2 = cone_pos.x + (cone_width / 2);
	float obj2_y1 = cone_pos.y - (cone_height / 2);
	float obj2_y2 = cone_pos.y + (cone_height / 2);

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
