#include "Components/CollisionCmp.hpp"
#include "Components/TransformCmp.hpp"
#include "Components/DrawCmp.hpp"
#include "Components/EnemyCmp.hpp"
#include "DecisionNode.hpp"
#include "ObjectManager.hpp"
#include "EnemyAction.hpp"
#include <string>
#include <iostream>

class EnemySystem
{
public:
	void init(ObjectManager om, CollisionCmp cc, TransformCmp tc, EnemyCmp ec);
	void update(float elapsed_ms);
	void handleEnemyDecisionTree(Enemy* enemy, Transform* samTransform);
	void patrolEnemy(Enemy* enemy, Transform* et, float step);
	void chaseSam(Enemy* enemy, Transform* et, Transform* st, float step);
	void returnToPatrolPosition(Enemy* enemy, Transform* et, float step);

private:
	CollisionCmp collisionComponent;
	TransformCmp transformComponent;
	EnemyCmp enemyComponent;
	ObjectManager objectManager;
	std::vector<DecisionNode*> decision_tree;
};
