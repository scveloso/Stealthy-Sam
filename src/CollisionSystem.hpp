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
	void update(float elapsed_ms);
	bool AABB(Transform *tr1, Transform *tr2);

private:
	CollisionCmp collisionComponent;
	TransformCmp transformComponent;
    ObjectManager objectManager;
};
