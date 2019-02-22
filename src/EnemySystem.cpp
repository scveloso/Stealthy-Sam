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
				if (et->direction == NO_DIRECTION) {
					et->direction = RIGHT;
					enemy->start = et->m_position;
				}

				if (et->m_position.x >= (enemy->start.x + enemy->patrolX)) {
					et->direction = LEFT;
					et->m_scale.x *= -1;
				}

				if (et->m_position.x < enemy->start.x) {
					et->direction = RIGHT;
				}

				if (et->direction == LEFT) {
					et->m_position.x = et->m_position.x - step;
				}

				if (et->direction == RIGHT) {
					et->m_position.x = et->m_position.x + step;
					et->m_scale.x = abs(et->m_scale.x);
				}

			}
			else {
				if (et->direction == NO_DIRECTION) {
					et->direction = UP;
					enemy->start = et->m_position;
				}

				if (et->m_position.x > (enemy->start.x + enemy->patrolX)) {
					et->direction = DOWN;
					et->m_scale.x *= -1;
				}

				if (et->m_position.x < enemy->start.x) {
					et->direction = UP;
					et->m_scale.x *= -1;
				}

				if (et->direction == DOWN) {
					et->m_position.x = et->m_position.x + step;
				}

				if (et->direction == UP) {
					et->m_position.x = et->m_position.x - step;
				}
			}
		
	}
}

