#include "EnemyCmp.hpp"

void EnemyCmp::add(Entity* entity, int patrolX) {
	Enemy *enemy = new Enemy();
	enemy->type = NORMAL;
	enemy->chase = false;
	enemy->patrolX = patrolX;
	enemy->start = { 0,0 };
	enemy_map[entity->id] = enemy;
}

std::unordered_map<int, Enemy *> EnemyCmp::getmap() {
	return enemy_map;
}