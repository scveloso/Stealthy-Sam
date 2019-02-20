#include "Components/CollisionCmp.hpp"
#include "Components/TransformCmp.hpp"
#include "Components/DrawCmp.hpp"
#include "ObjectManager.hpp"

class CollisionSystem
{
public:
	CollisionSystem(ObjectManager om, CollisionCmp cc, TransformCmp tc);
	void update(float elapsed_ms);
	void AABB(Entity e1, Transform *tr1, Entity e2, Transform *tr2);

private:
	CollisionCmp collisionComponent;
	TransformCmp transformComponent;
};