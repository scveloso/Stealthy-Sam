#include <Components/GameStateCmp.hpp>
#include "Components/CollisionCmp.hpp"
#include "Components/TransformCmp.hpp"
#include "Components/ItemCmp.hpp"
#include "Components/DrawCmp.hpp"
#include "Components/EnemyCmp.hpp"
#include "ObjectManager.hpp"
#include "SoundManager.hpp"
#include <string>
#include <iostream>

class CollisionSystem
{
public:
	void init(ObjectManager* om, CollisionCmp* cc, TransformCmp* tc, ItemCmp itc, EnemyCmp ec, GameStateCmp* gsc);
	int update(float elapsed_ms);
	static bool AABB(Transform *tr1, Transform *tr2);
	bool AABBCone(Transform *tr1, vec2 cone_pos, float cone_width, float cone_height);
	int handleDoors(Entity* entity);
	int handleEnemies(Entity* entity);
	int handleKeys(Entity* entity);
	bool handleClosets(Entity* entity);
	int handleTorches(Entity* entity);
	void handleEnemyVisionCone(Transform* samTransform, Entity* entity);

private:
	EnemyCmp enemyComponent;
	CollisionCmp* collisionComponent;
	TransformCmp* transformComponent;
	ItemCmp itemComponent;
  	ObjectManager* objectManager;
  	GameStateCmp* gameStateComponent;

	void increaseKeyCount();
};
