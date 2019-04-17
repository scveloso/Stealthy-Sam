#include <Components/GameStateCmp.hpp>
#include "EnemySystem.hpp"
#include "UpdateAction.hpp"
#include "TileConstants.hpp"

// System to update enemies based on decision tree AI.
//
// Has access to TransformCmp to properly handle enemy patrol, chasing, returning to patrol.
// Has access to EnemyCmp to be able to change/acess enemy state/type/characteristics.
// Has access to MovementCmp to set where and how enemy should move.
// Has access to ItemCmp to chase thrown torches, etc.

/* Current enemy decision tree:
+-------------+--------------------------------+-----------------------------+---------------------------------+
| Node Number |            Decision            |           Action            |             Action              |
+-------------+--------------------------------+-----------------------------+---------------------------------+
|           0 | Is Sam hidden?                 | Hidden? Go to node 1        | Not hidden? Go to node 4        |
|           1 | Is enemy chasing Sam?          | Chasing? Go to node 2       | Not chasing? Go to node 4       |
|           2 | End                            | Return to patrol            | Return to patrol                |
|           3 | Is enemy chasing torch?        | Chasing torch? Go to node 4 | Not chasing torch? Go to node 5 |
|           4 | End                            | Maintain Action             | Maintain Action                 |
+-------------+--------------------------------+-----------------------------+---------------------------------+ */
void EnemySystem::init(ObjectManager* om, TransformCmp* tc, EnemyCmp ec, MovementCmp* mc, ItemCmp itc, GameStateCmp* gsc) {
	objectManager = om;
	transformComponent = tc;
	enemyComponent = ec;
	movementComponent = mc;
	itemComponent = itc;
	gameStateComponent = gsc;

	initDecisionTree();
}

void EnemySystem::initDecisionTree() {
	// Node 0
	DecisionNode* decisionNode = new DecisionNode(1, 4);
	decision_tree.emplace_back(decisionNode);

	// Node 1
	decisionNode = new DecisionNode(2, 4);
	decision_tree.emplace_back(decisionNode);

	// Node 2
	decisionNode = new DecisionNode(RETURN_TO_PATROL);
	decision_tree.emplace_back(decisionNode);

	// Node 3
	decisionNode = new DecisionNode(4, 5);
	decision_tree.emplace_back(decisionNode);

	// Node 4
	decisionNode = new DecisionNode(MAINTAIN_ACTION);
	decision_tree.emplace_back(decisionNode);
}

int EnemySystem::update(float elapsed_ms) {
	Entity *sam = objectManager->getEntity(SAMS_GUID);
	Transform* samTransform = transformComponent->getTransform(sam);

	int returnAction = NO_CHANGE;
	timeSinceLastMissile += (int) elapsed_ms;
//	printf("time to missile %d\n",timeSinceLastMissile);

	for (auto& it : enemyComponent.getmap()) {

		Enemy *enemy = it.second;
        Entity* enemyEntity = objectManager->getEntity(it.first);
        Transform *et =  transformComponent->getTransform(enemyEntity);
		// Set enemy to either chase or patrol depending on decision tree
		if (enemy->type == BOSS_ENEMY_TYPE) {
			handleBossDecisionTree(enemy, et);
		} else {
			handleEnemyDecisionTree(enemy, samTransform);
		}

		switch (enemy->action)
		{
			case PATROL:
				patrolEnemy(enemy, enemyEntity, et, elapsed_ms);
				break;
			case CHASE_SAM:
				chaseTarget(enemy, et, samTransform, enemyEntity);
				break;
			case RETURN_TO_PATROL:
				returnToPatrolPosition(enemy, et, enemyEntity, elapsed_ms);
				break;
			case CHASE_TORCH:
				tryChaseThrownTorch(enemy, et, enemyEntity);
				break;
			case MOVE_TO_TOP_RIGHT_QUAD:
				goToTarget(et->m_position, BOSS_POSITION_TOP_RIGHT, enemyEntity);
				break;
			case MOVE_TO_BOTTOM_LEFT_QUAD:
				goToTarget(et->m_position, BOSS_POSITION_BOTTOM_LEFT, enemyEntity);
				break;
			case MOVE_TO_BOTTOM_RIGHT_QUAD:
				goToTarget(et->m_position, BOSS_POSITION_BOTTOM_RIGHT, enemyEntity);
				break;
			case FADE_AWAY:
				returnAction = GAME_WIN;
				break;
            case BOSS_SHOOT_MISSILE:
                if (timeSinceLastMissile > TIME_BETWEEN_MISSILES) {
                    timeSinceLastMissile = 0;
                    returnAction = SHOOT_MISSILE;
                }
                break;
		    case MAINTAIN_ACTION:
		        break;
			default:
				printf("enemy action not recognized: %d\n", enemy->action);
		}
	}

	return returnAction;
}

