#include "EnemySystem.hpp"

void EnemySystem::init(ObjectManager om, CollisionCmp cc, TransformCmp tc, EnemyCmp ec, MovementCmp mc) {
	objectManager = om;
	collisionComponent = cc;
	transformComponent = tc;
	enemyComponent = ec;
	movementComponent = mc;

	DecisionNode* decisionNode = new DecisionNode(1, 2);
	decision_tree.emplace_back(decisionNode);
	decisionNode = new DecisionNode(3, 5);
	decision_tree.emplace_back(decisionNode);
	decisionNode = new DecisionNode(4, 5);
	decision_tree.emplace_back(decisionNode);
	decisionNode = new DecisionNode(RETURN_TO_PATROL);
	decision_tree.emplace_back(decisionNode);
	decisionNode = new DecisionNode(CHASE_SAM);
	decision_tree.emplace_back(decisionNode);
	decisionNode = new DecisionNode(MAINTAIN_ACTION);
	decision_tree.emplace_back(decisionNode);
}

void EnemySystem::update(float elapsed_ms) {
	Entity *sam = objectManager.getEntity(SAMS_GUID);
	Transform* samTransform = transformComponent.getTransform(sam);

	for (auto& it : enemyComponent.getmap()) {

		Enemy *enemy = it.second;
		// Set enemy to either chase or patrol depending on decision tree
		handleEnemyDecisionTree(enemy, samTransform);
		Entity* enemyEntity = objectManager.getEntity(it.first);
		Transform *et =  transformComponent.getTransform(enemyEntity);

		if (enemy->action == PATROL) {
			patrolEnemy(enemy, enemyEntity, et);
		} else if (enemy->action == CHASE_SAM) {
			chaseSam(enemy, et, samTransform, enemyEntity);
		} else if (enemy->action == RETURN_TO_PATROL) {
			returnToPatrolPosition(enemy, et, enemyEntity, elapsed_ms);
		}
	}
}

// Set the enemy to chase Sam
void EnemySystem::chaseSam(Enemy* enemy, Transform* et, Transform* st, Entity* enemyEntity) {
	vec2 enemyPosition = et->m_position;
	vec2 samPosition = st->m_position;

	if (enemyPosition.x > samPosition.x) {
		movementComponent.removeMovementDirection(enemyEntity, RIGHT);
		movementComponent.setMovementDirection(enemyEntity, LEFT);
	}

	if (enemyPosition.x < samPosition.x) {
		movementComponent.removeMovementDirection(enemyEntity, LEFT);
		movementComponent.setMovementDirection(enemyEntity, RIGHT);
	}

	if (enemyPosition.y > samPosition.y) {
		movementComponent.removeMovementDirection(enemyEntity, DOWN);
		movementComponent.setMovementDirection(enemyEntity, UP);
	}

	if (enemyPosition.y < samPosition.y) {
		movementComponent.removeMovementDirection(enemyEntity, UP);
		movementComponent.setMovementDirection(enemyEntity, DOWN);
	}
}

// Set the enemy to return to starting position
void EnemySystem::returnToPatrolPosition(Enemy* enemy, Transform* et, Entity* enemyEntity, float elapsed_ms) {
	vec2 enemyPosition = et->m_position;
	vec2 startPosition = enemy->start;
	float step = movementComponent.getStep(enemyEntity, elapsed_ms);

	// Have to simulate movement in this method so that we can check if the enemy
	// is back to their starting position, but leave it to MovementSystem to actually
	// move the entity
	if (enemyPosition.x > startPosition.x) {
		enemyPosition = { enemyPosition.x - step, enemyPosition.y };
		movementComponent.removeMovementDirection(enemyEntity, RIGHT);
		movementComponent.setMovementDirection(enemyEntity, LEFT);
	}

	if (enemyPosition.x < startPosition.x) {
		enemyPosition = { enemyPosition.x + step, enemyPosition.y };
		movementComponent.removeMovementDirection(enemyEntity, LEFT);
		movementComponent.setMovementDirection(enemyEntity, RIGHT);
	}

	if (enemyPosition.y > startPosition.y) {
		enemyPosition = { enemyPosition.x, enemyPosition.y - step };
		movementComponent.removeMovementDirection(enemyEntity, DOWN);
		movementComponent.setMovementDirection(enemyEntity, UP);
	}

	if (enemyPosition.y < startPosition.y) {
		enemyPosition = { enemyPosition.x, enemyPosition.y + step };
		movementComponent.removeMovementDirection(enemyEntity, UP);
		movementComponent.setMovementDirection(enemyEntity, DOWN);
	}

	// If position will no longer change (as close to starting position as possible),
	// Set back to patrol
	if (enemyPosition.x == et->m_position.x && enemyPosition.y == et->m_position.y) {
		enemy->action = PATROL;
		movementComponent.resetMovementDirection(enemyEntity);
	}
}

// Set the enemy on patrol
void EnemySystem::patrolEnemy(Enemy* enemy, Entity* enemyEntity, Transform* et) {
	if (enemy->patrolX != 0) {
		if (movementComponent.getMovementDirection(enemyEntity) == NO_DIRECTION) {
			movementComponent.setMovementDirection(enemyEntity, RIGHT);
			enemy->start = et->m_position;
		}

		if (et->m_position.x >= (enemy->start.x + enemy->patrolX)) {
			movementComponent.removeMovementDirection(enemyEntity, RIGHT);
			movementComponent.setMovementDirection(enemyEntity, LEFT);
			et->m_scale.x *= -1;
		}

		if (et->m_position.x < enemy->start.x) {
			movementComponent.removeMovementDirection(enemyEntity, LEFT);
			movementComponent.setMovementDirection(enemyEntity, RIGHT);
		}
	}
	else {
		if (movementComponent.getMovementDirection(enemyEntity) == NO_DIRECTION) {
			movementComponent.setMovementDirection(enemyEntity, UP);
			enemy->start = et->m_position;
		}

		if (et->m_position.y > (enemy->start.y + enemy->patrolY)) {
			movementComponent.removeMovementDirection(enemyEntity, UP);
			movementComponent.setMovementDirection(enemyEntity, DOWN);
			et->m_scale.x *= -1;
		}

		if (et->m_position.y < enemy->start.y) {
			movementComponent.removeMovementDirection(enemyEntity, DOWN);
			movementComponent.setMovementDirection(enemyEntity, UP);
			et->m_scale.x *= -1;
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

	if (nextNodePos == 1) {
		nextNodePos = currNode->getNextNode(enemy->action == CHASE_SAM);
		currNode = decision_tree.at(nextNodePos);
		action = currNode->getAction();
	}
	else if (nextNodePos == 2) {
		nextNodePos = currNode->getNextNode(samTransform->m_position.x <= 600);
		currNode = decision_tree.at(nextNodePos);
		action = currNode->getAction();
	}

	if (action != MAINTAIN_ACTION) {
		enemy->action = action;
	}
}
