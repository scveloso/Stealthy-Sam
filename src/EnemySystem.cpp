#include "EnemySystem.hpp"

EnemySystem::EnemySystem(ObjectManager om, CollisionCmp cc, TransformCmp tc, EnemyCmp ec) {
	objectManager = om;
	collisionComponent = cc;
	transformComponent = tc;
	enemyComponent = ec;
}

void EnemySystem::update(float elapsed_ms) {

	float ENEMY_SPEED = 50.f;
	float step = ENEMY_SPEED * (elapsed_ms / 1000);

	for (auto& it : enemyComponent.getmap()) {

		Enemy *enemy = it.second;

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

	}
}