// Check if the thrown entity exists, if it does - chase it
// Otherwise, return to patrol
void EnemySystem::tryChaseThrownTorch(Enemy* enemy, Transform* et, Entity* enemyEntity) {
	int thrownTorchId = itemComponent.getThrownTorchId();

	if (thrownTorchId != -1) {
		Entity* thrownTorchEntity = objectManager->getEntity(thrownTorchId);

		if (thrownTorchEntity->label.compare("Torch") == 0 && thrownTorchEntity->active) {
			Transform *tt =  transformComponent->getTransform(thrownTorchEntity);
			chaseTarget(enemy, et, tt, enemyEntity);
			return;
		}
	}

	enemy->action = RETURN_TO_PATROL;
}

// Set the enemy to chase the target
void EnemySystem::chaseTarget(Enemy* enemy, Transform* et, Transform* gt, Entity* enemyEntity) {
	vec2 enemyPosition = et->m_position;
	vec2 targetPosition = gt->m_position;

	float diff_y= (enemyPosition.y-targetPosition.y)*(enemyPosition.y-targetPosition.y);
  float diff_x= (enemyPosition.x-targetPosition.x)*(enemyPosition.x-targetPosition.x);
	if (enemyPosition.y > targetPosition.y){
		movementComponent->removeMovementDirection(enemyEntity, DOWN);
		movementComponent->setMovementDirection(enemyEntity, UP);
		if (diff_y > diff_x){
			transformComponent->hardSetFacingDirection(enemyEntity, UP);
		}
	}
	if(enemyPosition.y < targetPosition.y) {
		movementComponent->removeMovementDirection(enemyEntity, UP);
		movementComponent->setMovementDirection(enemyEntity, DOWN);
		if (diff_y > diff_x ){
			transformComponent->hardSetFacingDirection(enemyEntity, DOWN);
		}
	}
	if (enemyPosition.x > targetPosition.x) {
		movementComponent->removeMovementDirection(enemyEntity, RIGHT);
		movementComponent->setMovementDirection(enemyEntity, LEFT);
		if (diff_y <= diff_x){
			transformComponent->hardSetFacingDirection(enemyEntity, LEFT);
		}
	}
	if (enemyPosition.x < targetPosition.x) {
		movementComponent->removeMovementDirection(enemyEntity, LEFT);
		movementComponent->setMovementDirection(enemyEntity, RIGHT);
		if (diff_y <= diff_x){
			transformComponent->hardSetFacingDirection(enemyEntity, RIGHT);
		}
	}
}

// Set the enemy to return to starting position
void EnemySystem::returnToPatrolPosition(Enemy* enemy, Transform* et, Entity* enemyEntity, float elapsed_ms) {
	vec2 enemyPosition = et->m_position;
	vec2 startPosition = enemy->start;
	float step = movementComponent->getStep(enemyEntity, elapsed_ms);

	// Have to simulate movement in this method so that we can check if the enemy
	// is back to their starting position, but leave it to MovementSystem to actually
	// move the entity
	if (enemyPosition.y > startPosition.y) {
		enemyPosition = { enemyPosition.x, enemyPosition.y - step };
		movementComponent->removeMovementDirection(enemyEntity, DOWN);
		movementComponent->setMovementDirection(enemyEntity, UP);
		transformComponent->hardSetFacingDirection(enemyEntity, UP);
	}

	if (enemyPosition.y < startPosition.y) {
		enemyPosition = { enemyPosition.x, enemyPosition.y + step };
		movementComponent->removeMovementDirection(enemyEntity, UP);
		movementComponent->setMovementDirection(enemyEntity, DOWN);
		transformComponent->hardSetFacingDirection(enemyEntity, DOWN);
	}

	if (enemyPosition.x > startPosition.x) {
		enemyPosition = { enemyPosition.x - step, enemyPosition.y };
		movementComponent->removeMovementDirection(enemyEntity, RIGHT);
		movementComponent->setMovementDirection(enemyEntity, LEFT);
		transformComponent->hardSetFacingDirection(enemyEntity, LEFT);
	}

	if (enemyPosition.x < startPosition.x) {
		enemyPosition = { enemyPosition.x + step, enemyPosition.y };
		movementComponent->removeMovementDirection(enemyEntity, LEFT);
		movementComponent->setMovementDirection(enemyEntity, RIGHT);
		transformComponent->hardSetFacingDirection(enemyEntity, RIGHT);
	}

	// If position within half a tile radius away from starting position, start patrolling again
	if (enemyPosition.x <= (startPosition.x + (TILE_WIDTH / 2)) &&
			enemyPosition.x >= (startPosition.x - (TILE_WIDTH / 2)) &&
			enemyPosition.y >= (startPosition.y - (TILE_HEIGHT / 2)) &&
			enemyPosition.y <= (startPosition.y + (TILE_HEIGHT / 2)))
	{
		enemy->action = PATROL;
		movementComponent->resetMovementDirection(enemyEntity);
		enemy->t = 0.0f;
		enemy->dir = 1;
	}

	// If position will no longer change (as close to starting position as possible),
	// Set back to patrol
	// if (enemyPosition.x == et->m_position.x && enemyPosition.y == et->m_position.y) {
	// 	enemy->action = PATROL;
	// 	movementComponent->resetMovementDirection(enemyEntity);
	// }
}

