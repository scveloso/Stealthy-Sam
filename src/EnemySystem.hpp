#include "Components/CollisionCmp.hpp"
#include "Components/TransformCmp.hpp"
#include "Components/DrawCmp.hpp"
#include "Components/EnemyCmp.hpp"
#include "ObjectManager.hpp"
#include <string>
#include <iostream>

class EnemySystem
{
public:
	EnemySystem(ObjectManager om, CollisionCmp cc, TransformCmp tc, EnemyCmp ec);
	void update(float elapsed_ms);

private:
	CollisionCmp collisionComponent;
	TransformCmp transformComponent;
	EnemyCmp enemyComponent;
	ObjectManager objectManager;
};
