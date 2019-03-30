#include "EnemyCmp.hpp"
#include "Strategies/strategies_common.hpp"

void EnemyCmp::add(Entity* entity, int patrolX, int patrolY) {
	Enemy *enemy = new Enemy();
	enemy->type = NORMAL;
	enemy->action = PATROL;
	enemy->patrolX = patrolX;
	enemy->patrolY = patrolY;
	enemy->start = { 0,0 };
	enemy->t = 0.0f;
	enemy->dir = 1;
	enemy_map[entity->id] = enemy;
}

std::unordered_map<int, Enemy *> EnemyCmp::getmap() {
	return enemy_map;
}

// For all enemies doing currEnemyAction, change to newEnemyAction
void EnemyCmp::updateEnemyAction(int currEnemyAction, int newEnemyAction) {
	for (auto& it : enemy_map)
	{
		Enemy* enemy = it.second;
		if (enemy->action == currEnemyAction) {
			enemy->action = newEnemyAction;
		}
	}
}

void EnemyCmp::mapdel() {

	for (auto& it : enemy_map) {
		delete it.second;
	}
	enemy_map.clear();
}
