#include "EnemySystem.hpp"

EnemySystem::EnemySystem(ObjectManager om, CollisionCmp cc, TransformCmp tc, EnemyCmp ec) {
	objectManager = om;
	collisionComponent = cc;
	transformComponent = tc;
	enemyComponent = ec;

	// Initialize decision tree
	DecisionNode* decisionNode = new DecisionNode(1, 2);
	decision_tree.emplace_back(decisionNode);
	decisionNode = new DecisionNode(RETURN_TO_PATROL);
	decision_tree.emplace_back(decisionNode);
	decisionNode = new DecisionNode(3, 4);
	decision_tree.emplace_back(decisionNode);
	decisionNode = new DecisionNode(CHASE_SAM);
	decision_tree.emplace_back(decisionNode);
	decisionNode = new DecisionNode(PATROL);
	decision_tree.emplace_back(decisionNode);
}

void EnemySystem::update(float elapsed_ms) {
	float ENEMY_SPEED = 50.f;
	float step = ENEMY_SPEED * (elapsed_ms / 1000);

	Entity *sam = objectManager.getEntity(SAMS_GUID);
	Transform* samTransform = transformComponent.getTransform(sam);

	for (auto& it : enemyComponent.getmap()) {

		Enemy *enemy = it.second;

<<<<<<< HEAD
			Transform *et =  transformComponent.getTransform(objectManager.getEntity(it.first));

			if (enemy->patrolX != 0) {
				if (et->movementDirection == NO_DIRECTION) {
					et->movementDirection = RIGHT;
					enemy->start = et->m_position;
				}

				if (et->m_position.x >= (enemy->start.x + enemy->patrolX)) {
					et->movementDirection = LEFT;
					if (et->m_scale.x > 0) {
						et->m_scale.x *= -1;
					}
				}

				if (et->m_position.x < enemy->start.x) {
					et->movementDirection = RIGHT;
				}

				if (et->movementDirection == LEFT) {
					et->m_position.x = et->m_position.x - step;
				}

				if (et->movementDirection == RIGHT) {
					et->m_position.x = et->m_position.x + step;
					if (et->m_scale.x < 0) {
						et->m_scale.x *= -1;
					}
				}

			}
			else {
				if (et->movementDirection == NO_DIRECTION) {
					et->movementDirection = UP;
					enemy->start = et->m_position;
				}

				if (et->m_position.y > (enemy->start.y + enemy->patrolY)) {
					et->movementDirection = DOWN;
					et->m_scale.x *= -1;
				}

				if (et->m_position.y < enemy->start.y) {
					et->movementDirection = UP;
					et->m_scale.x *= -1;
				}

				if (et->movementDirection == DOWN) {
					et->m_position.x = et->m_position.x + step;
				}

				if (et->movementDirection == UP) {
					et->m_position.x = et->m_position.x - step;
				}
			}
=======
		// Set enemy to either chase or patrol depending on decision tree
		handleEnemyDecisionTree(enemy, samTransform);
		Transform *et =  transformComponent.getTransform(objectManager.getEntity(it.first));

		if (enemy->action == PATROL) {
			patrolEnemy(enemy, et, step);
		} else if (enemy->action == CHASE_SAM) {
			chaseSam(enemy, et, samTransform, step);
		} else if (enemy->action == RETURN_TO_PATROL) {
			returnToPatrolPosition(enemy, et, step);
		}
	}
}

// Set the enemy to chase Sam
void EnemySystem::chaseSam(Enemy* enemy, Transform* et, Transform* st, float step) {
	vec2 enemyPosition = et->m_position;
	vec2 samPosition = st->m_position;

	if (enemyPosition.x > samPosition.x) {
		enemyPosition = { enemyPosition.x - step, enemyPosition.y };
	}

	if (enemyPosition.x < samPosition.x) {
		enemyPosition = { enemyPosition.x + step, enemyPosition.y };
	}

	if (enemyPosition.y > samPosition.y) {
		enemyPosition = { enemyPosition.x, enemyPosition.y - step };
	}

	if (enemyPosition.y < samPosition.y) {
		enemyPosition = { enemyPosition.x, enemyPosition.y + step };
	}

	et->m_position = enemyPosition;
}

// Set the enemy to return to starting position
void EnemySystem::returnToPatrolPosition(Enemy* enemy, Transform* et, float step) {
	vec2 enemyPosition = et->m_position;
	vec2 startPosition = et->start;

	if (enemyPosition.x > startPosition.x) {
		enemyPosition = { enemyPosition.x - step, enemyPosition.y };
	}

	if (enemyPosition.x < startPosition.x) {
		enemyPosition = { enemyPosition.x + step, enemyPosition.y };
	}

	if (enemyPosition.y > startPosition.y) {
		enemyPosition = { enemyPosition.x, enemyPosition.y - step };
	}

	if (enemyPosition.y < startPosition.y) {
		enemyPosition = { enemyPosition.x, enemyPosition.y + step };
	}

	if (enemyPosition.x == startPosition.x && enemyPosition.y == startPosition.y) {
		enemy->action = PATROL;
	}

	et->m_position = enemyPosition;
}

// Set the enemy on patrol
void EnemySystem::patrolEnemy(Enemy* enemy, Transform* et, float step) {
	if (enemy->patrolX != 0) {
		if (et->movementDirection == NO_DIRECTION) {
			et->movementDirection = RIGHT;
			enemy->start = et->m_position;
		}

		if (et->m_position.x >= (enemy->start.x + enemy->patrolX)) {
			et->movementDirection = LEFT;
			et->m_scale.x *= -1;
		}

		if (et->m_position.x < enemy->start.x) {
			et->movementDirection = RIGHT;
		}
>>>>>>> Checkpoint for enemy decision tree

		if (et->movementDirection == LEFT) {
			et->m_position.x = et->m_position.x - step;
		}

		if (et->movementDirection == RIGHT) {
			et->m_position.x = et->m_position.x + step;
			et->m_scale.x = abs(et->m_scale.x);
		}

	}
	else {
		if (et->movementDirection == NO_DIRECTION) {
			et->movementDirection = UP;
			enemy->start = et->m_position;
		}

		if (et->m_position.y > (enemy->start.y + enemy->patrolY)) {
			et->movementDirection = DOWN;
			et->m_scale.x *= -1;
		}

		if (et->m_position.y < enemy->start.y) {
			et->movementDirection = UP;
			et->m_scale.x *= -1;
		}

		if (et->movementDirection == DOWN) {
			et->m_position.x = et->m_position.x + step;
		}

		if (et->movementDirection == UP) {
			et->m_position.x = et->m_position.x - step;
		}
	}
}

// Given an enemy and where Sam is, decide what the next action of the
// enemy should be
void EnemySystem::handleEnemyDecisionTree(Enemy* enemy, Transform* samTransform) {
	int action;
	DecisionNode* currNode = decision_tree.at(0); 	// Root node
	int nextNodePos = currNode->getNextNode(!enemy->chase);

	// Traverse enemy decision tree
	currNode = decision_tree.at(nextNodePos);
	if (currNode->m_isEndNode) {
		action = currNode->getAction();
	}
	else {			// Find next next node
		// Is Sam on the right side of screen?
		nextNodePos = currNode->getNextNode(samTransform->m_position.x >= 600);
		currNode = decision_tree.at(nextNodePos);

		if (currNode->m_isEndNode) {
			action = currNode->getAction();
		}
	}

	// Set enemy to chase or patrol depending on decision found
	if (action == PATROL) {
		enemy->chase = false;
	}
	if (action == CHASE_SAM) {
		enemy->chase = true;
	}
}
