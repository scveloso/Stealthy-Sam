#include "EnemyCmp.hpp"
#include "EnemyAction.hpp"

void EnemyCmp::add(Entity* entity, int patrolX, int patrolY) {
	Enemy *enemy = new Enemy();
	enemy->type = NORMAL;
	enemy->action = PATROL;
	enemy->patrolX = patrolX;
	enemy->patrolY = patrolY;
	enemy->start = { 0,0 };
	enemy_map[entity->id] = enemy;
}

std::unordered_map<int, Enemy *> EnemyCmp::getmap() {
	return enemy_map;
}
