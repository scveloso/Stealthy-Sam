#pragma once
#include "Cmp.hpp"

class EnemyCmp : public Cmp
{
public:
	void add(Entity* entity, int patrolX, int patrolY, float startX, float startY);
	std::unordered_map<int, Enemy *> getmap();
	void updateEnemyAction(int currEnemyAction, int newEnemyAction);
	void updateSpecificEnemyAction(int entityId, int newEnemyAction);
	int getEnemyAction(int entityId);
	void mapdel();

private:
	std::unordered_map<int, Enemy *> enemy_map;
};
