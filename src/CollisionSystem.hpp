#include "Components/CollisionCmp.hpp"
#include "Components/TransformCmp.hpp"
#include "Components/DrawCmp.hpp"
#include "ObjectManager.hpp"
#include <string>
#include <iostream>

class CollisionSystem
{
public:
	CollisionSystem(ObjectManager om, CollisionCmp cc, TransformCmp tc);
	int update(float elapsed_ms);
	static bool AABB(Transform *tr1, Transform *tr2);
	int handleDoors(Entity* entity);
	int handleEnemies(Entity* entity);
	bool handleClosets(Entity* entity);

private:
	CollisionCmp collisionComponent;
	TransformCmp transformComponent;
  ObjectManager objectManager;
};