// Set the enemy on patrol
void EnemySystem::patrolEnemy(Enemy* enemy, Entity* enemyEntity, Transform* et, float elapsed_ms) {

	if (movementComponent->getMovementDirection(enemyEntity) == NO_DIRECTION) {
		enemy->start = et->m_position;
		et->facingDirection = RIGHT;
		movementComponent->setMovementDirection(enemyEntity, 99);
	}
	else {

		vec2 b = enemy->start;
		vec2 c = { b.x + enemy->patrolX + 100, b.y };
		vec2 a = { b.x, b.y - 900 };
		vec2 d = { c.x, c.y - 900 };
		std::vector<vec2> spline;
		spline.emplace_back(a);
		spline.emplace_back(b);
		spline.emplace_back(c);
		spline.emplace_back(d);

		if (enemy->t < 0.99f) {

			vec2 ans = crSpline(enemy->t += 0.005, spline, enemy->dir);
			transformComponent->getTransform(enemyEntity)->m_position = ans;

		}
		else {
			enemy->t = 0.0f;

			if (enemy->dir == 1) {
				enemy->dir = 0;
				et->facingDirection = LEFT;
			}
			else {
				enemy->dir = 1;
				et->facingDirection = RIGHT;
			}
		}
	}
}

// Given an enemy and where Sam is, decide what the next action of the
// enemy should be
void EnemySystem::handleEnemyDecisionTree(Enemy* enemy, Transform* samTransform) {
	int action = RETURN_TO_PATROL; // default action
	DecisionNode* currNode = decision_tree.at(0); 	// Root node
	int nextNodePos = currNode->getNextNode(!samTransform->visible);
	currNode = decision_tree.at(nextNodePos);

	// Node 1's children nodes are both end nodes
	if (nextNodePos == 1) {
		nextNodePos = currNode->getNextNode(enemy->action == CHASE_SAM);
		currNode = decision_tree.at(nextNodePos);
		action = currNode->getAction();
	}

	action = currNode->getAction();

	if (action != MAINTAIN_ACTION && action != enemy->action) {
		enemy->action = action;
	}
}

void EnemySystem::goToTarget(vec2 startPosition, vec2 targetPosition, Entity* entity) {
	float diff_y= (startPosition.y-targetPosition.y)*(startPosition.y-targetPosition.y);
	float diff_x= (startPosition.x-targetPosition.x)*(startPosition.x-targetPosition.x);

	if (startPosition.y > targetPosition.y){
		movementComponent->removeMovementDirection(entity, DOWN);
		movementComponent->setMovementDirection(entity, UP);
		if (diff_y > diff_x){
			transformComponent->hardSetFacingDirection(entity, UP);
		}
	}
	if(startPosition.y < targetPosition.y) {
		movementComponent->removeMovementDirection(entity, UP);
		movementComponent->setMovementDirection(entity, DOWN);
		if (diff_y > diff_x ){
			transformComponent->hardSetFacingDirection(entity, DOWN);
		}
	}
	if (startPosition.x > targetPosition.x) {
		movementComponent->removeMovementDirection(entity, RIGHT);
		movementComponent->setMovementDirection(entity, LEFT);
		if (diff_y <= diff_x){
			transformComponent->hardSetFacingDirection(entity, LEFT);
		}
	}
	if (startPosition.x < targetPosition.x) {
		movementComponent->removeMovementDirection(entity, LEFT);
		movementComponent->setMovementDirection(entity, RIGHT);
		if (diff_y <= diff_x){
			transformComponent->hardSetFacingDirection(entity, RIGHT);
		}
	}
}

void EnemySystem::handleBossDecisionTree(Enemy* enemy, Transform* bossTransform) {
	// TODO(sam): shoot missile, go to other locations, etc
	enemy->action = BossStrategy::handleAction(gameStateComponent, bossTransform->m_position);
}
