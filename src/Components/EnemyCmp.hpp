#pragma once
#include "Cmp.hpp"

class EnemyCmp : public Cmp
{
public:
	void add(Entity* entity, int patrolX, int patrolY);
	std::unordered_map<int, Enemy *> getmap();
private:
	std::unordered_map<int, Enemy *> enemy_map;
};
