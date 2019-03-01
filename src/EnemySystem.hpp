#include "Components/CollisionCmp.hpp"
#include "Components/TransformCmp.hpp"
#include "Components/DrawCmp.hpp"
#include "Components/EnemyCmp.hpp"
#include "Components/MovementCmp.hpp"
#include "DecisionNode.hpp"
#include "ObjectManager.hpp"
#include "EnemyAction.hpp"
#include <string>
#include <iostream>

class EnemySystem
{
public:
	void init(ObjectManager om, CollisionCmp cc, TransformCmp tc, EnemyCmp ec, MovementCmp mc);
	void update(float elapsed_ms);
	void handleEnemyDecisionTree(Enemy* enemy, Transform* samTransform);
	void patrolEnemy(Enemy* enemy, Entity* enemyEntity, Transform* et);
	void chaseSam(Enemy* enemy, Transform* et, Transform* st, Entity* enemyEntity);
	void returnToPatrolPosition(Enemy* enemy, Transform* et, Entity* enemyEntity, float elapsed_ms);

private:
	CollisionCmp collisionComponent;
	TransformCmp transformComponent;
	EnemyCmp enemyComponent;
	MovementCmp movementComponent;
	ObjectManager objectManager;
	std::vector<DecisionNode*> decision_tree;
};
