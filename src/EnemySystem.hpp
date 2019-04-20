#include "Components/CollisionCmp.hpp"
#include "Components/TransformCmp.hpp"
#include "Components/DrawCmp.hpp"
#include "Components/EnemyCmp.hpp"
#include "Components/ItemCmp.hpp"
#include "Components/MovementCmp.hpp"
#include "DecisionNode.hpp"
#include "ObjectManager.hpp"
#include "SoundManager.hpp"
#include "EnemyCone.hpp"
#include <Strategies/BossStrategy.hpp>
#include <string>
#include <iostream>

class EnemySystem
{
public:
	void init(ObjectManager* om, TransformCmp* tc, EnemyCmp ec, MovementCmp* mc, ItemCmp itc, GameStateCmp* gsc);
	void initDecisionTree();
	int update(float elapsed_ms);
	void handleEnemyDecisionTree(Enemy* enemy, Transform* samTransform);
	void handleBossDecisionTree(Enemy* enemy, Transform* enemyTransform);
	void patrolEnemy(Enemy* enemy, Entity* enemyEntity, Transform* et, float elapsed_ms);
	void tryChaseThrownTorch(Enemy* enemy, Transform* et, Entity* enemyEntity);
	void chaseTarget(Enemy* enemy, Transform* et, Transform* gt, Entity* enemyEntity);
	void returnToPatrolPosition(Enemy* enemy, Transform* et, Entity* enemyEntity, float elapsed_ms);
  void goToTarget(vec2 startPosition, vec2 targetPosition, Entity* entity);

private:
	TransformCmp* transformComponent;
	EnemyCmp enemyComponent;
	MovementCmp* movementComponent;
	ItemCmp itemComponent;
	ObjectManager* objectManager;
	GameStateCmp* gameStateComponent;

	std::vector<DecisionNode*> decision_tree;
	int timeSinceLastMissile = 0;
};
